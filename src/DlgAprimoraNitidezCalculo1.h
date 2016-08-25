#ifndef _DlgAprimoraNitidezCalculo1_H_

#define _DlgAprimoraNitidezCalculo1_H_
#include <afxwin.h>
#include "resource.h"
#include "PDImagemDoc.h"


// CDlgAprimoraNitidezCalculo1 dialog

class CDlgAprimoraNitidezCalculo1 : public CDialog
{
	
	friend class CPDImagemView;


//	DECLARE_DYNAMIC(CDlgAprimoraNitidezCalculo1)

public:
	
	CView*  m_pView;
	double  *m_p1;

	CDlgAprimoraNitidezCalculo1(CWnd* pParent = NULL);   // standard constructor
	CDlgAprimoraNitidezCalculo1(CView* pView, double *P1);
	BOOL Create();
	
//	virtual ~CDlgAprimoraNitidezCalculo1();

// Dialog Data
	enum { IDD = ID_APRIMORANITIDEZCALCULO1 };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	virtual void OnClickedOK();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
#endif 