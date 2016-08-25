// Definicao da classe CPDIBase

#ifndef _PDIBASE_H_

#define _PDIBASE_H_

#include <afxwin.h>
#include "stdafx.h"
#include <math.h>
#include <io.h>
#include <direct.h>
#include <memory.h>

#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')
#define PALVERSION   0x300
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
#define PI                   3.1415926535  
#define ROTATE(a,i,j,k,l) g=a[i][j];h=a[k][l];a[i][j]=g-s*(h+g*tau);\
        a[k][l]=h+s*(g-h*tau);
        
#define FREE_ARG char*
#define NR_END 1

#define TIMG_CONTORNO 3000
#define TIMG		  100
#define GDE_ESPACAMENTO_BRANCO 30

#define M_E 2.7182818284590452354

//ezequiel 19/09/2009
enum TipoKernell{TRIANGULO, BELL, HERMITTE, MITCHELL, CUBIC};

DECLARE_HANDLE(HDIB);

class CPDImagemDoc;

class CPDIBase : public CObject
{   
public:
	CString				ImageTitle;   // Titulo da Janela
	CObject*			pwPalette;    // Janela da Palheta
	CObject*			pwHistogram;  // Janela do Histograma
	CSize				m_sizeDOC;                
	CRgn rgn;
	CRgn* pRgn;
	BOOL Region;
	CRect rect;
	//ezequiel 19/09/2009
	float FatorAmpliacao;

	// Dados da Imagem

	float d[4], v[4][4]; 
	float a[4][4];
	int Escolha; 
	BOOL Pinta;

	CPalette*			pal;				// Palheta
	LPLOGPALETTE		Pal;

	BITMAPFILEHEADER	bmfHeader;		// Pointer Cabecalho do Bitmap
	LPBITMAPINFOHEADER	lpBI;			// Pointer to DIB info structure
	LPBITMAPINFO		lpbmi;			// pointer to BITMAPINFO structure (Win3.0)
	LPBITMAPCOREINFO	lpbmc;			// pointer to BITMAPCOREINFO structure (old)
	PALETTEENTRY		peValue;		// entrada da paleta

	DWORD				dwDIBSize;
	BOOL				HSI;			// sistema de cor
	BYTE				H;				// Hue - sistema de cores HSI
	BYTE				I;				// Intensity - sistema de cores HSI
	BYTE				S;				// Saturation - sistema de cores HSI

	float				hue,saturation,intensity;

	DWORD				bmWidthBytes;
	DWORD				Heightm1;		// Altura - 1
	WORD				palSize;		// Tamalho da Palheta
	int					iBPP;			// Bits por pixel -  negativo
										// se a imagem nao usa palheta

	BYTE *				pDIB;			// Ponteiro para um DIB
	BYTE *				lpBits;			// Ponteiro Bits do DIB  
	HDIB				hDIB;			// Handle do DIB

						CPDIBase();		// Construtor da Classe;
						~CPDIBase();	// Destrutor da Classe;
                      
	void				InitDIBData(); 
	BOOL				WhiteImage();
	BOOL				IsPaletted() const;
	WORD				GetBitsPerPixel() const;

	LONG				GetWidth() const;
	LONG				GetHeight() const;

	WORD				GetNearestPaletteIndex( DWORD dwColor ) const;

	WORD				GetPaletteEntries(	WORD wStartIndex, WORD wNumEntries,
											LPPALETTEENTRY pPaletteColors ) const;
	WORD				SetPaletteEntries(	WORD wStartIndex, WORD wNumEntries,
											LPPALETTEENTRY pPaletteColors );

	WORD				GetPaletteSize();

	void				SetPixel(DWORD , DWORD , COLORREF );
	DWORD				GetPixel(DWORD , DWORD ) const;

	DWORD				SetPixelBW(DWORD x, DWORD y, COLORREF dwColor);
	DWORD				GetPixelBW(DWORD x, DWORD y) const;

	HDIB				ReadDIBFile(CFile& file);
	HDIB				AumentaDIB(int h, int l);
	BOOL				SaveDIB(CFile& file);
	BOOL				CreateDIBPalette();
	WORD				DIBNumColors();
	BOOL				PaintDIB(	HDC hDC, LPRECT  lpDCRect,
									LPRECT  lpDIBRect);
	LPSTR				FindDIBBits(	LPSTR lpbi);
	BYTE*				FindBits();
	BOOL				GetRGBHistogram(	LPDWORD rCount, LPDWORD gCount,
											LPDWORD bCount);

	void  				ConverteHSI();
  	void 				OnRGBToYCrCb();
	void 				OnYCrCbToRGB(); 

	BOOL  				Equalizacao(); 

	void				LimiarOtsu();
	void				LimiarKittler();
	void				LimiarEntropia();
	void				LimiarAnisotropia();
	void				LimiarManual(int Limiar);

	void				InverteImagem();
	void				ExtracaoPrimitivas(int digito);
	HDIB				CopyHandle();
	HDIB				CopyHandleWhite();
	HDIB				CopyRegion();
	BOOL				GrayPalette();
	void				TonsCinza();
	void				TonsCinzaNTSC();
	BOOL				EqualizacaoYCrCb();
	void				AprimoraNitidezYCrCbCalculo1(double P1);
	void				AprimoraNitidezYCrCbCalculo2(double P2);
	void				AprimoraNitidezYCrCbCalculo3(double P3);

//	void				LineMasks(char tipo);

	int					temloops(CDC *pDC);

	//Função pra equalizar histograma em YIQ - Jean Felsky 06/04/2010
	void EqualizaHistogramaYIQ(char canal, double *matrizY);
	void EqualizaHistogramaYIQmatriz(char canal, double *matrizY);
	BYTE converteI(double I);
	BYTE converteQ(double Q);
	double desconverteI(BYTE I);
	double desconverteQ(BYTE Q);	
	BYTE normalizaRGB(double RGB, double MaiorRGB, double MenorRGB);

	void AprimoraNitidezYIQ(double *matrizY);
	double normaliza01(double RGB, double MaiorRGB, double MenorRGB);
};             

#endif // !defined(_PDIBASE_H_)




