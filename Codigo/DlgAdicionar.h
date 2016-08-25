#pragma once


// CDlgAdicionar dialog

class CDlgAdicionar : public CDialog
{
	DECLARE_DYNAMIC(CDlgAdicionar)

public:
	
	CView *m_pView;
	BYTE *m_pValor;

	CDlgAdicionar(CWnd* pParent = NULL);   // standard constructor
	CDlgAdicionar(CView *pView, BYTE *pValor);
	virtual ~CDlgAdicionar();

	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_ADICIONAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
