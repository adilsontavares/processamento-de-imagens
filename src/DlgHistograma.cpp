// DlgHistograma.cpp : implementation file
//

#include "stdafx.h" 

#ifndef _PDIMAGEM_H_
   #include "PDImagem.h"
#endif

#include "PDIBase.h"
#include "DlgHistograma.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHistograma dialog

CDlgHistograma::CDlgHistograma(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgHistograma::IDD, pParent)
{
	m_pView = NULL;

	//{{AFX_DATA_INIT(CDlgHistograma)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

                                        
CDlgHistograma::CDlgHistograma(CView* pView, CPDIBase* pImg,CString* Titulo) 
    : CDialog(IDD_HISTOGRAMA)
{  
	m_Titulo	= *Titulo;
	m_pView		= pView;
	m_pImg		= pImg;
	m_pImg->HSI	= FALSE;
}   
   

BOOL CDlgHistograma::OnInitDialog()
{
	return CDialog::OnInitDialog();
}
   
BOOL CDlgHistograma::Create()                
{  
	BOOL ret; 
	ret = CDialog::Create(IDD_HISTOGRAMA);

	m_pImg->HSI = FALSE;
	CheckDlgButton(IDC_VERMELHO,1);
	CheckDlgButton(IDC_VERDE,1);
	CheckDlgButton(IDC_AZUL,1);
	CheckDlgButton(IDC_RGB,1);
	CheckRadioButton(IDC_RGB,IDC_HSI,IDC_RGB);

	SetDlgItemText(IDC_VERMELHO, "R");
	SetDlgItemText(IDC_VERDE, "G");
	SetDlgItemText(IDC_AZUL, "B");
	SetDlgItemText(IDC_ESCALA,"0...........32...........64..........96.........128.........160.........192........224.........256");

	mR = mG = mB = TRUE;         
	CWnd::SetWindowText(m_Titulo);

	return ret;
}
      
void CDlgHistograma::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgHistograma)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

void CDlgHistograma::OnOK()
{
	CDialog::OnOK();
	if (m_pView != NULL) 
	{
	   m_pView->SendMessage(ID_GOODBYE, IDOK);
	}   
}

BEGIN_MESSAGE_MAP(CDlgHistograma, CDialog)
	ON_BN_CLICKED(IDC_AZUL, OnClickedAzul)
	ON_BN_CLICKED(IDC_VERDE, OnClickedVerde)
	ON_BN_CLICKED(IDC_VERMELHO, OnClickedVermelho)
	ON_BN_CLICKED(IDC_RGB, OnClickedRGB)
	ON_BN_CLICKED(IDC_HSI, OnClickedHSI)

	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHistograma message handlers

void CDlgHistograma::OnClickedAzul()
{     
	if (mB) 
		mB = FALSE;
	else
		mB = TRUE;

	CheckDlgButton(IDC_AZUL,mB);
	GetDlgItem(IDC_AREA_DESENHO)->Invalidate();
}

void CDlgHistograma::OnClickedVerde()
{
      if (mG) 
          mG = FALSE;
      else
          mG = TRUE;

      CheckDlgButton(IDC_VERDE,mG);
      GetDlgItem(IDC_AREA_DESENHO)->Invalidate();
}

void CDlgHistograma::OnClickedVermelho()
{
	if (mR)
		mR = FALSE;
	else
		mR = TRUE;

	CheckDlgButton(IDC_VERMELHO,mR);
	GetDlgItem(IDC_AREA_DESENHO)->Invalidate();
}                   


void CDlgHistograma::OnClickedRGB()
{
	HCURSOR OldCursor;

	mR = TRUE;
	mG = TRUE;
	mB = TRUE;

	CheckDlgButton(IDC_VERMELHO	,mR);
	CheckDlgButton(IDC_VERDE		,mG);
	CheckDlgButton(IDC_AZUL		,mB);


	CheckRadioButton(IDC_RGB,IDC_HSI,IDC_RGB);
	SetDlgItemText(IDC_VERMELHO, "R");
	SetDlgItemText(IDC_VERDE, "G");
	SetDlgItemText(IDC_AZUL, "B");
	SetDlgItemText(IDC_ESCALA,"0...........32...........64..........96.........128.........160.........192........224.........256");

	OldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
	m_pImg->HSI = FALSE;
	m_pImg->GetRGBHistogram(rCount, gCount, bCount); 
	SetCursor(OldCursor);

	GetDlgItem(IDC_AREA_DESENHO)->Invalidate();
}

void CDlgHistograma::OnClickedHSI()
{
	HCURSOR OldCursor;

	CheckDlgButton(IDC_VERMELHO,1);
	CheckDlgButton(IDC_VERDE,1);
	CheckDlgButton(IDC_AZUL,1);
	mR = mG = mB = TRUE;

	CheckRadioButton(IDC_RGB,IDC_HSI,IDC_HSI);
	SetDlgItemText(IDC_VERMELHO, "H");
	SetDlgItemText(IDC_VERDE, "S");
	SetDlgItemText(IDC_AZUL, "I");
	SetDlgItemText(IDC_ESCALA,"0.........0.12.........0.25........0.37.........0.5........0.62........0.75.......0.87........1,0");

	m_pImg->HSI = TRUE;
	OldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
	m_pImg->GetRGBHistogram(rCount, gCount, bCount); 
	SetCursor(OldCursor);

	GetDlgItem(IDC_AREA_DESENHO)->Invalidate();
}

void CDlgHistograma::OnDrawItem(int ident, LPDRAWITEMSTRUCT lPDIS)
{
	// draw the whole button
	CDC* pDC = CDC::FromHandle(lPDIS->hDC);
	CRect rc;
	rc.CopyRect(&lPDIS->rcItem);

	// Maximum
	DWORD maxRed = 1, maxGreen = 1, maxBlue = 1;
	int   idxRed = 0, idxGreen = 0, idxBlue = 0;

	for (int i = 0; i < 256; i++) 
	{
		if (rCount[i] > maxRed  ) { maxRed   = rCount[i]; idxRed   = i; }
		if (gCount[i] > maxGreen) { maxGreen = gCount[i]; idxGreen = i; }
		if (bCount[i] > maxBlue ) { maxBlue  = bCount[i]; idxBlue  = i; }
	}

	CPen         *pPenAux, *pOldPen;
	CPoint       ptAux;
	COLORREF     crColor;
	DWORD        dwWidth  = abs(rc.Width());
	DWORD        dwHeight = abs(rc.Height() - 1);

	DWORD   dwYSize;
	double  sclRed, sclGreen, sclBlue,
	scaleX,
	posRed, posGreen, posBlue;

	if (mR && mG && mB) 
	{
		dwYSize  = dwHeight/3;
		posRed   = (double) dwYSize;
		posGreen = (double) (dwHeight-dwYSize);
		posBlue  = (double) dwHeight;
	}
	else 
	{
		if (mR && mG) 
		{
			dwYSize  = dwHeight/2;
			posRed   = (double) dwYSize;
			posGreen = (double) dwHeight;
		}
		else 
		{
			if (mR && mB) 
			{
				dwYSize  = dwHeight/2;
				posRed   = (double) dwYSize;
				posBlue  = (double) dwHeight;
			}
			else 
			{
				if (mG && mB) 
				{
					dwYSize  = dwHeight/2;
					posGreen = (double) dwYSize;
					posBlue  = (double) dwHeight;
				}    
				else 
				{
					dwYSize  = dwHeight;
					posRed   = (double) dwYSize;
					posGreen = (double) dwYSize;
					posBlue  = (double) dwYSize;
				} 
			}
		}         
	}

	// Red channel
	if  (mR) 
	{
		sclRed   = (double) dwYSize / (double) maxRed;   // Scale     
		scaleX   = (double) dwWidth/256.;
		crColor = RGB(255,0,0);
		pPenAux = new CPen(PS_SOLID, 1, crColor);
		pOldPen = (CPen*) pDC->SelectObject(pPenAux);
		ptAux.x = 0;
		ptAux.y = (int) ( posRed - (double) rCount[0] * sclRed );
		pDC->MoveTo( ptAux );

		for (int i = 1; i < 256; i++) 
		{
			ptAux.x = (int) ( (double)     i * scaleX );
			ptAux.y = (int) ( posRed - (double) rCount[i] * sclRed );
			pDC->LineTo( ptAux );
		} // for i

		pDC->SelectObject(pOldPen);
		delete pPenAux;
	} // if

	// Green channel
	if (mG)
	{
		sclGreen = (double) dwYSize / (double) maxGreen; // factors
		scaleX   = (double) dwWidth/256.; 
		crColor = RGB(0,255,0);
		pPenAux = new CPen(PS_SOLID, 1, crColor);
		pOldPen = (CPen*) pDC->SelectObject(pPenAux);
		ptAux.x = 0;
		ptAux.y = (int) ( posGreen - (double) gCount[0] * sclGreen );
		pDC->MoveTo( ptAux );

		for (int i = 1; i < 256; i++) 
		{
			ptAux.x = (int) ( (double)     i * scaleX );
			ptAux.y = (int) ( posGreen - (double) gCount[i] * sclGreen );
			pDC->LineTo( ptAux );
		} // for i

		pDC->SelectObject(pOldPen);
		delete pPenAux;
	} // if

	// Blue channel
	if (mB)
	{
		sclBlue  = (double) dwYSize / (double) maxBlue;
		scaleX   = (double) dwWidth/256.;
		crColor = RGB(0,0,255);
		pPenAux = new CPen(PS_SOLID, 1, crColor);
		pOldPen = (CPen*) pDC->SelectObject(pPenAux);
		ptAux.x = 0;
		ptAux.y = (int) ( posBlue - (double) bCount[0] * sclBlue );
		pDC->MoveTo( ptAux );

		for (int i = 1; i < 256; i++) 
		{
			ptAux.x = (int) ( (double)     i * scaleX );
			ptAux.y = (int) ( posBlue - (double) bCount[i] * sclBlue );
			pDC->LineTo( ptAux );
		} // for i

		pDC->SelectObject(pOldPen);
		delete pPenAux;
	} // if
} // DlgHistogram