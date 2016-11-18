// PDIDlgHis.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPDIDlgHis dialog

class CPDIDlgHis : public CDialog
{
// Construction
public:
	CPDIDlgHis(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPDIDlgHis)
	enum { IDD = IDD_HISTOGRAMA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CPDIDlgHis)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
