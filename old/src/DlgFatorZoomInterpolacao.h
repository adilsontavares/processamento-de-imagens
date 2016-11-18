#pragma once

#include <afxwin.h> 

#include "resource.h"
#include "PDImagemDoc.h"


// CDlgFatorZoomInterpolacao dialog

class CDlgFatorZoomInterpolacao : public CDialog
{
	friend class CPDImagemView;
	DECLARE_DYNAMIC(CDlgFatorZoomInterpolacao)

public:
	CView*	m_pView;//é como se fosse uma janela do sistema
	CString m_Titulo;//é o título que vai na janela
    double	*m_pFator;//são os fatores de ampliação da imagem

	CDlgFatorZoomInterpolacao(CWnd* pParent = NULL);   // standard constructor
	CDlgFatorZoomInterpolacao(CView* pView, double *pFator);
	BOOL Create();    
	virtual ~CDlgFatorZoomInterpolacao();

// Dialog Data
	enum { IDD = IDD_FATORZOOM_INTERP };

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnOK(); 
    virtual void OnCancel();
    afx_msg void OnClickedOk();  
    DECLARE_MESSAGE_MAP()
};