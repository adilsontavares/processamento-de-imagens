// DlgFatorZoom.cpp : implementation file
//

#include "stdafx.h" 

#ifndef __PDIMAGEM_H__
	#include "PDImagem.h"
#endif

#include "PDIBase.h"
#include "PDIMorfb.h"
#include "DlgFatorZoom.h"
                   
extern CObList ObjectList;
extern POSITION pos;
                   
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFatorZoom dialog

CDlgFatorZoom::CDlgFatorZoom(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgFatorZoom::IDD, pParent)
{
    m_pView = NULL;
    //{{AFX_DATA_INIT(CDlgFatorZoom)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}
                                        
CDlgFatorZoom::CDlgFatorZoom(CView* pView, double *pH, double *pL) 
    : CDialog(IDD_FATORZOOM)
{  
   m_pView = pView;
   m_pH = pH;
   m_pL = pL;
}   

//initDialog - coloca lá dentro valores default(1,1) e constroi a tela e pega 
BOOL CDlgFatorZoom::OnInitDialog()
{  
   *m_pH = 1.0;
   *m_pL = 1.0;
   m_Titulo	= "Fator Ampliação/Redução";
   CWnd::SetWindowText(m_Titulo);
   return CDialog::OnInitDialog();
}
   
BOOL CDlgFatorZoom::Create()                
{  
   BOOL ret; 
   ret = CDialog::Create(IDD_FATORZOOM);
   
   return ret;
   
}
      
void CDlgFatorZoom::DoDataExchange(CDataExchange* pDX)
{

    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_ZOOMH, *m_pH);
    DDV_MinMaxDouble(pDX, *m_pH, 0.1, 10);
    DDX_Text(pDX, IDC_ZOOML, *m_pL);
    DDV_MinMaxDouble(pDX, *m_pL, 0.1, 10);
    //{{AFX_DATA_MAP(CDlgFatorZoom)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


void CDlgFatorZoom::OnOK()
{
   // Funcao ficticia - resposta a tecla <enter>    

}


void CDlgFatorZoom::OnCancel()
{
        
    CDialog::OnCancel();
    if (m_pView != NULL) {
        m_pView->SendMessage(ID_GOODBYE, IDCANCEL);
    }   
}

BEGIN_MESSAGE_MAP(CDlgFatorZoom, CDialog)

    ON_BN_CLICKED(IDOK, OnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnCancel)
    
END_MESSAGE_MAP()

//////


void CDlgFatorZoom::OnClickedOk()
{
    CDialog::OnOK();

    if ((*m_pH > 0 && *m_pH < 11) && (*m_pL > 0 && *m_pL < 11)) { 
       if (m_pView != NULL) {
           m_pView->SendMessage(ID_GOODBYE, IDOK);
       }   
    }    
}



/////////////////////////////////////////////////////////////////////////////
// CDlgFatorZoom message handlers