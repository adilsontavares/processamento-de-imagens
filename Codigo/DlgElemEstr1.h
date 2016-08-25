// DlgElemEstr1.h : header file
//

#ifndef _DlgElemEstr1_H_

#define _DlgElemEstr1_H_

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgElemEstr1 dialog

class CDlgElemEstr1 : public CDialog
{
// Construction
public:
	CDlgElemEstr1(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgElemEstr1)
	enum { IDD = IDD_ELEMESTR };
	int		m_iterar;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDlgElemEstr1)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // _DlgElemEstr1_H_