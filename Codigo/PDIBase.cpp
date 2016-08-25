// Membros da classe CPDIBase 

#include "stdafx.h"
#include "PDImagem.h"
#include "PDImagemDoc.h" 
#include "PDImagemView.h"
#include "PDIBase.h"

#include <afxtempl.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>  // para fprintf, fopen ...
#include <time.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define PDI_DEBUG

double *buffer;

extern CObList ObjectList;
extern POSITION pos;
extern BOOL Inserir; 
      
// Construtor da Classe CPDIBase
CPDIBase::CPDIBase()
{
	bmWidthBytes = 0;
	Heightm1 = 0;
	palSize = 0;
	iBPP = 0;
	hDIB = NULL;
	pDIB = NULL;
}

// Destrutor da Classe CPDIBase
CPDIBase::~CPDIBase()
{
	if (hDIB != NULL) 
	{
		GlobalUnlock(hDIB);
		GlobalFree(hDIB);
	}
} 

void CPDIBase::InitDIBData()
{ 
	if (pal != NULL)
		pal = NULL;

	if (hDIB == NULL)
		return;

	pRgn = NULL;
	Region = Pinta = FALSE;

	// Set up document size
	pDIB = (BYTE *) ::GlobalLock((HGLOBAL) hDIB);
	lpBI = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hDIB);

	/* get pointer to BITMAPINFO (Win 3.0) */
	lpbmi = (LPBITMAPINFO)pDIB;

	/* get pointer to BITMAPCOREINFO (old 1.x) */
	lpbmc = (LPBITMAPCOREINFO)pDIB;
	bmWidthBytes = 4 * ( (GetWidth() * GetBitsPerPixel() + 31) / 32 );

	lpBits = FindBits();

	if (GetWidth() > INT_MAX || GetHeight() > INT_MAX)
	{
		::GlobalUnlock((HGLOBAL) hDIB);
		::GlobalFree((HGLOBAL) hDIB);
		hDIB = NULL;
		MessageBox(NULL, "DIB is too large", NULL,MB_ICONINFORMATION | MB_OK);
		return;
	}
	m_sizeDOC = CSize((int) GetWidth(), (int) GetHeight());
	::GlobalUnlock((HGLOBAL) hDIB);

	// Create copy of palette
	pal = new CPalette;
	if (pal == NULL)
	{
		// we must be really low on memory
		::GlobalFree((HGLOBAL) hDIB);
		hDIB = NULL;
		return;
	}
	if (CreateDIBPalette() == NULL)
	{
		// DIB may not have a palette
		delete pal;
		pal = NULL;
		return;
	}
}

// Verifica se existe uma Paleta
BOOL CPDIBase::IsPaletted() const
{
	WORD BPPixel;  

	if (IS_WIN30_DIB(lpBI))
		BPPixel = ((LPBITMAPINFOHEADER)lpBI)->biBitCount;
	else
		BPPixel = ((LPBITMAPCOREHEADER)lpBI)->bcBitCount;

	switch(BPPixel) 
	{
		case  1:
		case  4:
		case  8:          
			return TRUE;
		default:
			return FALSE;
	}
}  

// Retorna numero de bits por pixels - Acho que é nessa classe que resolvemos tudo, para saber se a figura é escala de cinza ou colorida
WORD CPDIBase::GetBitsPerPixel() const
{
    if (IS_WIN30_DIB(lpBI))
        return ((LPBITMAPINFOHEADER)lpBI)->biBitCount;
    else
        return ((LPBITMAPCOREHEADER)lpBI)->bcBitCount;
}

// Retorna o indice da cor mais proximada paleta
WORD CPDIBase::GetNearestPaletteIndex( DWORD dwColor ) const
{
	if (!IsPaletted()) 
		return 0;
	return (WORD) pal->GetNearestPaletteIndex( dwColor );
}

// Retorna as entradas da paleta
WORD CPDIBase::GetPaletteEntries( WORD wStartIndex, WORD wNumEntries,LPPALETTEENTRY pPaletteColors ) const
{
	if (!IsPaletted()) 
		return 0;
	return (WORD) pal->GetPaletteEntries(wStartIndex, wNumEntries, pPaletteColors);
}

// Retorna o tamanho da paleta
WORD CPDIBase::GetPaletteSize() 
{
	if (!IsPaletted()) 
		return 0;
	if (IS_WIN30_DIB (lpBI))
		return (WORD)(DIBNumColors() * sizeof(RGBQUAD));
	else
		return (WORD)(DIBNumColors() * sizeof(RGBTRIPLE));
}
 

// Retorna a cor na posicao (col, lin)
DWORD CPDIBase::GetPixel(DWORD col, DWORD lin) const
{
	BYTE* lpTemp = (BYTE*)lpBits;
	lpTemp += bmWidthBytes *  lin;      //( bmWidthBytes * ( (GetHeight() -1) - y ) ); // Height - 1 - y
	
	//	PALETTEENTRY pe;
	switch(GetBitsPerPixel()) 
	{
		case  1:
			lpTemp += (LONG) col / 8;
			return ((*lpTemp) >> (7-(col % 8))) & 1;
//			pal->GetPaletteEntries( ((*lpTemp) >> (7-(col % 8))) & 1, 1, &pe );
//			return RGB( pe.peRed, pe.peGreen, pe.peBlue );
		case  4:
//			UINT idx;
			lpTemp += (LONG) col / 2;
			if (col % 2) 
				return	*lpTemp & 0x0F;
			else 
				return	*lpTemp >> 4;
		case  8:
			lpTemp += (LONG) col;
			return *lpTemp;
//			pal->GetPaletteEntries( *lpTemp, 1, &pe );
//			return RGB( pe.peRed, pe.peGreen, pe.peBlue );
		case 24:
			lpTemp += (LONG) 3 * col;
			return RGB(*(lpTemp+2), *(lpTemp+1), *(lpTemp));
	}
	return (DWORD) 0;
}   

// Troca a cor na posicao (col, lin)
void CPDIBase::SetPixel(DWORD col, DWORD lin, COLORREF dwColor)
{ 
	BYTE* lpTemp = (BYTE*)lpBits;
	lpTemp += bmWidthBytes *  lin;      //( bmWidthBytes * ( (GetHeight() -1) - y ) ); // Height - 1 - y
	BYTE bt = *lpTemp;

	switch(GetBitsPerPixel()) 
	{
		case  1:
		{
			lpTemp += (LONG) col / 8;
			BYTE tab[8] = {0x7F, 0xBF,  0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};
			BYTE mask = tab[col % 8];

			*lpTemp &= mask;
			*lpTemp |= ( (dwColor && 0x01) << (7 - (col % 8)));
			break;
		}
		case  4:
		{
			lpTemp += (LONG) col / 2;
			if (col % 2)
				bt = (BYTE) ( (bt & 0xF0) | (dwColor & 0x0F) );
			else
				bt = (BYTE) ( (bt & 0x0F) | ((dwColor & 0x0F) << 4) );

			*lpTemp = bt;
			break;
		}
		case  8:
		{
			lpTemp += (LONG) col;
			*lpTemp = (BYTE) (dwColor & 0xFF);
			break;
		}
		case 24:
		{
			lpTemp += (LONG) 3 * col;
			*(lpTemp+2) = GetRValue(dwColor);
			*(lpTemp+1) = GetGValue(dwColor);
			*(lpTemp+0) = GetBValue(dwColor);
			break;
		}
	}
} 

// Retorna a altura da imagem
LONG CPDIBase::GetHeight() const
{
	if (IS_WIN30_DIB(lpBI))
		return ((LPBITMAPINFOHEADER)lpBI)->biHeight;
	else
		return ((LPBITMAPCOREHEADER)lpBI)->bcHeight;
}

// Retorna a largura da imagem
LONG CPDIBase::GetWidth() const
{ 
    if (IS_WIN30_DIB(lpBI))
        return ((LPBITMAPINFOHEADER)lpBI)->biWidth;
    else
        return ((LPBITMAPCOREHEADER)lpBI)->bcWidth;
}

// Troca entradas da paleta
WORD CPDIBase::SetPaletteEntries( WORD wStartIndex, WORD wNumEntries,
                        LPPALETTEENTRY pPaletteColors )
{
	if (!IsPaletted())
		return 0;

	return (WORD) pal->SetPaletteEntries( wStartIndex, wNumEntries, pPaletteColors );
}

DWORD CPDIBase::GetPixelBW(DWORD x, DWORD y) const
{
	BYTE * lpTemp = lpBits;

	lpTemp += ( bmWidthBytes * ( (GetHeight() -1) - y ) ); // Height - 1 - y
	int cor;
	switch(GetBitsPerPixel()) 
	{
		case  1:
		{
			lpTemp += (LONG) x/8;
			PALETTEENTRY      pe;
			pal->GetPaletteEntries( ((*lpTemp) >> (7-(x % 8))) & 1, 1, &pe );
			cor = (int) RGB( pe.peRed, pe.peGreen, pe.peBlue );
			break;
		}
		case  4:
		{
			UINT idx;
			lpTemp += (LONG) x/2;
			if (x % 2) 
			{
				idx = *lpTemp & 0x0F;
			}
			else 
				idx = *lpTemp >> 4;

			PALETTEENTRY      pe;
			pal->GetPaletteEntries( idx, 1, &pe );
			cor = (int) RGB( pe.peRed, pe.peGreen, pe.peBlue ); break;
		}
		case  8:
		{
			lpTemp += (LONG) x;
			PALETTEENTRY pe;
			pal->GetPaletteEntries( *lpTemp, 1, &pe );
			cor = (int) RGB( pe.peRed, pe.peGreen, pe.peBlue ); break;
		}
		case 24:
		{
			lpTemp += (LONG) 3*x;
			cor = (int) RGB(*(lpTemp+2), *(lpTemp+1), *(lpTemp));
		}
	}

	if (cor==0) 
		return 0;
	else
		return 1;
}   
  
DWORD CPDIBase::SetPixelBW(DWORD x, DWORD y, DWORD dwColor)
{ 
	BYTE *lpTemp;
	
	lpTemp = lpBits;

	if (dwColor == 1)
		dwColor = 255;
	else 
		dwColor = 0; 	  

	lpTemp += ( bmWidthBytes * ( (GetHeight() -1) - y ) ); // Height - 1 - y

	switch(GetBitsPerPixel())
	{
		case  1:
		{
			lpTemp += (LONG) x/8;
			UINT idx = 7 - (BYTE) (x % 8);
			BYTE bt = *lpTemp;
			BYTE mask = (BYTE) (0x01 << idx);
			bt = (BYTE) ( (bt & mask) | ((dwColor & 0x01) << idx) );
			*lpTemp = bt;

			PALETTEENTRY  pe;
			pal->GetPaletteEntries( (UINT) dwColor, 1, &pe );
			return RGB( pe.peRed, pe.peGreen, pe.peBlue );
		}
		case  4:
		{
			BYTE bt;
			lpTemp += (LONG) x/2;
			bt = *lpTemp;
			if (x % 2)
				bt = (BYTE) ( (bt & 0xF0) | (dwColor & 0x0F) );
			else
				bt = (BYTE) ( (bt & 0x0F) | ((dwColor & 0x0F) << 4) );

			*lpTemp = bt;

			PALETTEENTRY  pe;
			pal->GetPaletteEntries( (UINT) dwColor, 1, &pe );
			return RGB( pe.peRed, pe.peGreen, pe.peBlue );
		}
		case  8:
		{
			lpTemp += (LONG) x;
			*lpTemp = (BYTE) (dwColor & 0xFF);
			PALETTEENTRY  pe;
			pal->GetPaletteEntries( (UINT) dwColor, 1, &pe );
			return RGB( pe.peRed, pe.peGreen, pe.peBlue );
		}
		case 24:
		{
			lpTemp += (LONG) 3*x;
			*(lpTemp+2) = GetRValue(dwColor);
			*(lpTemp+1) = GetGValue(dwColor);
			*(lpTemp)   = GetBValue(dwColor);
			return dwColor;
		}
	}
	return (DWORD) 0;
} 

// Carrega a imagem
HDIB CPDIBase::ReadDIBFile(CFile& file)
{
	DWORD dwBitsSize;

	/* get length of DIB in bytes for use when reading */
	dwBitsSize = file.GetLength();

	/* Go read the DIB file header and check if it's valid. */
	if ((file.Read(&bmfHeader, sizeof(BITMAPFILEHEADER)) !=
		sizeof(BITMAPFILEHEADER)) || (bmfHeader.bfType != DIB_HEADER_MARKER))
	{
		return NULL;
	}

	/* Allocate memory for DIB */
	hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	if (hDIB == 0)
		return NULL;          

	pDIB = (BYTE *) ::GlobalLock((HGLOBAL) hDIB);

	/* Go read the bits. */
	if (file.Read(pDIB, dwBitsSize - sizeof(BITMAPFILEHEADER)) !=
		dwBitsSize - sizeof(BITMAPFILEHEADER) )
	{
		::GlobalUnlock((HGLOBAL) hDIB);
		::GlobalFree((HGLOBAL) hDIB);
		return NULL;
	}
	::GlobalUnlock((HGLOBAL) hDIB);

	return hDIB;
}            

// Salva a imagem
BOOL CPDIBase::SaveDIB(CFile& file)
{
	if (hDIB == NULL)
		return FALSE;
	
	/* test pointer to the DIB memory, the first of which contains **
	** a BITMAPINFO structure                                      */
	if (lpBI == NULL)
		return FALSE;

	if (!IS_WIN30_DIB(lpBI))
	{
		::GlobalUnlock((HGLOBAL) hDIB);
		return FALSE;       // It's an other-style DIB (save not supported)
	}

	/* Fill in the fields of the file header */
	/* Fill in file type (first 2 bytes must be "BM" for a bitmap) */
	bmfHeader.bfType = DIB_HEADER_MARKER;  // "BM"


	dwDIBSize = *(LPDWORD)lpBI + GetPaletteSize();  // Partial Calculation

	// Now calculate the size of the image

	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
		dwDIBSize += lpBI->biSizeImage;
	else
	{
		DWORD dwBmBitsSize;  // Size of Bitmap Bits only

		// It's not RLE, so size is Width (DWORD aligned) * Height

		dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;

		dwDIBSize += dwBmBitsSize;

		lpBI->biSizeImage = dwBmBitsSize;
	}

	// Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)
	bmfHeader.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHeader.bfReserved1 = 0;
	bmfHeader.bfReserved2 = 0;

	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize
											  + GetPaletteSize();
	TRY
	{
		// Write the file header
		file.Write(&bmfHeader, sizeof(BITMAPFILEHEADER));
		//
		// Write the DIB header and the bits
		//
		file.Write(lpBI, dwDIBSize);
	}
	CATCH (CFileException, e)
	{
		::GlobalUnlock((HGLOBAL) hDIB);
		THROW_LAST();
	}
	END_CATCH

	::GlobalUnlock((HGLOBAL) hDIB);
	return TRUE;
}

HDIB CPDIBase::AumentaDIB(int h, int l)
{
	BYTE	*lpCopy;
	BYTE	*lp;
	HDIB    hCopy;
	DWORD   dwLen; 

	int		i, lin, col;
	double	tamanho;
	DWORD	tamret;                        
               
	if (hDIB == NULL)
		return NULL;

	tamret = ((DWORD) h * (DWORD) l);

	dwLen = WIDTHBYTES((l) * ((DWORD)lpBI->biBitCount)) * h ;
	dwLen = dwLen + *(LPDWORD)lpBI + (DWORD) GetPaletteSize();

	int div, resto;
	if (dwLen <= 16)
		dwLen = 16;
	else
	{
		div		= dwLen / 16;
		resto	= dwLen - (div * 16);
		dwLen	= dwLen + (16 - resto);   
	}

	tamanho = (GetHeight() - 1);

	if ((hCopy = (HDIB) ::GlobalAlloc (GHND, dwLen)) != NULL)
	{
		BYTE * lpBitsCopy;
		lpCopy = (BYTE *) ::GlobalLock((HGLOBAL) hCopy);
		LPBITMAPINFOHEADER lpBICopy = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hCopy);
		LPBITMAPINFO lpbmiCopy = (LPBITMAPINFO) ::GlobalLock((HGLOBAL) hCopy);
		LPBITMAPFILEHEADER lpbmfHdrCopy = (LPBITMAPFILEHEADER) ::GlobalLock((HGLOBAL) hCopy);
		*(lpBICopy) = *(lpBI); 
		*(lpbmiCopy) = *(lpbmi);
		lpBICopy->biWidth = l;
		lpBICopy->biHeight = h;
		lpBICopy->biSizeImage = tamret;

		lpBitsCopy = (BYTE *) (lpCopy + *(LPDWORD)lpBICopy + GetPaletteSize());

		for (i = 0; i < DIBNumColors(); i++)
		{                
			lpbmiCopy->bmiColors[i].rgbRed = lpbmi->bmiColors[i].rgbRed;
			lpbmiCopy->bmiColors[i].rgbGreen = lpbmi->bmiColors[i].rgbGreen;
			lpbmiCopy->bmiColors[i].rgbBlue = lpbmi->bmiColors[i].rgbBlue;
			lpbmiCopy->bmiColors[i].rgbReserved = lpbmi->bmiColors[i].rgbReserved; 
		}

//      for (lin = 0; lin < (int) GetHeight(); lin++)
//			for (col = 0; col < (int) GetWidth(); col++)
//			{
//				SetPixel((DWORD) col, (DWORD) lin, 255);      
//          }     

		DWORD  bmWidthBytesCopy;
		bmWidthBytesCopy = 4 * ((l * GetBitsPerPixel() + 31) / 32 );

		lp=lpBitsCopy;
		for (lin=0; lin < h; lin++, lp += bmWidthBytesCopy)  // uma linha
			for (col = 0; col < l; col++) 
				lp[col] = 255;      

		::GlobalUnlock((HGLOBAL) hCopy);
		::GlobalUnlock((HGLOBAL) hDIB);
	}

	return hCopy;
}

// Cria uma paleta para a imagem
BOOL CPDIBase::CreateDIBPalette()
{
	LPLOGPALETTE	lpPal;			// pointer to a logical palette
	HANDLE			hLogPal;		// handle to a logical palette
	HPALETTE		hPal = NULL;	// handle to a palette
	int				i;				// loop index
	WORD			wNumColors;		// number of colors in color table
	BOOL			bWinStyleDIB;	// flag which signifies whether this is a Win3.0 DIB
	BOOL			bResult;

	bResult = FALSE;
	/* if handle to DIB is invalid, return FALSE */
	if (hDIB == NULL)
		return FALSE;

	/* get the number of colors in the DIB */
	wNumColors = DIBNumColors();

	if (wNumColors != 0)
	{
		/* allocate memory block for logical palette */
		hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * wNumColors);

		/* if not enough memory, clean up and return NULL */
		if (hLogPal == 0)
		{
			::GlobalUnlock((HGLOBAL) hDIB);
			return FALSE;
		}

		lpPal = (LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);

		/* set version and number of palette entries */
		lpPal->palVersion		= PALVERSION;
		lpPal->palNumEntries	= (WORD)wNumColors;

		/* is this a Win 3.0 DIB? */
		bWinStyleDIB = IS_WIN30_DIB(pDIB);
		for (i = 0; i < (int)wNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				lpPal->palPalEntry[i].peRed		= lpbmi->bmiColors[i].rgbRed;
				lpPal->palPalEntry[i].peGreen	= lpbmi->bmiColors[i].rgbGreen;
				lpPal->palPalEntry[i].peBlue	= lpbmi->bmiColors[i].rgbBlue;
				lpPal->palPalEntry[i].peFlags	= PC_NOCOLLAPSE;
			}
			else
			{
				lpPal->palPalEntry[i].peRed		= lpbmc->bmciColors[i].rgbtRed;
				lpPal->palPalEntry[i].peGreen	= lpbmc->bmciColors[i].rgbtGreen;
				lpPal->palPalEntry[i].peBlue	= lpbmc->bmciColors[i].rgbtBlue;
				lpPal->palPalEntry[i].peFlags	= PC_NOCOLLAPSE;
			}
		}

		/* create the palette and get handle to it */
		bResult = pal->CreatePalette(lpPal);
		::GlobalUnlock((HGLOBAL) hLogPal);
		::GlobalFree((HGLOBAL) hLogPal);
	}

	::GlobalUnlock((HGLOBAL) hDIB);

	return bResult;
}


// Retorna o numero de cores da imagem

WORD CPDIBase::DIBNumColors()
{
	WORD wBitCount;  // DIB bit count

	if (IS_WIN30_DIB(lpBI))
	{
		DWORD dwClrUsed;

		dwClrUsed = ((LPBITMAPINFOHEADER)lpBI)->biClrUsed;
		if (dwClrUsed != 0)
			return (WORD)dwClrUsed;
	};

	/* Calculate the number of colors in the color table based on **
	**  the number of bits per pixel for the DIB.                 */
	if (IS_WIN30_DIB(lpBI))
		wBitCount = ((LPBITMAPINFOHEADER)lpBI)->biBitCount;
	else
		wBitCount = ((LPBITMAPCOREHEADER)lpBI)->bcBitCount;

	/* return number of colors based on bits per pixel */
	switch (wBitCount)
	{
		case 1:
			return 2;
		case 4:
			return 16;
		case 8:
			return 256;
		default:
			return 0;
	}
}

// Visualizacao da imagem
BOOL CPDIBase::PaintDIB(HDC hDC,LPRECT  lpDCRect, LPRECT  lpDIBRect)
{
	LPSTR    lpDIBHdr;            // Pointer to BITMAPINFOHEADER
	LPSTR    lpDIBBits;           // Pointer to DIB bits
	BOOL     bSuccess=FALSE;      // Success/fail flag
	HPALETTE hPal=NULL;           // Our DIB's palette
	HPALETTE hOldPal=NULL;        // Previous palette

	/* Check for valid DIB handle */
	if (hDIB == NULL)
	return FALSE;

	/* Lock down the DIB, and get a pointer to the beginning of the bit
	*  buffer
	*/
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	lpDIBBits = FindDIBBits(lpDIBHdr);

	/* Make sure to use the stretching mode best for color pictures */
	::SetStretchBltMode(hDC, COLORONCOLOR);

	/* Determine whether to call StretchDIBits() or SetDIBitsToDevice() */
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
		(RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect))) 
	{ 
		bSuccess = ::SetDIBitsToDevice(	hDC,					// hDC
										lpDCRect->left,			// DestX
										lpDCRect->top,			// DestY
										RECTWIDTH(lpDCRect),	// nDestWidth
										RECTHEIGHT(lpDCRect),	// nDestHeight
										lpDIBRect->left,		// SrcX
										(int)GetHeight() -
										lpDIBRect->top -
										RECTHEIGHT(lpDIBRect),	// SrcY
										0,						// nStartScan
										(WORD)GetHeight(),		// nNumScans
										lpDIBBits,				// lpBits
										(LPBITMAPINFO)lpDIBHdr,	// lpBitsInfo
										DIB_COR);				// wUsage
	}                                
	else
	{
		bSuccess = ::StretchDIBits(		hDC,					// hDC
										lpDCRect->left,			// DestX
										lpDCRect->top,			// DestY
										RECTWIDTH(lpDCRect),	// nDestWidth
										RECTHEIGHT(lpDCRect),	// nDestHeight
										lpDIBRect->left,		// SrcX
										lpDIBRect->top,			// SrcY
										RECTWIDTH(lpDIBRect),	// wSrcWidth
										RECTHEIGHT(lpDIBRect),	// wSrcHeight
										lpDIBBits,				// lpBits
										(LPBITMAPINFO)lpDIBHdr,	// lpBitsInfo
										DIB_COR,				// wUsage
										SRCCOPY);				// dwROP
	}

	::GlobalUnlock((HGLOBAL) hDIB);

	return bSuccess;
}


// Retorna um ponteiro (LPTSTR) para os bits (pixels) da imagem
LPSTR CPDIBase::FindDIBBits(LPSTR lpbi)
{
	return (lpbi + *(LPDWORD)lpbi + GetPaletteSize());
}

// Retorna um ponteiro (BYTE) para o bits da imagem
BYTE *CPDIBase::FindBits()
{
	return (BYTE *) (pDIB + *(LPDWORD)lpBI + GetPaletteSize());
}


void CPDIBase::ConverteHSI()
{
	BYTE R,G,B;
	double div,r,g,b,h,s,i,MinimoRGB, PIx2 = (double) (2.0 * PI), UmTerco = (float) (1.0 / 3.0);

	R = peValue.peRed;
	G = peValue.peGreen;
	B = peValue.peBlue;

	//  Converte para HSI    
	r = (double) R / 255; //( R + G + B);
	g = (double) G / 255; //( R + G + B);
	b = (double) B / 255; //( R + G + B);

	// calculo da cor
	div = (float) (sqrt(pow((r - g),2.) + (r - b)*(g - b)));

	if (div != 0.)
		h = (float) (acos( ( ((r - g) + (r - b))/2. )/div ));
	else                               
		h = (float) 0.;

	if (b > g)
		h = PIx2 - h;               

	h = h / PIx2;  

	// calculo da intensidade
	i = UmTerco * (r + g + b);

	// calculo da saturacao
	MinimoRGB = r;
	if (g < MinimoRGB)
		MinimoRGB = g;
	if (b < MinimoRGB)
		MinimoRGB = b;
	if ((r + g + b) != 0) 
		s = (float) (1. - ( (  3./(r + g + b) )* (MinimoRGB)));
	else
		s = (float) 1.;      

	// ajusta escala
	hue			= (float) h;
	saturation	= (float) s;
	intensity	= (float) i;

	H = (BYTE) (h * 255);
	S = (BYTE) (s * 255); 
	I = (BYTE) (i * 255); 
}

//i representa B, i+1 representa G e i+2 representa R
void CPDIBase::OnRGBToYCrCb()
{
	//Aqui o vetor de Byte que representa a imagem é copiado para uma
	//variável temporária
	BYTE* lpTemp = lpBits;
	//pega a altura da imagem
    int h = GetHeight();
	//pega a largura da imagem
	int w = GetWidth();
	//cria um buffer para armazenar as informações reais do espaço de cor YCrCb
//	int size=0;
//	double *buffer = new double[size];
	buffer = new double[h*(w*3)];
	int i;
	
	double Canal_Y=0, Canal_Cr=0,  Canal_Cb=0;
	int Canal_b, Canal_g, Canal_r;

	//Este laço percorre o vetor de Byte, a cada iteração o i é incrementado
	//em três posições, pois um pixel no formato RGB é representado por três
	//elementos.
	for(i = 0; i < h*(w*3); i = i + 3)
	{
		//pega o valor de cada canal do vetor da imagem e armazena temporariamente
		//para facilitar a compreensão do cálculo
		Canal_b = (int)lpTemp[i];
		Canal_g = (int)lpTemp[i+1];
		Canal_r = (int)lpTemp[i+2];
		
		//Aqui ocorrem os cálculos dos canais Y, Cr e Cb
		Canal_Y = (0.257 * Canal_r) + (0.504 * Canal_g) + (0.098 * Canal_b) + 16;
		Canal_Cr = (0.439 * Canal_r) - (0.368 * Canal_g) - (0.071 * Canal_b) + 128;
		Canal_Cb = -(0.148 * Canal_r) - (0.291 * Canal_g) + (0.439 * Canal_b) + 128;

		//Armazena os resultados de YCrCb no buffer para futura
		//conversão de retorno ao sistema RGB 
		buffer[i] = Canal_Cb;
		buffer[i+1] = Canal_Cr;
		buffer[i+2] = Canal_Y;

		//Joga os resultados direto no vetor que representa a imagem
		//tornando mais rápido o acesso aos novos elementos a serem
		//mostrados na tela
		lpTemp[i] = (BYTE) Canal_Cb;
		lpTemp[i+1] = (BYTE) Canal_Cr;
		lpTemp[i+2] = (BYTE) Canal_Y;

	}

}

/*Patin*/

//i representa Cb, i+1 representa Cr e i+2 representa Y
void CPDIBase::OnYCrCbToRGB()
{
	int i;
	//Aqui o vetor de Byte que representa a imagem é copiado para uma
	//variável temporária
	BYTE* lpTemp = lpBits;
	//pega a altura da imagem
    int h = GetHeight();
	//pega a largura da imagem
	int w = GetWidth();

	//Este laço percorre o vetor de Byte, a cada iteração o i é incrementado
	//em três posições, pois um pixel no formato RGB é representado por três
	//elementos.

	double Canal_b, Canal_g, Canal_r;
	double Canal_Y=0, Canal_Cr=0, Canal_Cb=0;

	for(i = 0; i < h*(w*3); i = i + 3)
	{
		//pega o valor de cada canal do buffer e armazena temporariamente
		//para facilitar a compreensão do cálculo
		Canal_Y = buffer[i+2];
		Canal_Cr = buffer[i+1];
		Canal_Cb = buffer[i];

		//Aqui ocorrem os cálculos dos canais B, G e R
		Canal_b = (1.164*(Canal_Y - 16)) + (2.018*(Canal_Cb - 128));
		Canal_g = (1.164*(Canal_Y - 16)) - (0.813*(Canal_Cr - 128)) - (0.391*(Canal_Cb - 128));
		Canal_r = (1.164*(Canal_Y - 16)) + (1.596*(Canal_Cr - 128));

		//Joga os resultados direto no vetor que representa a imagem
		//tornando mais rápido o acesso aos novos elementos a serem
		//mostrados na tela
		lpTemp[i] = (BYTE) Canal_b;
		lpTemp[i+1] = (BYTE) Canal_g;
		lpTemp[i+2] = (BYTE) Canal_r;
	}

}

BOOL CPDIBase::GetRGBHistogram(LPDWORD rCount, LPDWORD gCount, LPDWORD bCount)
{
	int i; 
	for (i = 0; i < 256; i++)            // Initiate with 0
		rCount[i] = gCount[i] = bCount[i] = 0;

	BYTE	*poDIB;

	poDIB = lpBits;
	switch(GetBitsPerPixel()) 
	{   /// numero de bits da imagem : 8
		case  1:
		{
			DWORD ones = 0; // Count "ones"
			BYTE * lpTemp, byte, bit;
			LONG x, y;

			for (y = 0; y < GetHeight(); y++)
			{ // a line
				// SetDone((int) (100*y/GetHeight()));
				lpTemp = poDIB + (bmWidthBytes * y );
				for (x = 0; x < (GetWidth() / 8); x++)
				{ // a byte
					byte = *(lpTemp++);
						for (bit = 0; bit < 8; bit++) // a bit
							if ((byte << bit) & 0x80)
								ones++;
				} // for x
				
				if (GetWidth() % 8) 
				{
					byte = *lpTemp; // others bits
					for (bit = 0; bit < (GetWidth() % 8); bit++) // a bit
						if ((byte << bit) & 0x80)
							ones++;
				}
			} // for y

			DWORD zeros = GetHeight()*GetWidth() - ones;

			GetPaletteEntries(0, 1, &peValue);
			if (!HSI)
			{
				rCount[peValue.peRed  ] = zeros;
				gCount[peValue.peGreen] = zeros;
				bCount[peValue.peBlue ] = zeros;
			}   
			else
			{  
				ConverteHSI(); 
				rCount[H] = zeros;
				gCount[S] = zeros;
				bCount[I] = zeros;
			}   

			GetPaletteEntries(1, 1, &peValue);
			if (!HSI)
			{ 
				rCount[peValue.peRed  ] =  ones;
				gCount[peValue.peGreen] =  ones;
				bCount[peValue.peBlue ] =  ones; 
			}
			else
			{
				ConverteHSI(); 
				rCount[H] =  ones;
				gCount[S] =  ones;
				bCount[I] =  ones; 
			}
			return TRUE;
		} // case  1
		case  4:
		{
			DWORD btCount[16];
			BYTE * lpTemp, byte;
			LONG x, y;

			for (i = 0; i < 16; i++) btCount[i] = 0;

			for (y = 0; y < GetHeight(); y++) 
			{ // a line
				// SetDone((int) (100*y/GetHeight()));
				lpTemp = poDIB + ( bmWidthBytes * y );
				for (x = 0; x < (GetWidth() / 2); x++)
				{ // a byte
					byte = *(lpTemp++);
					btCount[(byte >> 4) & 0x0F]++;
					btCount[(byte) & 0x0F]++;
				} // for x
			
				if (GetWidth() % 2)
				{
					byte = *lpTemp; // others bits
					btCount[ (byte >> 4) & 0x0F ]++;
				}
			} // for y

			for (i = 0; i < 16; i++)
			{
				GetPaletteEntries((WORD)i, 1, &peValue);
				if (!HSI)
				{
					rCount[peValue.peRed  ] += btCount[i];
					gCount[peValue.peGreen] += btCount[i];
					bCount[peValue.peBlue ] += btCount[i];
				}
				else
				{
					ConverteHSI();
					rCount[H] += btCount[i];
					gCount[S] += btCount[i];
					bCount[I] += btCount[i];
				}                  
			}
			return TRUE;
		} // case  4
		case  8:
		{
			DWORD btCount[256];
			BYTE * lpTemp;
			LONG x, y;

			for (i = 0; i < 256; i++)
				btCount[i] = 0;

			for (y = 0; y < GetHeight(); y++)
			{ // a line
				// SetDone((int) (100*y/GetHeight()));
				lpTemp = poDIB + ( bmWidthBytes * y );
				for (x = 0; x < GetWidth(); x++)
					btCount[*(lpTemp++)]++;
			} // for y

			for (i = 0; i < 256; i++)
			{
				GetPaletteEntries((WORD)i, 1, &peValue);
				if (!HSI)
				{
					rCount[peValue.peRed] += btCount[i];
					gCount[peValue.peGreen] += btCount[i];
					bCount[peValue.peBlue ] += btCount[i];
				}
				else
				{
					ConverteHSI();     
					rCount[H] += btCount[i];
					gCount[S] += btCount[i];
					bCount[I] += btCount[i];
				}
			}
			return TRUE;
		} // case  8
		case 24:
		{
			BYTE * lpTemp;
			LONG x, y;

			for (y = 0; y < GetHeight(); y++)
			{ // a line
				// SetDone((int) (100*y/GetHeight()));
				lpTemp = poDIB + ( bmWidthBytes * y );
				for (x = 0; x < GetWidth(); x++)
				{
					if (!HSI)
					{
						peValue.peRed = *(lpTemp++);
						peValue.peGreen = *(lpTemp++);
						peValue.peBlue  = *(lpTemp++);
						bCount[peValue.peRed]++;
						gCount[peValue.peGreen]++;
						rCount[peValue.peBlue]++; 
					}
					else
					{
						peValue.peRed = *(lpTemp++);
						peValue.peGreen = *(lpTemp++);
						peValue.peBlue  = *(lpTemp++);
						ConverteHSI();
						bCount[H]++;
						gCount[S]++;
						rCount[I]++; 
					}
				}  // for x
			} // for y
			return TRUE;
		} // case 24
	} // switch
	return FALSE;
} // CImage::GetRGBHistogram()

HDIB CPDIBase::CopyHandle()
{
	BYTE *     lpCopy;
	BYTE *     lp;
	HDIB           hCopy;
	DWORD          dwLen;

	if (hDIB == NULL)
		return NULL;

	dwLen = ::GlobalSize((HGLOBAL) hDIB);

	if ((hCopy = (HDIB) ::GlobalAlloc (GHND, dwLen)) != NULL)
	{
		lpCopy = (BYTE *) ::GlobalLock((HGLOBAL) hCopy);
		lp     = (BYTE *) ::GlobalLock((HGLOBAL) hDIB);

		while (dwLen--)
			*lpCopy++ = *lp++;

		::GlobalUnlock((HGLOBAL) hCopy);
		::GlobalUnlock((HGLOBAL) hDIB);
	}

	return hCopy;
}

BOOL	CPDIBase::WhiteImage()
{
    LONG	lin,col;
	BYTE*	lpTemp;
	
	lpTemp = lpBits;
	for (lin = 0; lin < GetHeight(); lin++, lpTemp += bmWidthBytes) 
		for (col = 0; col < GetWidth(); col++) 
		{
			SetPixel(col,lin,RGB(255,255,255));
		}

	return TRUE;
}



HDIB CPDIBase::CopyHandleWhite()
{
    BYTE *     lpCopy;
    BYTE *     lp;
    HDIB           hCopy;
    DWORD          dwLen;

    if (hDIB == NULL)
        return NULL;

    dwLen = ::GlobalSize((HGLOBAL) hDIB);

    if ((hCopy = (HDIB) ::GlobalAlloc (GHND, dwLen)) != NULL)
    {
        lpCopy = (BYTE *) ::GlobalLock((HGLOBAL) hCopy);
        lp     = (BYTE *) ::GlobalLock((HGLOBAL) hDIB);

        while (dwLen--)
            *lpCopy++ = 255;

        ::GlobalUnlock((HGLOBAL) hCopy);
        ::GlobalUnlock((HGLOBAL) hDIB);
    }

    return hCopy;
}

// copia uma regiao da imagem
HDIB CPDIBase::CopyRegion()
{
	BYTE *  lpCopy;
	BYTE *  lp;
	HDIB    hCopy;
	DWORD   dwLen; 

	int i, lin, col;
	double tamanho;
	DWORD tamret;                        
           
	if (hDIB == NULL)
	return NULL;

	int h = (int) fabs((double)rect.Height()) ;
	int l = (int) fabs((double)rect.Width());

	tamret = ((DWORD) h * (DWORD) l);

	dwLen = WIDTHBYTES((l) * ((DWORD)lpBI->biBitCount)) * h ;
	dwLen = dwLen + *(LPDWORD)lpBI + (DWORD) GetPaletteSize();

	int div, resto;
	if (dwLen <= 16)
		dwLen = 16;
	else 
	{
		div = dwLen / 16;
		resto = dwLen - (div * 16);
		dwLen = dwLen + (16 - resto);   
	}     

	tamanho = (GetHeight() - 1);

	if ((hCopy = (HDIB) ::GlobalAlloc (GHND, dwLen)) != NULL)
	{
		BYTE * lpBitsCopy;
		lpCopy = (BYTE *) ::GlobalLock((HGLOBAL) hCopy);
		LPBITMAPINFOHEADER	lpBICopy		= (LPBITMAPINFOHEADER)	::GlobalLock((HGLOBAL) hCopy);
		LPBITMAPINFO		lpbmiCopy		= (LPBITMAPINFO)		::GlobalLock((HGLOBAL) hCopy);
		LPBITMAPFILEHEADER lpbmfHdrCopy	= (LPBITMAPFILEHEADER)	::GlobalLock((HGLOBAL) hCopy);
		*(lpBICopy) = *(lpBI); 
		*(lpbmiCopy) = *(lpbmi);
		lpBICopy->biWidth = l;
		lpBICopy->biHeight = h;
		lpBICopy->biSizeImage = tamret;

		lpBitsCopy = (BYTE *) (lpCopy + *(LPDWORD)lpBICopy + GetPaletteSize());

		for (i = 0; i < DIBNumColors(); i++)
		{                
			lpbmiCopy->bmiColors[i].rgbRed = lpbmi->bmiColors[i].rgbRed;
			lpbmiCopy->bmiColors[i].rgbGreen = lpbmi->bmiColors[i].rgbGreen;
			lpbmiCopy->bmiColors[i].rgbBlue = lpbmi->bmiColors[i].rgbBlue;
			lpbmiCopy->bmiColors[i].rgbReserved = lpbmi->bmiColors[i].rgbReserved; 
		}

		lp = lpBits;
		i = 0;
		DWORD  bmWidthBytesCopy;
		int y = 0;
		bmWidthBytesCopy = 4 * ( (l * GetBitsPerPixel() + 31) / 32 );

		CPoint ptLeft, ptRight;
		ptLeft = rect.TopLeft(); 
		ptRight = rect.BottomRight(); 

		ptLeft.y = (long)tamanho - ptLeft.y;
		ptRight.y = (long)tamanho - ptRight.y;

		lp = lp + (ptRight.y * bmWidthBytes);

		for (lin = ptRight.y; lin < ptLeft.y; lin++, lp += bmWidthBytes)  // uma linha
		{
			for (col = ptLeft.x; col < ptRight.x; col++)
			{
				if (i >= (int) l)
				{
					lpBitsCopy += bmWidthBytesCopy;
					i = 0;
				}    
				lpBitsCopy[i] = lp[col];      
				i++;
			}     
		}
		::GlobalUnlock((HGLOBAL) hCopy);
		::GlobalUnlock((HGLOBAL) hDIB);
	}

	return hCopy;
}


// Cria uma paleta cinza
BOOL CPDIBase::GrayPalette()
{
	int x;   
	BYTE cor[16] = { 0,17,34,51,68,85,102,119,136,153,170,187,204,221,238,255};

	if (!IsPaletted())
		return FALSE;

	switch (DIBNumColors()) 
	{
		case   2:
			peValue.peRed = 0;
			peValue.peGreen = 0;
			peValue.peBlue = 0;
			peValue.peFlags = PC_NOCOLLAPSE;   
			SetPaletteEntries(0,1,&peValue);
			peValue.peRed = 255;
			peValue.peGreen = 255;
			peValue.peBlue = 255;
			SetPaletteEntries(1,1,&peValue);
			break;
		case  16:
			for (x = 0; x < 16; x++) 
			{
				peValue.peRed = cor[x];    
				peValue.peGreen = cor[x]; 
				peValue.peBlue = cor[x];
				peValue.peFlags = PC_NOCOLLAPSE;   
				SetPaletteEntries(x,1,&peValue);
				lpbmi->bmiColors[x].rgbRed =  cor[x];
				lpbmi->bmiColors[x].rgbGreen = cor[x];
				lpbmi->bmiColors[x].rgbBlue = cor[x];
			}    
			break;   
		case 256:
			for (x = 0; x < 256; x++)
			{
				peValue.peRed = x;
				peValue.peGreen = x;                 
				peValue.peBlue = x;
				peValue.peFlags = PC_NOCOLLAPSE;   
				SetPaletteEntries(x,1,&peValue);
				lpbmi->bmiColors[x].rgbRed =  x;
				lpbmi->bmiColors[x].rgbGreen = x;
				lpbmi->bmiColors[x].rgbBlue = x;
			}
	}

	return TRUE;  
}


BOOL CPDIBase::Equalizacao()
{
	long int	histo[256], histoequalizado[256];
	int			lin, col;
	int			val,val1,valminimo, cinza;
	double		proba[256],probaniveis[256],probasoma[256],numero,distanciaminima;
	BYTE		*lpTemp;

	numero = (double) (GetWidth() * GetHeight());

	/* calculo dos niveis de cinza em termos de probabilidades */
	/* zera histograma */
	for (val = 0; val < 256; val++)
	{
		histo[val] = 0;   
		probaniveis[val] = (double)val/255;
	}  

	lpTemp = lpBits;
	for (lin = 0; lin < (int) GetHeight(); lin++, lpTemp += bmWidthBytes) 
		for (col = 0; col < (int) GetWidth(); col++) 
			histo[(int)lpTemp[col]]++; 

	/* probabilidades do histograma */
	for (val = 0; val < 256; val++)
		proba[val] = (double)histo[val]/numero;

	/* probabilidades cumulada do histograma */
	probasoma[0] = proba[0] ;
	for (val = 1; val < 256; val++)
		probasoma[val] = probasoma[val-1] + proba[val];

	/* equalizacao do histograma */
	for (val = 0; val < 256; val++)
	{    
		distanciaminima = numero;   /* inicializacao distancia minima */
		valminimo = 256 ;       /* inicializacao valor nivel de cinza correspondente a distancia minima */

		for (val1 = val; val1 < 256; val1++)
		{
			if( fabs( probaniveis[val1] - probasoma[val]) < distanciaminima)         /* val1/255*/
			{
			distanciaminima =  fabs( probaniveis[val1] - probasoma[val]);     /* val1/255*/
			valminimo = val1 ;
			} /* fim do if */       
		}

		histoequalizado[val] = valminimo ; /* Redistribuicao do histograma */
	} /* fim do for */

	GrayPalette(); 
	
	lpTemp = lpBits;  
	for (lin = 0; lin < (int) GetHeight(); lin++, lpTemp += bmWidthBytes)  // uma linha
		for (col = 0; col < (int) GetWidth(); col++)
		{
			cinza = (BYTE) lpTemp[col];
			lpTemp[col] = (BYTE) histoequalizado[cinza];
		}     

	return TRUE;
}

void CPDIBase::LimiarOtsu()
{
	// Aloca as Matrizes
	double proba[256], fOmega[256], fMi[256], fSigmaB[256], fSigmaBMax, fMiTotal;

	long int	sPosicao, histo[256];
	int			lin, col;
	long		k, lTotalPontos;
	int			uiLimiar,  i;
	int			data; 
	BYTE	*lpTemp;

	for (sPosicao = 0; sPosicao < 256; sPosicao++)
		histo[sPosicao] = 0;

	lpTemp = lpBits;

	/* calculo do numero de pontos em funcao da grade */ 
	for (lin = 0; lin < (int) GetHeight(); lin++, lpTemp += bmWidthBytes) 
		for (col = 0; col < (int) GetWidth(); col++) 
			histo[(int)lpTemp[col]]++; 

	// calculo das probabilidades a priori
	lTotalPontos = (long) (GetWidth() * GetHeight());

	for (i = 0; i < 256; i++)
	{
		proba[i] = (double) ((histo[i])/(double)(lTotalPontos));
		fOmega[i] = fMi[i] = 0.0;
	}

	for (k = 0; k < 256; k++)
	{
		for (i = 0; i < k; i++)
			fOmega[k] += proba[i];
	}

	for (k = 0; k < 256; k++)
	{
		for (i = 0; i < k; i++)
			fMi[k] += (i + 1) * proba[i];
	}                    

	fMiTotal = fSigmaBMax = 0.0;  
	for (i = 0; i < 256; i++)
		fMiTotal += (i + 1) * proba[i];

	if ((fOmega[0] * (1 - fOmega[0])) != 0.0)
	{
		fSigmaBMax = (  (fMiTotal * fOmega[0] - fMi[0]) *
						(fMiTotal * fOmega[0] - fMi[0]) ) /
						(fOmega[0] * (1 - fOmega[0]));
		uiLimiar = 0;
	}

	for (k = 1; k < 256; k++)
	{
		if ((fOmega[k] * (1 - fOmega[k])) != 0.0)
		{
			fSigmaB[k] = (  (fMiTotal * fOmega[k] - fMi[k]) *
							(fMiTotal * fOmega[k] - fMi[k]) ) /
							(fOmega[k] * (1 - fOmega[k]));
			if (fSigmaB[k] > fSigmaBMax)
			{
				fSigmaBMax = fSigmaB[k];
				uiLimiar = (int) k;
			}
		}
	}

	lpTemp = lpBits;
	for (lin = 0; lin < (int) GetHeight(); lin++, lpTemp += bmWidthBytes)
		for (col = 0; col < (int) GetWidth(); col++)
		{
			data = lpTemp[col];
			lpTemp[col] = (data < uiLimiar) ? 0 : 255;
		}     

//	char cLimiar[20]; 
//	itoa ((int) uiLimiar, cLimiar, 10);
//	MessageBox(NULL, cLimiar, cLimiar, MB_OK);
}

void CPDIBase::LimiarKittler()
{
	long	histo[256];
	double	p1[256],p2[256],mu1[256],mu2[256],var1[256],var2[256],j[256],minimo;
	long	val,k,kminimo ;    
	int		col,lin;
	int		data;
	BYTE	*lpTemp;

	for (lin = 0; lin < 256; lin++)
		histo[lin] = 0;

	lpTemp = lpBits;

	for (lin = 0; lin < (int) GetHeight(); lin++, lpTemp += bmWidthBytes) 
		for (col = 0; col < (int) GetWidth(); col++) 
			histo[(int)lpTemp[col]]++; 

	for (val = 0; val < 256; val++)
	{
		p1[val]		= 0.0; 
		p2[val]		= 0.0; 
		mu1[val]	= 0.0; 
		mu2[val]	= 0.0; 
		var1[val]	= 0.0; 
		var2[val]	= 0.0; 
		j[val]		= 1.0 ;
	}


	/* calculos de p1[], p2[] , mu1[], mu2[] */
	for (val = 0; val < 256; val++)
	{
		for (k = 0; k <= val ; k++)
			p1[val] += (float) histo[k]; 
		for (k = val+1; k < 256 ; k++)
			p2[val] += (float) histo[k];   

		for (k = 0; k <= val ; k++)
			mu1[val] +=  (float) histo[k]*k ;      
		mu1[val] +=  (p1[val] != 0) ? (float) mu1[val]/p1[val] : 0;

		for (k = val+1; k < 256 ; k++)
			mu2[val] +=  (float) histo[k]*k ;      
		mu2[val] +=  (p2[val] != 0) ? (float) mu2[val]/p2[val] : 0;      
	}

	// calculos de var1[], var2[] 
	for (val = 0; val < 256; val++)
	{     
		for (k = 0; k <= val ; k++)
			var1[val] +=  histo[k]*(k-mu1[val])*(k-mu1[val]) ;      
		var1[val] +=  (p1[val] != 0) ? var1[val]/p1[val] : 0;      

		for (k = val+1; k < 256 ; k++)
			var2[val] += histo[k]*(k-mu2[val])*(k-mu2[val]) ;      
		var2[val] +=  (p2[val] != 0) ? var2[val]/p2[val] : 0;      
	}   


	// calculo de j[]
	for (val = 0; val < 256; val++)
	{
		j[val] +=  (var1[val] != 0) ? log((double) var1[val]) *p1[val] : 0.;      
		j[val] +=  (var2[val] != 0) ? log((double) var2[val]) *p2[val] : 0.;      

		j[val] -=  (p1[val] != 0) ? 2*log((double) p1[val]) *p1[val] : 0.;      
		j[val] -=  (p2[val] != 0) ? 2*log((double) p2[val]) *p2[val] : 0.;      
	}   

	// calculo do limiar kminimo
	val = 1;
	minimo = (float) j[val];     
	kminimo = val ;    
	for (val = 2; val < 256 ; val++)
	{
		if( j[val] < minimo )
		{
			minimo = (float) j[val];
			kminimo = val ;
		}
	}


	lpTemp = lpBits;     
	for (lin = 0; lin < (int) GetHeight(); lin++, lpTemp += bmWidthBytes) 
		for (col = 0; col < (int) GetWidth(); col++)
		{
			data = lpTemp[col];
			lpTemp[col] = (data < kminimo) ? 0 : 255;
		}     
}


void CPDIBase::LimiarEntropia()
{
	int     data;
	long    histo[256];
	int     lin, col;
	long    t,i,j,limiar,soma1, soma2, soma3, soma4, total1, total2;
	double  min;

	HGLOBAL hni, hni1, hni2, hmi1, hmi2;
	double FAR *ni, FAR *ni1, FAR *mi1, FAR *mi2;
	double FAR* ni2;
	hni = GlobalAlloc(GPTR, 256*sizeof(double));
	ni = (double FAR*) GlobalLock(hni);

	hni1 = GlobalAlloc(GPTR, 256*sizeof(double));
	ni1 = (double FAR *) GlobalLock(hni1);

	hni2 = GlobalAlloc(GPTR, 256*sizeof(long double));
	ni2 = (double FAR *) GlobalLock(hni2);

	hmi1 = GlobalAlloc(GPTR, 256*sizeof(double));
	mi1 = (double FAR *) GlobalLock(hmi1);

	hmi2 = GlobalAlloc(GPTR, 256*sizeof(double));
	mi2 = (double FAR *) GlobalLock(hmi2);

	BYTE * lpTemp = lpBits;
	lpTemp = lpBits;

	for (i = 0; i < 256; i++)
		histo[i] = 0;


	for (lin = 0; lin < (int) GetHeight(); lin++, lpTemp += bmWidthBytes) 
		for (col = 0; col < (int) GetWidth(); col++) 
			histo[(int)lpTemp[col]]++; 

	mi1[1] = 0.;
	ni1[1] = 0.;
	ni2[1] = 0.;
	total1 = 0;
	total2 = 0;
	soma1 = 0;
	soma2 = 0;
	soma3 = 0;
	soma4 = 0; 

	for (j = 1; j < 256; j++)      /* nao pode iniciar com 0 porque log(0) nao existe */
	{
		total1 += j * histo[j]; 
		total2 += histo[j];
	}
	mi2[1] =  (total2 != 0) ? ((double) (total1)) / total2 : 0.;

	if (mi2[1] != 0)
	{
		for (j = 1; j < 256; j++)
		ni2[1] += (double) j * (double) histo[j] * (double) log(j/mi2[1]);

		ni[1] = ni2[1];

		for (t = 2; t < 256; t++)
		{

			soma1 = soma1 + (t-1) * histo[t-1];
			soma2 = soma2 + histo [t-1];
			mi1[t] = (soma2 != 0) ? ((double) (soma1))/soma2 : 0.;

			soma3 = total1 - soma1;
			soma4 = total2 - soma2;
			mi2[t] = (soma4 != 0) ? ((double) (soma3)) / soma4 : 0.;

			ni1[t] = 0.;
			for (j = 1; j < t; j++)
				ni1[t] += (mi1[t] != 0) ? (j * histo[j] * log(j/mi1[t])) : 0.; 

			ni2[t] = 0.;
			for (j = t; j < 256; j++)
				ni2[t] += (mi2[t] != 0) ? (j * histo[j] * log(j/mi2[t])) : 0.;

			ni[t] = ni1[t] + ni2[t];        
		}       

		/* busca do minimo = limiar */
		t = 1;
		min = ni[t];
		limiar = t;
		for (t = 2; t < 256; t++)
		{
			if (ni[t] < min)
			{
				min = ni[t];
				limiar = t;
			}    
		}   

		lpTemp = lpBits;
		for (lin = 0; lin < (int) GetHeight(); lin++, lpTemp += bmWidthBytes) 
			for (col = 0; col < (int) GetWidth(); col++)  
			{
				data = lpTemp[col];
				lpTemp[col] = (data < limiar) ? 0 : 255;
			}     
	}

	GlobalUnlock(hni);
	GlobalFree(hni);

	GlobalUnlock(hni1);
	GlobalFree(hni1);

	GlobalUnlock(hni2);
	GlobalFree(hni2);

	GlobalUnlock(hmi1);
	GlobalFree(hmi1);

	GlobalUnlock(hmi2);
	GlobalFree(hmi2);
}

void CPDIBase::LimiarAnisotropia()
{
	long     histo[256];
	double	p[256], sum1, sum2,alfa, numero;
	long     val,k,kanisotropy,mminimo ;       
	int      col,lin;
	int      data;
	BYTE	*lpTemp;

	numero = (GetWidth() * GetHeight());

	for (lin = 0; lin < 256; lin++)
		histo[lin] = 0;

	
	lpTemp = lpBits;

	for (lin = 0; lin < (int) GetHeight(); lin++, lpTemp += bmWidthBytes) 
		for (col = 0; col < (int) GetWidth(); col++) 
			histo[(int)lpTemp[col]]++; 

	sum1 = 0.0;
	sum2 = 0.0;
	/* calculo das probabilidades a priori */
	for (val = 0; val < 256; val++)
		p[val] = ( histo[val]/numero); 

	/* busca do m minimo, mminimo */
	val = 1; 
	sum1 = p[val] ; 
	mminimo = val ;     
	for (val = 1; val < 256; val++)
	{
		sum1 += p[val] ; 
		if( sum1 >= 0.5)    
		{
			mminimo = val ;
			break ;
		}
	}


	/* calculo do alfa */
	sum1 = 0.0;      
	for (k = 0; k <= mminimo ; k++)       
		sum1 +=  (p[k] != 0.) ? (p[k]* log((double) p[k]) ) : 0. ;

	sum2 = sum1;      
	for (k = mminimo+1 ; k < 256 ; k++)    
		sum2 +=  (p[k] != 0.) ? (p[k]* log((double) p[k]) ) : 0. ; 

	alfa =  (sum2 != 0.) ? sum1/sum2 : 0. ;

	/* calculo do limiar kanisotropy */
	val = 1; 
	sum1 =  p[val] ; 
	kanisotropy = val ;     
	if (alfa <= 0.5)
	{
		sum1 = 0.0;      
		for (val = 0; val < 256; val++)
		{
			sum1 += p[val]; 
			if( sum1 >= 1 - alfa )  
			{
				kanisotropy = val ;
				break ;
			}
		}      
	}
	else
	{
		sum1 = (float) 0.0;      
		for (val = 0; val < 256; val++)
		{
			sum1 +=  (float) p[val]; 
			if( sum1 >= alfa )  
			{
				kanisotropy = val ;
				break ;
			}
		}  
	}

	lpTemp = lpBits;
	for (lin = 0; lin < (int) GetHeight(); lin++, lpTemp += bmWidthBytes)
		for (col = 0; col < (int) GetWidth(); col++)
		{
			data = lpTemp[col];
			lpTemp[col] = (data < kanisotropy) ? 0 : 255;
		}
}


void CPDIBase::LimiarManual(int Limiar)
{
	int		col,lin;
	int		data;
	BYTE	*lpTemp;

	lpTemp = lpBits;
	for (lin = 0; lin < (int) GetHeight(); lin++, lpTemp += bmWidthBytes)
	{
		for (col = 0; col < (int) GetWidth(); col++)
		{
			data = lpTemp[col];
			lpTemp[col] = (data < Limiar) ? 0 : 255;
		}
	}
}


void CPDIBase::InverteImagem()
{
	int		col,lin;   
	BYTE	*lpTemp;

	lpTemp = lpBits;
	for (lin = 0; lin < (int) GetHeight(); lin++, lpTemp += bmWidthBytes)
	{
		for (col = 0; col < (int) GetWidth(); col++) 
			lpTemp[col] = 255 - lpTemp[col]; 
	}
}

void CPDIBase::TonsCinza(){
}

void CPDIBase::TonsCinzaNTSC(){
}

/*******************************************************************************************************************
*EqualizaHistogramaYIQ - Equaliza histograma para espaço de cor YIQ                                                *
*Parâmetros: canal a ser equalizado                                                                                *
*Retorno:                                                                                                          *
*Criação: Jean Felipe Felsky                                     Data: 06/04/2010                                  *
*Última alteração: Jean Felipe Felsky                            Data: 14/04/2010                                  *
*******************************************************************************************************************/
void CPDIBase::EqualizaHistogramaYIQ(char canal, double *matrizY)
{


	long int	histo[256], histoequalizado[256];
	int			lin, col;
	int			val,val1,valminimo;
	double		proba[256],probaniveis[256],probasoma[256],numero,distanciaminima;
	BYTE		*lpTemp;
	BYTE		*buffer_Temp;
	int 		y, fator_canal;
	double		Canal_Y= 0, Canal_I=0, Canal_Q=0;  
	int			inteiroy;
	double		Canal_b, Canal_g, Canal_r;

	numero = (double) (GetWidth() * GetHeight());

	lpTemp = lpBits;

	/* Define qual canal vai ser equalizado */
	fator_canal = 0;
	switch (canal)
	{
		case 'Y': 
			fator_canal = 0;
			break;
		case 'I':
			fator_canal = 1;
			break;
		case 'Q':
			fator_canal = 2;
			break;
	}

	/* calculo dos niveis de cinza em termos de probabilidades */
	/* zera histograma */
	for (val = 0; val < 256; val++)
	{
		histo[val] = 0;   
		probaniveis[val] = (double)val/255;
	}  


	/*conta cada um dos níveis */
	for (lin = 0; lin < (int) GetHeight(); lin++) 
	{
		for (col = 0; col < (int) (GetWidth()*3); col=col+3) 
		{
			inteiroy =  (int) (lpTemp[(lin* bmWidthBytes) + col+fator_canal]);
			histo[inteiroy] = histo[inteiroy] + 1;
		}
	}

	/* probabilidades do histograma */
	for (val = 0; val < 256; val++)
		proba[val] = (double)histo[val]/numero;

	/* probabilidades cumulada do histograma */
	probasoma[0] = proba[0] ;
	for (val = 1; val < 256; val++)
		probasoma[val] = probasoma[val-1] + proba[val];

	/* equalizacao do histograma */
	for (val = 0; val < 256; val++)
	{    
		distanciaminima = numero;   /* inicializacao distancia minima */
		valminimo = 256 ;       /* inicializacao valor nivel de cinza correspondente a distancia minima */

		for (val1 = val; val1 < 256; val1++)
		{
			if( fabs( probaniveis[val1] - probasoma[val]) < distanciaminima)         /* val1/255*/
			{
				distanciaminima =  fabs( probaniveis[val1] - probasoma[val]);     /* val1/255*/
				valminimo = val1 ;
			} 
		}

		histoequalizado[val] = valminimo ; 
	} 

	/* Redistribuicao do histograma */
	inteiroy = 0;
	for (lin = 0; lin < (int) GetHeight(); lin++) {
		for (col = 0; col < (int) (GetWidth()*3); col=col+3) {
			y  = (int) lpTemp[(lin* bmWidthBytes) + col+fator_canal];
			lpTemp[(lin* bmWidthBytes) + col + fator_canal] = (BYTE) histoequalizado[y];
			matrizY[inteiroy] = histoequalizado[y];
			inteiroy++;
		}     
	}

	lpBits = lpTemp;
	
	
}


void CPDIBase::EqualizaHistogramaYIQmatriz(char canal, double *matrizY)
{


	long int	histo[256], histoequalizado[256];
	int			lin, col;
	int			val,val1,valminimo;
	double		proba[256],probaniveis[256],probasoma[256],numero,distanciaminima;
	BYTE		*lpTemp;
	BYTE		*buffer_Temp;
	int 		y, fator_canal;
	double		Canal_Y= 0, Canal_I=0, Canal_Q=0;  
	int			inteiroy;
	double		Canal_b, Canal_g, Canal_r;
	int i = 0;

	numero = (double) (GetWidth() * GetHeight());

	/* Define qual canal vai ser equalizado */
	fator_canal = 0;
	switch (canal)
	{
		case 'Y': 
			fator_canal = 0;
			break;
		case 'I':
			fator_canal = 1;
			break;
		case 'Q':
			fator_canal = 2;
			break;
	}

	/* calculo dos niveis de cinza em termos de probabilidades */
	/* zera histograma */
	for (val = 0; val < 256; val++)
	{
		histo[val] = 0;   
		probaniveis[val] = (double)val/255;
	}  


	/*conta cada um dos níveis */
	i = 0;
	for (lin = 0; lin < (int) GetHeight(); lin++) 
	{
		for (col = 0; col < (int) (GetWidth()*3); col=col+3) 
		{
			inteiroy =  (int) (matrizY[i]);
			histo[inteiroy] = histo[inteiroy] + 1;
			i++;
		}
	}

	/* probabilidades do histograma */
	for (val = 0; val < 256; val++)
		proba[val] = (double)histo[val]/numero;

	/* probabilidades cumulada do histograma */
	probasoma[0] = proba[0] ;
	for (val = 1; val < 256; val++)
		probasoma[val] = probasoma[val-1] + proba[val];

	/* equalizacao do histograma */
	for (val = 0; val < 256; val++)
	{    
		distanciaminima = numero;   /* inicializacao distancia minima */
		valminimo = 256 ;       /* inicializacao valor nivel de cinza correspondente a distancia minima */

		for (val1 = val; val1 < 256; val1++)
		{
			if( fabs( probaniveis[val1] - probasoma[val]) < distanciaminima)         /* val1/255*/
			{
				distanciaminima =  fabs( probaniveis[val1] - probasoma[val]);     /* val1/255*/
				valminimo = val1 ;
			} 
		}

		histoequalizado[val] = valminimo ; 
	} 

	/* Redistribuicao do histograma */
	inteiroy = 0;
	i = 0;
	for (lin = 0; lin < (int) GetHeight(); lin++) {
		for (col = 0; col < (int) (GetWidth()*3); col=col+3) {
			y  = (int) matrizY[i];
			matrizY[i] = histoequalizado[y];
			i++;
		}     
	}
	
	
}


/*******************************************************************************************************************
*converteI - Converte os valores do canal I de -152 a +152 para de 0 a 255                                         *
*Parâmetros: valor I, no range [-152, 152]                                                                         *
*Retorno: valor I, no range [0, 255]                                                                               *
*Criação: Jean Felipe Felsky                                     Data: 13/04/2010                                  *
*Última alteração: Jean Felipe Felsky                            Data: 13/04/2010                                  *
*******************************************************************************************************************/
BYTE CPDIBase::converteI(double I)
{
	I += 152;
	I *= 0.8389;
	return (BYTE) I;
}

/*******************************************************************************************************************
*converteQ - Converte os valores do canal I de -134 a +134 para de 0 a 255                                         *
*Parâmetros: valor Q, no range [-134, 134]                                                                         *
*Retorno: valor Q, no range [0, 255]                                                                               *
*Criação: Jean Felipe Felsky                                     Data: 13/04/2010                                  *
*Última alteração: Jean Felipe Felsky                            Data: 13/04/2010                                  *
*******************************************************************************************************************/
BYTE CPDIBase::converteQ(double Q)
{
	Q += 134;
	Q *= 0.9515;
	return (BYTE) Q;
}

/*******************************************************************************************************************
*desconverteI - Converte os valores do canal I de 0 a 255 para de -152 a +152                                      *
*Parâmetros: valor I, no range [0, 255]                                                                            *
*Retorno: valor I, no range [-152, +152]                                                                           *
*Criação: Jean Felipe Felsky                                     Data: 13/04/2010                                  *
*Última alteração: Jean Felipe Felsky                            Data: 13/04/2010                                  *
*******************************************************************************************************************/
double CPDIBase::desconverteI(BYTE I)
{
	double retornoI;
	retornoI = (double) I*1.1921;
	retornoI -= 152;
	return retornoI;
}

/*******************************************************************************************************************
*desconverteQ - Converte os valores do canal Q de 0 a 255 para de -134 a +134                                      *
*Parâmetros: valor Q, no range [0, 255]                                                                            *
*Retorno: valor Q, no range [-134, +134]                                                                           *
*Criação: Jean Felipe Felsky                                     Data: 13/04/2010                                  *
*Última alteração: Jean Felipe Felsky                            Data: 13/04/2010                                  *
*******************************************************************************************************************/
double CPDIBase::desconverteQ(BYTE Q)
{
	double retornoQ;
	retornoQ = (double) Q*1.0509;
	retornoQ -= 134;
	return retornoQ;
}

/*******************************************************************************************************************
*normalizaRGB - Converte os valores de um canal RGB, vindo no range [MaiorRGB, MenorRGB] para [0, 255]             *
*Parâmetros: RGB, valor a ser convertido, limites do range, MaiorRGB e MenorRGB                                    *
*Retorno: valor RGB no range [0, 255]                                                                              *
*Criação: Jean Felipe Felsky                                     Data: 13/04/2010                                  *
*Última alteração: Jean Felipe Felsky                            Data: 13/04/2010                                  *
*******************************************************************************************************************/
BYTE CPDIBase::normalizaRGB(double RGB, double MaiorRGB, double MenorRGB)
{
	RGB -= MenorRGB;
	RGB *= 255/(MaiorRGB - MenorRGB);
	return (BYTE) RGB;

}

double CPDIBase::normaliza01(double RGB, double MaiorRGB, double MenorRGB)
{
	RGB -= MenorRGB;
	RGB *= 1/(MaiorRGB - MenorRGB);
	return RGB;

}

void CPDIBase::AprimoraNitidezYIQ(double *matrizY) 
{

	int			lin, col;
	double		*buffer_y_mat, *buffer_media_y_mat;
	double		*matTempPB, *matTempPA;
	double		*buffer_Temp;
	double 		v1, media;
	double 		alfa = 0.9, beta = 1.1;

	matTempPB = (double*) malloc((int)GetHeight() * (int)GetWidth()*3* sizeof(double));
	matTempPA = (double*) malloc((int)GetHeight() * (int)GetWidth()*3* sizeof(double));
	
	buffer_y_mat = (double*) malloc((int)GetHeight() * (int)GetWidth()*3* sizeof(double));
	buffer_media_y_mat = (double*) malloc((int)GetHeight() * (int)GetWidth()*3* sizeof(double));	

	
	int i, j;

	i = 0;

	//log(1 + f(n1,n2)
	for (lin = 0; lin < (int) GetHeight(); lin++) 
	{
		for (col = 0; col < (int) (GetWidth()*3); col=col+3) 
		{
			matrizY[i] = log(1 + matrizY[i]);

			i++;
		}
	}
	

	//Filtro passa baixa
	// procedimentos para montagem do buffer com os valores do canal y para aplicar o filtro da média
	
	i = j = 0;
	for (lin = 0; lin < (int) GetHeight(); lin++) 
	{
		i = 0;
		for (col = 0; col < (int) (GetWidth()*3); col=col + 3) 
		{
			buffer_y_mat[(lin*GetWidth())+ i] = matrizY[j];
			buffer_media_y_mat[(lin* GetWidth())+ i] = matrizY[j];
			i++;
			j++;
		}
	} 

	// procedimentos para o cálculo do filtro da média 
	i = 0;
	for (lin=1; lin < ((int) GetHeight()-1); lin++)
	{
		for (col=1; col < ( (int) GetWidth()-1); col++)
		{

			v1 = buffer_y_mat[(GetWidth() * (lin-1))+(col-1)];
			v1 += buffer_y_mat[(GetWidth() * (lin-1))+ col];
			v1 += buffer_y_mat[(GetWidth() * (lin-1))+(col+1)];
			v1 += buffer_y_mat[(GetWidth() * lin)+(col-1)];
			v1 += buffer_y_mat[(GetWidth() * lin)+(col)];
			v1 += buffer_y_mat[(GetWidth() * lin)+(col+1)];
			v1 += buffer_y_mat[(GetWidth() * (lin+1))+(col-1)];
			v1 += buffer_y_mat[(GetWidth() * (lin+1))+col];
			v1 += buffer_y_mat[(GetWidth() * (lin+1))+(col+1)];
			media = v1/9;

			buffer_media_y_mat[(GetWidth() * lin) + col] = media;
		}
	}

	i = 0;
	
	//Filtro passa baixa * alfa
	// procedimentos para retorno ao buffer_temp do canal Y com o filtro da média 
	for (lin=0; lin < ((int) GetHeight()); lin++)
	{
		j = 0;
		for (col=0; col < ( (int) GetWidth()*3); col = col+3)
		{	
			matTempPB[i]= buffer_media_y_mat[(lin* GetWidth())+ j] * alfa;
			j++;
			i++;
		}
	}

	
	j = 0;
	//Filtro passa alta
	for (lin = 0; lin < (int) GetHeight(); lin++) 
	{
		
		i = 0;
		for (col = 0; col < (int) (GetWidth()*3); col=col+3) 
		{
			buffer_y_mat[(lin*GetWidth())+ i] = matrizY[j];
			buffer_media_y_mat[(lin* GetWidth())+ i] = matrizY[j];
			i++;
			j++;
		}
	} 

	// procedimentos para o cálculo do filtro passa alta

	for (lin=1; lin < ((int) GetHeight()-1); lin++)
	{
		for (col=1; col < ( (int) GetWidth()-1); col++)
		{

			v1 = buffer_y_mat[(GetWidth() * (lin-1))+(col-1)] * -1;
			v1 += buffer_y_mat[(GetWidth() * (lin-1))+ col] * -1;
			v1 += buffer_y_mat[(GetWidth() * (lin-1))+(col+1)] * -1;
			v1 += buffer_y_mat[(GetWidth() * lin)+(col-1)] * -1;
			v1 += buffer_y_mat[(GetWidth() * lin)+(col)] * 9;
			v1 += buffer_y_mat[(GetWidth() * lin)+(col+1)] * -1;
			v1 += buffer_y_mat[(GetWidth() * (lin+1))+(col-1)] * -1;
			v1 += buffer_y_mat[(GetWidth() * (lin+1))+col] * -1;
			v1 += buffer_y_mat[(GetWidth() * (lin+1))+(col+1)] * -1;
			//media = v1/9;

			buffer_media_y_mat[(GetWidth() * lin) + col] = v1;
		}
	}

	i = 0;
	
	//Filtro passa alta * beta
	// procedimentos para retorno ao buffer_temp do canal Y com o filtro passa alta
	for (lin=0; lin < ((int) GetHeight()); lin++)
	{
		j = 0;
		for (col=0; col < ( (int) GetWidth()*3); col = col+3)
		{	
			matTempPA[i]= buffer_media_y_mat[(lin* GetWidth())+ j] * beta;
			j++;
			i++;

		}
	}
	

	//Soma
	i = 0;
	for (lin=0; lin < ((int) GetHeight()); lin++)
	{
		for (col=0; col < ( (int) GetWidth()*3); col = col+3)
		{	
			matrizY[i] = matTempPA[i] + matTempPB[i];
			i++;
		}
	}

	
	//Normaliza entre 0 e 1
	double Maior_mat, Menor_mat;
	Maior_mat = -9999;
	Menor_mat = 9999;

	

	i = 0;
	for (lin=0; lin < ((int) GetHeight()); lin++)
	{
		for (col=0; col < ( (int) GetWidth()*3); col = col+3)
		{
			if ( matrizY[i] > Maior_mat )
				Maior_mat = matrizY[i];
			if ( matrizY[i] < Menor_mat )
				Menor_mat = matrizY[i];

			i++;
		}
	}

	i = 0;
	for (lin=0; lin < ((int) GetHeight()); lin++)
	{
		for (col=0; col < ( (int) GetWidth()*3); col = col+3)
		{
			matrizY[i] = normaliza01(matrizY[i], Maior_mat, Menor_mat);
			i++;
		}
	}

	i = 0;
	//Exponencial
	for (lin=0; lin < ((int) GetHeight()); lin++)
	{
		for (col=0; col < ( (int) GetWidth()*3); col = col+3)
		{		
			matrizY[i] = pow((double)M_E, (double)matrizY[i]);
			i++;
		}
	}
	
	//Normaliza entre 0 e 255
	i = 0;
	for (lin=0; lin < ((int) GetHeight()); lin++)
	{
		for (col=0; col < ( (int) GetWidth()*3); col=col+3)
		{
			matrizY[i] = normalizaRGB(matrizY[i], M_E, 1);
			i++;
		}
	} 
	
}



void CPDIBase::AprimoraNitidezYCrCbCalculo1(double P1)
{
double		Canal_Y= 0, Canal_Cr=0, Canal_Cb=0, media, v1, v2, v3, v4, v5, v6, v7, v8;  
long int	cont_conv, numero, altura, largura;
long int	Canal_b, Canal_g, Canal_r; 
BYTE		*lpTemp;
float		delta;
double		*buffer_temp, *buffer_y, *buffer_media_y, *imagem1, *imagem2, *imagem3, *imagem4;
long int	i=0, j, col, lin; 

numero = (int) (GetWidth() * GetHeight());
buffer_temp = new double[numero*3];
buffer_y = new double[numero];
buffer_media_y = new double[numero];
imagem1 = new double[numero];
imagem2 = new double[numero];
imagem3 = new double[numero];
imagem4 = new double[numero];
lpTemp = lpBits;
altura = GetHeight();
largura = GetWidth();

// inicio dos procedimentos para conversão de YCrCb para RGB

for (cont_conv=0;  cont_conv < (numero*3); cont_conv = cont_conv+3)
	{
		Canal_b = (int) lpTemp[cont_conv];
		Canal_g = (int) lpTemp[cont_conv+1];
		Canal_r = (int) lpTemp[cont_conv+2];
		Canal_Y = (0.257 * Canal_r) + (0.504 * Canal_g) + (0.098 * Canal_b) + 16;
		Canal_Cr = (0.439 * Canal_r) - (0.368 * Canal_g) - (0.071 * Canal_b) + 128;
		Canal_Cb = -(0.148 * Canal_r) - (0.291 * Canal_g) + (0.439 * Canal_b) + 128;
		buffer_temp[cont_conv]= Canal_Cb;
		buffer_temp[cont_conv+1]= Canal_Cr;
		buffer_temp[cont_conv+2]= Canal_Y;
		lpTemp[cont_conv] = (BYTE) Canal_Cb;
		lpTemp[cont_conv+1] = (BYTE) Canal_Cr;
		lpTemp[cont_conv+2] = (BYTE) Canal_Y;
	}
// final dos procedimentos de conversão


// procedimentos para montagem do buffer com os valores do canal y para aplicar o filtro da média
for (j=2; j<(numero*3); j=j+3)
{buffer_y[i] = buffer_temp[j];
 buffer_media_y[i] = buffer_temp[j];
 i++;
}

// procedimentos para o cálculo do filtro da média

for (lin=1; lin<(altura-1); lin++)
	{
		for (col=1; col<(largura-1); col++)
		{
			v1 = buffer_y [(largura * (lin-1))+(col-1)];
			v2 = buffer_y [(largura * (lin-1))+ col];
			v3 = buffer_y [(largura * (lin-1))+(col+1)];
			v4 = buffer_y [(largura * lin)+(col-1)];
			v5 = buffer_y [(largura * lin)+(col+1)];
			v6 = buffer_y [(largura * (lin+1))+(col-1)];
			v7 = buffer_y [(largura * (lin+1))+col];
			v8 = buffer_y [(largura * (lin+1))+(col+1)];
			media = (v1+v2+v3+v4+v5+v6+v7+v8)/8;
			buffer_media_y[(largura * lin) + col] = media;
		}
	}


// formação do Delta D - função de informação espacial

for (j=0; j < numero; j++)
{imagem1[j] = buffer_y[j] - buffer_media_y[j];}

delta = P1;
delta = (-1)*delta;

// formação da imagem com a qualidade aprimorada

for (j=0; j < numero; j++)
{imagem2[j] = (buffer_y[j] - imagem1[j] * delta);}

// procedimentos para retorno ao buffer temp do canal Y modificado
i=0;
for (j=2; j<(numero*3); j=j+3)
{buffer_temp[j]= imagem2[i];
 i++;
}


// procedimentos para conversão do formato YCrCB para o formato RGB

for (cont_conv = 0; cont_conv < numero *3; cont_conv= cont_conv+3)
	{
		Canal_Y  = buffer_temp[cont_conv+2];
		if (Canal_Y > 240) {Canal_Y=240;} 
		if (Canal_Y < 16) {Canal_Y=16;}
		Canal_Cr = buffer_temp[cont_conv+1];
		Canal_Cb = buffer_temp[cont_conv];
		Canal_b = (1.164*(Canal_Y - 16)) + (2.018*(Canal_Cb - 128));
		Canal_g = (1.164*(Canal_Y - 16)) - (0.813*(Canal_Cr - 128)) - (0.391*(Canal_Cb - 128));
		Canal_r = (1.164*(Canal_Y - 16)) + (1.596*(Canal_Cr - 128));
		if (Canal_b > 255) {Canal_b=255;} 
		if (Canal_b < 0) {Canal_b=0;}
		if (Canal_g > 255) {Canal_g=255;} 
		if (Canal_g < 0) {Canal_g=0;}
		if (Canal_r > 255) {Canal_r=255;} 
		if (Canal_r < 0) {Canal_r=0;}
		lpTemp[cont_conv] = (BYTE) Canal_b;
		lpTemp[cont_conv+1] = (BYTE) Canal_g;
		lpTemp[cont_conv+2] = (BYTE) Canal_r;

	}
// fim dos procedimentos para conversão do formato YCrCb para RGB

//	return TRUE;
}

void CPDIBase::AprimoraNitidezYCrCbCalculo2(double P2)
{
double		Canal_Y= 0, Canal_Cr=0, Canal_Cb=0, media, v1, v2, v3, v4, v5, v6, v7, v8;  
long int	cont_conv, numero, altura, largura;
long int	Canal_b, Canal_g, Canal_r; 
BYTE		*lpTemp;
float		delta;
double		*buffer_temp, *buffer_y, *buffer_media_y, *imagem1, *imagem2, *imagem3, *imagem4;
long int	i=0, j, col, lin; 

numero = (int) (GetWidth() * GetHeight());
buffer_temp = new double[numero*3];
buffer_y = new double[numero];
buffer_media_y = new double[numero];
imagem1 = new double[numero];
imagem2 = new double[numero];
imagem3 = new double[numero];
imagem4 = new double[numero];
lpTemp = lpBits;
altura = GetHeight();
largura = GetWidth();


// inicio dos procedimentos para conversão de YCrCb para RGB

for (cont_conv=0;  cont_conv < (numero*3); cont_conv = cont_conv+3)
	{
		Canal_b = (int) lpTemp[cont_conv];
		Canal_g = (int) lpTemp[cont_conv+1];
		Canal_r = (int) lpTemp[cont_conv+2];
		Canal_Y = (0.257 * Canal_r) + (0.504 * Canal_g) + (0.098 * Canal_b) + 16;
		Canal_Cr = (0.439 * Canal_r) - (0.368 * Canal_g) - (0.071 * Canal_b) + 128;
		Canal_Cb = -(0.148 * Canal_r) - (0.291 * Canal_g) + (0.439 * Canal_b) + 128;
		buffer_temp[cont_conv]= Canal_Cb;
		buffer_temp[cont_conv+1]= Canal_Cr;
		buffer_temp[cont_conv+2]= Canal_Y;
		lpTemp[cont_conv] = (BYTE) Canal_Cb;
		lpTemp[cont_conv+1] = (BYTE) Canal_Cr;
		lpTemp[cont_conv+2] = (BYTE) Canal_Y;
	}
// final dos procedimentos de conversão


// procedimentos para montagem do buffer com os valores do canal y para aplicar o filtro da média
for (j=2; j<(numero*3); j=j+3)
{buffer_y[i] = buffer_temp[j];
 buffer_media_y[i] = buffer_temp[j];
 i++;
}

// procedimentos para o cálculo do filtro da média

for (lin=1; lin<(altura-1); lin++)
	{
		for (col=1; col<(largura-1); col++)
		{
			v1 = buffer_y [(largura * (lin-1))+(col-1)];
			v2 = buffer_y [(largura * (lin-1))+ col];
			v3 = buffer_y [(largura * (lin-1))+(col+1)];
			v4 = buffer_y [(largura * lin)+(col-1)];
			v5 = buffer_y [(largura * lin)+(col+1)];
			v6 = buffer_y [(largura * (lin+1))+(col-1)];
			v7 = buffer_y [(largura * (lin+1))+col];
			v8 = buffer_y [(largura * (lin+1))+(col+1)];
			media = (v1+v2+v3+v4+v5+v6+v7+v8)/8;
			buffer_media_y[(largura * lin) + col] = media;
		}
	}

// formação do Delta D que corresponde a função de informação espacial

for (j=0; j < numero; j++)
{imagem1[j] = buffer_y[j] - buffer_media_y[j];}

delta = P2;

// formação do módulo da função de informação espacial
for (j=0; j<numero; j++)
{	if (imagem1[j]<0) 
		{imagem2[j]=imagem1[j]*(-1);}
	else 
		{imagem2[j]=imagem1[j];}
}

// formação da imagem final

for (j=0; j < numero; j++)
{imagem3[j] = ( buffer_y[j] + (delta * imagem2[j]));}


// procedimentos para retorno ao buffer temp do canal Y modificado
i=0;
for (j=2; j<(numero*3); j=j+3)
{buffer_temp[j]= imagem3[i];
 i++;
}

// procedimentos para conversão do formato YCrCB para o formato RGB

for (cont_conv = 0; cont_conv < numero *3; cont_conv= cont_conv+3)
	{
		Canal_Y  = buffer_temp[cont_conv+2];
		if (Canal_Y > 240) {Canal_Y=240;} 
		if (Canal_Y < 16) {Canal_Y=16;}
		Canal_Cr = buffer_temp[cont_conv+1];
		Canal_Cb = buffer_temp[cont_conv];
		Canal_b = (1.164*(Canal_Y - 16)) + (2.018*(Canal_Cb - 128));
		Canal_g = (1.164*(Canal_Y - 16)) - (0.813*(Canal_Cr - 128)) - (0.391*(Canal_Cb - 128));
		Canal_r = (1.164*(Canal_Y - 16)) + (1.596*(Canal_Cr - 128));
		if (Canal_b > 255) {Canal_b=255;} 
		if (Canal_b < 0) {Canal_b=0;}
		if (Canal_g > 255) {Canal_g=255;} 
		if (Canal_g < 0) {Canal_g=0;}
		if (Canal_r > 255) {Canal_r=255;} 
		if (Canal_r < 0) {Canal_r=0;}
		lpTemp[cont_conv] = (BYTE) Canal_b;
		lpTemp[cont_conv+1] = (BYTE) Canal_g;
		lpTemp[cont_conv+2] = (BYTE) Canal_r;

	}
// fim dos procedimentos para conversão do formato YCrCb para RGB


}

void CPDIBase::AprimoraNitidezYCrCbCalculo3(double P3)
{
double		Canal_Y= 0, Canal_Cr=0, Canal_Cb=0, media, v1, v2, v3, v4, v5, v6, v7, v8;  
long int	cont_conv, numero, altura, largura;
long int	Canal_b, Canal_g, Canal_r; 
BYTE		*lpTemp;
float		delta;
double		*buffer_temp, *buffer_y, *buffer_media_y, *imagem1, *imagem2, *imagem3, *imagem4;
long int	i=0, j, col, lin; 

numero = (int) (GetWidth() * GetHeight());
buffer_temp = new double[numero*3];
buffer_y = new double[numero];
buffer_media_y = new double[numero];
imagem1 = new double[numero];
imagem2 = new double[numero];
imagem3 = new double[numero];
imagem4 = new double[numero];
lpTemp = lpBits;
altura = GetHeight();
largura = GetWidth();


// inicio dos procedimentos para conversão de YCrCb para RGB

for (cont_conv=0;  cont_conv < (numero*3); cont_conv = cont_conv+3)
	{
		Canal_b = (int) lpTemp[cont_conv];
		Canal_g = (int) lpTemp[cont_conv+1];
		Canal_r = (int) lpTemp[cont_conv+2];
		Canal_Y = (0.257 * Canal_r) + (0.504 * Canal_g) + (0.098 * Canal_b) + 16;
		Canal_Cr = (0.439 * Canal_r) - (0.368 * Canal_g) - (0.071 * Canal_b) + 128;
		Canal_Cb = -(0.148 * Canal_r) - (0.291 * Canal_g) + (0.439 * Canal_b) + 128;
		buffer_temp[cont_conv]= Canal_Cb;
		buffer_temp[cont_conv+1]= Canal_Cr;
		buffer_temp[cont_conv+2]= Canal_Y;
		lpTemp[cont_conv] = (BYTE) Canal_Cb;
		lpTemp[cont_conv+1] = (BYTE) Canal_Cr;
		lpTemp[cont_conv+2] = (BYTE) Canal_Y;
	}
// final dos procedimentos de conversão


// procedimentos para montagem do buffer com os valores do canal y para aplicar o filtro da média
for (j=2; j<(numero*3); j=j+3)
{buffer_y[i] = buffer_temp[j];
 buffer_media_y[i] = buffer_temp[j];
 i++;
}

// procedimentos para o cálculo do filtro da média

for (lin=1; lin<(altura-1); lin++)
	{
		for (col=1; col<(largura-1); col++)
		{
			v1 = buffer_y [(largura * (lin-1))+(col-1)];
			v2 = buffer_y [(largura * (lin-1))+ col];
			v3 = buffer_y [(largura * (lin-1))+(col+1)];
			v4 = buffer_y [(largura * lin)+(col-1)];
			v5 = buffer_y [(largura * lin)+(col+1)];
			v6 = buffer_y [(largura * (lin+1))+(col-1)];
			v7 = buffer_y [(largura * (lin+1))+col];
			v8 = buffer_y [(largura * (lin+1))+(col+1)];
			media = (v1+v2+v3+v4+v5+v6+v7+v8)/8;
			buffer_media_y[(largura * lin) + col] = media;
		}
	}

// formação do Delta D que corresponde a função de informação espacial

for (j=0; j < numero; j++)
{imagem1[j] = buffer_y[j] - buffer_media_y[j];}

// formação da fração negativa da função de informação espacial
delta = P3;

for (j=0; j < numero; j++)
{ if (imagem1[j]>=0) 
	{imagem2[j]=0;}
  else
	{imagem2[j]=imagem1[j];}
}

// formação do módulo da função de informação espacial
for (j=0; j<numero; j++)
{	if (imagem1[j]<0) 
		{imagem3[j]=imagem1[j]*(-1);}
	else 
		{imagem3[j]=imagem1[j];}
}

// formação da imagem final

for (j=0; j < numero; j++)
{imagem4[j] = ( buffer_y[j] + (delta * imagem3[j]) + (delta * imagem2[j]) );}


// procedimentos para retorno ao buffer temp do canal Y modificado
i=0;
for (j=2; j<(numero*3); j=j+3)
{buffer_temp[j]= imagem4[i];
 i++;
}




// procedimentos para conversão do formato YCrCB para o formato RGB

for (cont_conv = 0; cont_conv < numero *3; cont_conv= cont_conv+3)
	{
		Canal_Y  = buffer_temp[cont_conv+2];
		if (Canal_Y > 240) {Canal_Y=240;} 
		if (Canal_Y < 16) {Canal_Y=16;}
		Canal_Cr = buffer_temp[cont_conv+1];
		Canal_Cb = buffer_temp[cont_conv];
		Canal_b = (1.164*(Canal_Y - 16)) + (2.018*(Canal_Cb - 128));
		Canal_g = (1.164*(Canal_Y - 16)) - (0.813*(Canal_Cr - 128)) - (0.391*(Canal_Cb - 128));
		Canal_r = (1.164*(Canal_Y - 16)) + (1.596*(Canal_Cr - 128));
		if (Canal_b > 255) {Canal_b=255;} 
		if (Canal_b < 0) {Canal_b=0;}
		if (Canal_g > 255) {Canal_g=255;} 
		if (Canal_g < 0) {Canal_g=0;}
		if (Canal_r > 255) {Canal_r=255;} 
		if (Canal_r < 0) {Canal_r=0;}
		lpTemp[cont_conv] = (BYTE) Canal_b;
		lpTemp[cont_conv+1] = (BYTE) Canal_g;
		lpTemp[cont_conv+2] = (BYTE) Canal_r;

	}
// fim dos procedimentos para conversão do formato YCrCb para RGB

}

// Equalizacao no espaço de cores YCrCb no canal Y

BOOL CPDIBase::EqualizacaoYCrCb()
{
long int		histoY[256], histoequalizadoY[256];
long int    numero, val;
double		probaY[256], probasomaY[256];
double		Canal_Y= 0, Canal_Cr=0, Canal_Cb=0, x;  
int			inteiroy, cont;
long int	cont_conv, contador; 
long int	Canal_b, Canal_g, Canal_r;
BYTE		cb, cr, y;
BYTE		*lpTemp;
double		*buffer_temp;
	
numero = (int) (GetWidth() * GetHeight());
buffer_temp = new double[numero*3];
	
	
	for (val = 0; val < 256; val++)
	{	histoY[val] = 0;   
	}
	
	lpTemp = lpBits;
	
	// Procedimentos para conversão do formato RGB para o formato YCrCB
	
	for (cont_conv=0;  cont_conv < (numero*3); cont_conv = cont_conv+3)
	{
		Canal_b = (int) lpTemp[cont_conv];
		Canal_g = (int) lpTemp[cont_conv+1];
		Canal_r = (int) lpTemp[cont_conv+2];
		Canal_Y = (0.257 * Canal_r) + (0.504 * Canal_g) + (0.098 * Canal_b) + 16;
		Canal_Cr = (0.439 * Canal_r) - (0.368 * Canal_g) - (0.071 * Canal_b) + 128;
		Canal_Cb = -(0.148 * Canal_r) - (0.291 * Canal_g) + (0.439 * Canal_b) + 128;
		buffer_temp[cont_conv]= Canal_Cb;
		buffer_temp[cont_conv+1]= Canal_Cr;
		buffer_temp[cont_conv+2]= Canal_Y;
		lpTemp[cont_conv] = (BYTE) Canal_Cb;
		lpTemp[cont_conv+1] = (BYTE) Canal_Cr;
		lpTemp[cont_conv+2] = (BYTE) Canal_Y;
	}

// final dos procedimentos de conversão
	
	for (cont_conv=0; cont_conv < (numero *3); cont_conv=cont_conv+3)
	{
		inteiroy = (int) buffer_temp[cont_conv+2];
		histoY[inteiroy] = histoY[inteiroy] + 1;
	}

	/* probabilidades do histograma */
	for (val = 0; val < 256; val++)
	{
		probaY[val]  = (double)(histoY[val])/(numero);
	}
	
	/* probabilidades cumulada do histograma */
	
	probasomaY[0]  = probaY[0];

	for (val = 1; val < 256; val++)
	{	probasomaY[val] = probasomaY[val-1] + probaY[val];
	}
	
	/* equalizacao do histograma */

  	for (val = 0; val < 256; val++)
	{    
		histoequalizadoY[val] = (int) (probasomaY[val] * 225 + 0.5) ; /* Multipliccao por 224 niveis de cinza */
	} /* fim da equalização */

	GrayPalette(); 
	
	for (cont_conv=0; cont_conv < (numero *3); cont_conv=cont_conv+3)
	{
		y = buffer_temp[cont_conv+2];
		buffer_temp[cont_conv+2] = histoequalizadoY[y];
	}

// procedimentos para conversão do formato YCrCB para o formato RGB

for (cont_conv = 0; cont_conv < numero *3; cont_conv= cont_conv+3)
	{
		Canal_Y  = buffer_temp[cont_conv+2];
		if (Canal_Y > 240) {Canal_Y=240;} 
		if (Canal_Y < 16) {Canal_Y=16;}
		Canal_Cr = buffer_temp[cont_conv+1];
		Canal_Cb = buffer_temp[cont_conv];
		Canal_b = (1.164*(Canal_Y - 16)) + (2.018*(Canal_Cb - 128));
		Canal_g = (1.164*(Canal_Y - 16)) - (0.813*(Canal_Cr - 128)) - (0.391*(Canal_Cb - 128));
		Canal_r = (1.164*(Canal_Y - 16)) + (1.596*(Canal_Cr - 128));
		if (Canal_b > 255) {Canal_b=255;} 
		if (Canal_b < 0) {Canal_b=0;}
		if (Canal_g > 255) {Canal_g=255;} 
		if (Canal_g < 0) {Canal_g=0;}
		if (Canal_r > 255) {Canal_r=255;} 
		if (Canal_r < 0) {Canal_r=0;}
		lpTemp[cont_conv] = (BYTE) Canal_b;
		lpTemp[cont_conv+1] = (BYTE) Canal_g;
		lpTemp[cont_conv+2] = (BYTE) Canal_r;

	}
// fim dos procedimentos para conversão do formato YCrCb para RGB
	
	return TRUE;
}
