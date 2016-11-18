// pdidlghi.cpp : implementation file
//

#include "stdafx.h"
#include "PDImagem.h"
#include "PDIDlgHis.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPDIDlgHis dialog

CPDIDlgHis::CPDIDlgHis(CWnd* pParent /*=NULL*/)
	: CDialog(CPDIDlgHis::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPDIDlgHis)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CPDIDlgHis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPDIDlgHis)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPDIDlgHis, CDialog)
	//{{AFX_MSG_MAP(CPDIDlgHis)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPDIDlgHis message handlers
