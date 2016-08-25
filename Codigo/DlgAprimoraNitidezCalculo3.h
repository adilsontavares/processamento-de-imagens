#ifndef _DlgAprimoraNitidezCalculo3_H_

#define _DlgAprimoraNitidezCalculo3_H_
#include <afxwin.h>
#include "resource.h"
#include "PDImagemDoc.h"


// CDlgAprimoraNitidezCalculo3 dialog

class CDlgAprimoraNitidezCalculo3 : public CDialog
{
	
	friend class CPDImagemView;


//	DECLARE_DYNAMIC(CDlgAprimoraNitidezCalculo1)

public:
	
	CView*  m_pView;
	double  *m_p3;

	CDlgAprimoraNitidezCalculo3(CWnd* pParent = NULL);   // standard constructor
	CDlgAprimoraNitidezCalculo3(CView* pView, double *P3);
	BOOL Create();
	
//	virtual ~CDlgAprimoraNitidezCalculo3();

// Dialog Data
	enum { IDD = ID_APRIMORANITIDEZCALCULO3 };

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
