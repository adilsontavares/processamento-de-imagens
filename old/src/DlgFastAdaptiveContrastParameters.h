#pragma once


// DlgFastAdaptiveContrastParameters dialog

class DlgFastAdaptiveContrastParameters : public CDialog
{
	DECLARE_DYNAMIC(DlgFastAdaptiveContrastParameters)

public:

	CView *m_pView;
	BYTE *m_pLimiarRuido;

	DlgFastAdaptiveContrastParameters(CWnd* pParent = NULL);   // standard constructor
	DlgFastAdaptiveContrastParameters(CView *pView, BYTE *pLimiarRuido);
	virtual ~DlgFastAdaptiveContrastParameters();

	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_FASTADAPTIVECONTRAST_LIMIARRUIDO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
