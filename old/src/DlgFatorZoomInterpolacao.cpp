// DlgFatorZoomInterpolacao.cpp : implementation file
//

#include "stdafx.h"
#include "PDImagem.h"
#include "PDIBase.h"
#include "PDIMorfb.h"
#include "DlgFatorZoomInterpolacao.h"



// CDlgFatorZoomInterpolacao dialog

IMPLEMENT_DYNAMIC(CDlgFatorZoomInterpolacao, CDialog)

CDlgFatorZoomInterpolacao::CDlgFatorZoomInterpolacao(CWnd* pParent /*= NULL*/)
: CDialog(CDlgFatorZoomInterpolacao::IDD, pParent)
{
	m_pView = NULL;
}

CDlgFatorZoomInterpolacao::CDlgFatorZoomInterpolacao(CView* pView, double *pFator) 
    : CDialog(IDD)
{  
   m_pView = pView;
   m_pFator = pFator;
}

//initDialog - coloca lá dentro valores default(1,1) e constroi a tela e pega 
BOOL CDlgFatorZoomInterpolacao::OnInitDialog()
{  
   *m_pFator = 1.0;
   return CDialog::OnInitDialog();
}

CDlgFatorZoomInterpolacao::~CDlgFatorZoomInterpolacao()
{
}

void CDlgFatorZoomInterpolacao::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_EDIT_FAT_INTERP, *m_pFator);
    DDV_MinMaxDouble(pDX, *m_pFator, 0.1, 10);
}

BOOL CDlgFatorZoomInterpolacao::Create()                
{  
   BOOL ret; 
   ret = CDialog::Create(IDD_FATORZOOM_INTERP);
   
   return ret;  
}

void CDlgFatorZoomInterpolacao::OnOK()
{// Funcao ficticia - resposta a tecla <enter>    
}

void CDlgFatorZoomInterpolacao::OnCancel()
{
    CDialog::OnCancel();
    if (m_pView != NULL) {
        m_pView->SendMessage(ID_GOODBYE, IDCANCEL);
    }   
}

void CDlgFatorZoomInterpolacao::OnClickedOk()
{
    CDialog::OnOK();

    if (	*m_pFator > 0 && *m_pFator < 11	) { 
       if (	m_pView != NULL	) {
           m_pView->SendMessage(ID_GOODBYE, IDOK);
       }   
    }    
}

BEGIN_MESSAGE_MAP(CDlgFatorZoomInterpolacao, CDialog)
	ON_BN_CLICKED(IDOK, OnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

// CDlgFatorZoomInterpolacao message handlers