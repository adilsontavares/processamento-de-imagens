// DlgInfo.cpp : implementation file
//

#include "stdafx.h"

#include "DlgInfo.h"
#include "PDImagem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInfo dialog

CDlgInfo::CDlgInfo(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgInfo::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgInfo)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


CDlgInfo::CDlgInfo(CView* pView, CPDImagemDoc* pDoc) 
    : CDialog(IDD_INFO)
{  
	m_pView = pView;
	m_pDoc = pDoc;
}   

BOOL CDlgInfo::OnInitDialog()
{  
   GetDlgItem(IDC_IMAGEMEX)->Invalidate();
   CString Aux;  
   char v5[5], v10[10];

   SetDlgItemText(IDC_TITULO, m_pDoc->GetTitle());

   _itoa((int) m_pDoc->Imagem.GetHeight(), v5, 10);
   Aux = v5;
   Aux = Aux + " pixels" ;
   SetDlgItemText(IDC_ALTURA, Aux);

   _itoa((int) m_pDoc->Imagem.GetWidth(), v5, 10);
   Aux = v5;
   Aux = Aux + " pixels" ;
   SetDlgItemText(IDC_LARGURA, Aux);

   _itoa((int) m_pDoc->Imagem.lpBI->biPlanes, v5, 10);
   Aux = v5;
   SetDlgItemText(IDC_PLANOS, Aux);

   _itoa((int) m_pDoc->Imagem.GetBitsPerPixel(), v5, 10);
   Aux = v5;
   SetDlgItemText(IDC_BPS, Aux);

   _itoa((int) m_pDoc->Imagem.DIBNumColors(), v5, 10);
   Aux = v5;
   SetDlgItemText(IDC_CORES, Aux);

   _itoa((int) m_pDoc->Imagem.bmfHeader.bfSize, v10, 10);
   Aux = v10;
   Aux = Aux + " bytes";
   SetDlgItemText(IDC_TAMANHO, Aux);

   return CDialog::OnInitDialog();
}

void CDlgInfo::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgInfo)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgInfo, CDialog)
    //{{AFX_MSG_MAP(CDlgInfo)
    //}}AFX_MSG_MAP
    ON_WM_DRAWITEM()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInfo message handlers

void CDlgInfo::OnOK()
{
    // TODO: Add extra validation here
    
    CDialog::OnOK();
}

void CDlgInfo::OnDrawItem(int ident, LPDRAWITEMSTRUCT lpDIS)
{
	// draw the whole button
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rc;
	rc.CopyRect(&lpDIS->rcItem);

	CPalette* pOldPal = NULL;

	if (m_pDoc->Imagem.IsPaletted())
	{
		pOldPal = pDC->SelectPalette(m_pDoc->Imagem.pal, TRUE);
		pDC->RealizePalette();         
	}   

	int cxDIB = (int) m_pDoc->Imagem.GetWidth();         // Size of DIB - x
	int cyDIB = (int) m_pDoc->Imagem.GetHeight();        // Size of DIB - y

	CRect rcDIB;
	rcDIB.top		= 0;
	rcDIB.left		= 0;
	rcDIB.right		= cxDIB;
	rcDIB.bottom	= cyDIB;
	CRect rcDest;

	rcDest = rc;
	m_pDoc->Imagem.PaintDIB(pDC->m_hDC, &rcDest, &rcDIB);

	if (pOldPal != NULL)
		pDC->SelectPalette(pOldPal, FALSE);

}