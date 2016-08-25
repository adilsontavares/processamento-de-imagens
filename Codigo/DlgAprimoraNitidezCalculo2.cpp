// DlgAprimoraNitidezCalculo2.cpp : implementation file
//

#include "stdafx.h"
#ifndef _PDIMAGEM_H_
	#include "PDImagem.h" 
#endif

#include "PDIBase.h"
// #include "PDImagem.h"
#include "PDIMorfb.h"
#include "DlgAprimoraNitidezCalculo2.h"


// CDlgAprimoraNitidezCalculo2 dialog

// IMPLEMENT_DYNAMIC(CDlgAprimoraNitidezCalculo1, CDialog)

CDlgAprimoraNitidezCalculo2::CDlgAprimoraNitidezCalculo2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAprimoraNitidezCalculo2::IDD, pParent)
{
m_pView = NULL;
}

CDlgAprimoraNitidezCalculo2::CDlgAprimoraNitidezCalculo2(CView* pView, double *P2): CDialog(ID_APRIMORANITIDEZCALCULO2)
{
m_pView = pView;
m_p2 = P2;
}

BOOL CDlgAprimoraNitidezCalculo2::OnInitDialog()
{
*m_p2=1.0;
return CDialog::OnInitDialog();
}

BOOL CDlgAprimoraNitidezCalculo2::Create()
{
BOOL ret;
ret = CDialog::Create(ID_APRIMORANITIDEZCALCULO2);
return ret;
}

void CDlgAprimoraNitidezCalculo2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text (pDX, IDC_EDIT1, *m_p2);
	DDV_MinMaxDouble(pDX, *m_p2, 0.0, 40);
}


void CDlgAprimoraNitidezCalculo2::OnOK()
{
}

void CDlgAprimoraNitidezCalculo2::OnCancel()
{
	CDialog::OnCancel();
	if (m_pView !=NULL)
	{
		m_pView->SendMessage(ID_GOODBYE, IDCANCEL);
	}
}

void CDlgAprimoraNitidezCalculo2::OnClickedOK()
{
	CDialog::OnOK();
}

BEGIN_MESSAGE_MAP(CDlgAprimoraNitidezCalculo2, CDialog)

    ON_BN_CLICKED(IDOK, OnClickedOK)
    ON_BN_CLICKED(IDCANCEL, OnCancel)
    
END_MESSAGE_MAP()


// CDlgAprimoraNitidezCalculo2 message handlers

void CDlgAprimoraNitidezCalculo2::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlgAprimoraNitidezCalculo2::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}
