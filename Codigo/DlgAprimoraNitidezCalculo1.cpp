// DlgAprimoraNitidezCalculo1.cpp : implementation file
//

#include "stdafx.h"
#ifndef _PDIMAGEM_H_
	#include "PDImagem.h" 
#endif

#include "PDIBase.h"
// #include "PDImagem.h"
#include "PDIMorfb.h"
#include "DlgAprimoraNitidezCalculo1.h"





// CDlgAprimoraNitidezCalculo1 dialog

// IMPLEMENT_DYNAMIC(CDlgAprimoraNitidezCalculo1, CDialog)

CDlgAprimoraNitidezCalculo1::CDlgAprimoraNitidezCalculo1(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAprimoraNitidezCalculo1::IDD, pParent)
{
m_pView = NULL;
}

CDlgAprimoraNitidezCalculo1::CDlgAprimoraNitidezCalculo1(CView* pView, double *P1): CDialog(ID_APRIMORANITIDEZCALCULO1)
{
m_pView = pView;
m_p1 = P1;
}

BOOL CDlgAprimoraNitidezCalculo1::OnInitDialog()
{
*m_p1=1.0;
return CDialog::OnInitDialog();
}

BOOL CDlgAprimoraNitidezCalculo1::Create()
{
BOOL ret;
ret = CDialog::Create(ID_APRIMORANITIDEZCALCULO1);
return ret;
}

void CDlgAprimoraNitidezCalculo1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text (pDX, IDC_EDIT1, *m_p1);
	DDV_MinMaxDouble(pDX, *m_p1, 0.0, 50);
}


void CDlgAprimoraNitidezCalculo1::OnOK()
{
}

void CDlgAprimoraNitidezCalculo1::OnCancel()
{
	CDialog::OnCancel();
	if (m_pView !=NULL)
	{
		m_pView->SendMessage(ID_GOODBYE, IDCANCEL);
	}
}

void CDlgAprimoraNitidezCalculo1::OnClickedOK()
{
	CDialog::OnOK();
}

BEGIN_MESSAGE_MAP(CDlgAprimoraNitidezCalculo1, CDialog)

    ON_BN_CLICKED(ID_OK, OnClickedOK)
    ON_BN_CLICKED(ID_CANCEL, OnCancel)
    
END_MESSAGE_MAP()


// CDlgAprimoraNitidezCalculo1 message handlers

void CDlgAprimoraNitidezCalculo1::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlgAprimoraNitidezCalculo1::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}
