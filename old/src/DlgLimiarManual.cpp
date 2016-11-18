// DlgLimiarManual.cpp : implementation file
//

#include "stdafx.h"
#ifndef __PDIMAGEM_H__
    #include "PDImagem.h"
#endif

#include "PDIBase.h"
#include "DlgLimiarManual.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLimiarManual dialog

CDlgLimiarManual::CDlgLimiarManual(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgLimiarManual::IDD, pParent)
{
    m_pView = NULL;
    //{{AFX_DATA_INIT(CDlgLimiarManual)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

                                        
CDlgLimiarManual::CDlgLimiarManual(CView* pView, CPDIBase* pImgf, CPDIBase* pImg, CString* Titulo) 
    : CDialog(IDD_LIMIARMANUAL)
{  
	m_Titulo = *Titulo;
	m_pView = pView;
	m_pImgf = pImgf;
	m_pImg = pImg;
	CWnd::SetWindowText(m_Titulo);
}   
   

BOOL CDlgLimiarManual::OnInitDialog()
{  
   p_impr = m_pImgf;
   int style;
    
   GetDlgItem(IDC_IMAGEM)->Invalidate();
   CWnd::SetWindowText(m_Titulo);
   CButton* pButton = (CButton*) GetDlgItem(IDC_IMAGEM);
   style = pButton->GetButtonStyle();
   pButton->SetButtonStyle(style,0);
   return CDialog::OnInitDialog();
}
   
BOOL CDlgLimiarManual::Create()                
{  
   BOOL ret; 
   ret = CDialog::Create(IDD_DIALOG2);
   return ret;
}
      
void CDlgLimiarManual::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgLimiarManual)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

void CDlgLimiarManual::OnOK()
{
        
    CDialog::OnOK();
    if (m_pView != NULL) {
       m_pView->SendMessage(ID_GOODBYE, IDOK);
    }   
}

void CDlgLimiarManual::OnCancel()
{
        
    CDialog::OnCancel();
    if (m_pView != NULL) {
        m_pView->SendMessage(ID_GOODBYE, IDCANCEL);
    }   
}


BEGIN_MESSAGE_MAP(CDlgLimiarManual, CDialog)
    
    ON_BN_CLICKED(IDC_TESTAR, OnClickedVerifica)
    ON_WM_DRAWITEM()
                   
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLimiarManual message handlers


void CDlgLimiarManual::OnClickedVerifica()
{
      int Limiar;  
      HCURSOR OldCursor;
      CComboBox* pBox = (CComboBox*) GetDlgItem(IDC_LIMIAR);
      Limiar = pBox->GetCurSel();
    
      OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

      m_pImg->hDIB = m_pImgf->CopyHandle(); 
      m_pImg->InitDIBData();
       
      m_pImg->LimiarManual(Limiar);

      p_impr = m_pImg;
      SetCursor(OldCursor);
      GetDlgItem(IDC_IMAGEMR)->Invalidate();
}                   
                    

void CDlgLimiarManual::OnDrawItem(int ident, LPDRAWITEMSTRUCT lpDIS)
{


      // draw the whole button
      CDC* pDC = CDC::FromHandle(lpDIS->hDC);
      CRect rc;
      rc.CopyRect(&lpDIS->rcItem);

      CPalette* pOldPal = NULL;
       
      if (p_impr->IsPaletted())  {
         pOldPal = pDC->SelectPalette(p_impr->pal, TRUE);
         pDC->RealizePalette();         
      }   
 
       
       
      int cxDIB = (int) p_impr->GetWidth();         // Size of DIB - x
      int cyDIB = (int) p_impr->GetHeight();        // Size of DIB - y
      CRect rcDIB;
      rcDIB.top = rcDIB.left = 0;
      rcDIB.right = cxDIB;
      rcDIB.bottom = cyDIB;
      CRect rcDest;
 
      rcDest = rc;
      p_impr->PaintDIB(pDC->m_hDC, &rcDest, &rcDIB);

      if (pOldPal != NULL)
         pDC->SelectPalette(pOldPal, FALSE);

} //DlgHistogram


