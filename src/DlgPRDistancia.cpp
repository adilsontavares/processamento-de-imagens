// DlgPRDistancia.cpp : implementation file
//

#include "stdafx.h"

#include "DlgPRDistancia.h"
#include "PDImagem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPRDistancia dialog

CDlgPRDistancia::CDlgPRDistancia(CPDIBase*	poImagemPadrao		, 
								 CPDIBase*	poImagemTeste		,
								 CString	sTitleImagemPadrao	,
								 CString	sTitleImagemTeste	,
								 CString	sDistancia			) 
    : CDialog(IDD_PR_DISTANCIA)
{
	m_poImagemPadrao		= poImagemPadrao	;
	m_poImagemTeste			= poImagemTeste		;
	m_sTitleImagemPadrao	= sTitleImagemPadrao;
	m_sTitleImagemTeste		= sTitleImagemTeste	;
	m_sDistancia			= sDistancia		;
}   

BOOL CDlgPRDistancia::OnInitDialog()
{  
	GetDlgItem(IDC_IMAGEM_PADRAO	)->Invalidate();
	GetDlgItem(IDC_IMAGEM_TESTE		)->Invalidate();

	SetDlgItemText(IDC_TITULO_PADRAO	, m_sTitleImagemPadrao	);
	SetDlgItemText(IDC_TITULO_TESTE		, m_sTitleImagemTeste	);
	SetDlgItemText(IDC_DISTANCIA		, m_sDistancia			);

	return CDialog::OnInitDialog();
}

void CDlgPRDistancia::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgPRDistancia)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgPRDistancia, CDialog)
    //{{AFX_MSG_MAP(CDlgPRDistancia)
    //}}AFX_MSG_MAP
    ON_WM_DRAWITEM()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPRDistancia message handlers

void CDlgPRDistancia::OnOK()
{
    // TODO: Add extra validation here
    
    CDialog::OnOK();
}

void CDlgPRDistancia::OnDrawItem(int ident, LPDRAWITEMSTRUCT lpDIS)
{
	// draw the whole button
	CDC* pDC;
	pDC = CDC::FromHandle(lpDIS->hDC);
	
	CRect rc;
	rc.CopyRect(&lpDIS->rcItem);

	CPalette* pOldPal = NULL;

	CPDIBase* poImagem;
	poImagem = NULL;

	if (ident == IDC_IMAGEM_PADRAO	)
		poImagem = m_poImagemPadrao;
	if (ident == IDC_IMAGEM_TESTE	)
		poImagem = m_poImagemTeste;

	if (poImagem != NULL)
	{
		if (poImagem->IsPaletted())
		{
			pOldPal = pDC->SelectPalette(poImagem->pal, TRUE);
			pDC->RealizePalette();
		}   

		int cxDIB = poImagem->GetWidth();		// Size of DIB - x
		int cyDIB = poImagem->GetHeight();	// Size of DIB - y

		CRect rcDIB;
		rcDIB.top		= 0;
		rcDIB.left		= 0;
		rcDIB.right		= cxDIB;
		rcDIB.bottom	= cyDIB;

		CRect rcDest;
		rcDest = rc;

		poImagem->PaintDIB(pDC->m_hDC, &rcDest, &rcDIB);
	}

	if (pOldPal != NULL)
		pDC->SelectPalette(pOldPal, FALSE);
}