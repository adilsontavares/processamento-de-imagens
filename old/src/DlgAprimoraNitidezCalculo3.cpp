// DlgAprimoraNitidezCalculo3.cpp : implementation file
//

#include "stdafx.h"
#ifndef _PDIMAGEM_H_
	#include "PDImagem.h" 
#endif

#include "PDIBase.h"
// #include "PDImagem.h"
#include "PDIMorfb.h"
#include "DlgAprimoraNitidezCalculo3.h"


// CDlgAprimoraNitidezCalculo3 dialog

// IMPLEMENT_DYNAMIC(CDlgAprimoraNitidezCalculo1, CDialog)

CDlgAprimoraNitidezCalculo3::CDlgAprimoraNitidezCalculo3(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAprimoraNitidezCalculo3::IDD, pParent)
{
m_pView = NULL;
}

CDlgAprimoraNitidezCalculo3::CDlgAprimoraNitidezCalculo3(CView* pView, double *P3): CDialog(ID_APRIMORANITIDEZCALCULO3)
{
m_pView = pView;
m_p3 = P3;
}

BOOL CDlgAprimoraNitidezCalculo3::OnInitDialog()
{
*m_p3=10;
return CDialog::OnInitDialog();
}

BOOL CDlgAprimoraNitidezCalculo3::Create()
{
BOOL ret;
ret = CDialog::Create(ID_APRIMORANITIDEZCALCULO3);
return ret;
}

void CDlgAprimoraNitidezCalculo3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text (pDX, IDC_EDIT1, *m_p3);
	DDV_MinMaxDouble(pDX, *m_p3, 0.0, 50);
}


void CDlgAprimoraNitidezCalculo3::OnOK()
{
}

void CDlgAprimoraNitidezCalculo3::OnCancel()
{
	CDialog::OnCancel();
	if (m_pView !=NULL)
	{
		m_pView->SendMessage(ID_GOODBYE, IDCANCEL);
	}
}

void CDlgAprimoraNitidezCalculo3::OnClickedOK()
{
	CDialog::OnOK();
}

BEGIN_MESSAGE_MAP(CDlgAprimoraNitidezCalculo3, CDialog)

    ON_BN_CLICKED(IDOK, OnClickedOK)
    ON_BN_CLICKED(IDCANCEL, OnCancel)
    
END_MESSAGE_MAP()


// CDlgAprimoraNitidezCalculo3 message handlers

void CDlgAprimoraNitidezCalculo3::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlgAprimoraNitidezCalculo3::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}
