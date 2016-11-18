// DlgElemEstr1.cpp : implementation file
//

#include "stdafx.h"

#include "DlgElemEstr1.h"

#include "PDImagem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgElemEstr1 dialog

CDlgElemEstr1::CDlgElemEstr1(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgElemEstr1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgElemEstr1)
	m_iterar = 0;
	//}}AFX_DATA_INIT
}

void CDlgElemEstr1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgElemEstr1)
	DDX_Text(pDX, IDC_ITERAR, m_iterar);
	DDV_MinMaxInt(pDX, m_iterar, 1, 99);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgElemEstr1, CDialog)
	//{{AFX_MSG_MAP(CDlgElemEstr1)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgElemEstr1 message handlers
