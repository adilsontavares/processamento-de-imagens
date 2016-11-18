#ifndef _DlgAprimoraNitidezCalculo2_H_

#define _DlgAprimoraNitidezCalculo2_H_
#include <afxwin.h>
#include "resource.h"
#include "PDImagemDoc.h"


// CDlgAprimoraNitidezCalculo2 dialog

class CDlgAprimoraNitidezCalculo2 : public CDialog
{
	
	friend class CPDImagemView;


//	DECLARE_DYNAMIC(CDlgAprimoraNitidezCalculo1)

public:
	
	CView*  m_pView;
	double  *m_p2;

	CDlgAprimoraNitidezCalculo2(CWnd* pParent = NULL);   // standard constructor
	CDlgAprimoraNitidezCalculo2(CView* pView, double *P2);
	BOOL Create();
	
//	virtual ~CDlgAprimoraNitidezCalculo1();

// Dialog Data
	enum { IDD = ID_APRIMORANITIDEZCALCULO2 };

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