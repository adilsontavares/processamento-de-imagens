
/////////////////////////////////////////////////////////////////////////////
// CPDImaDlg dialog

#ifndef _DlgFatorZoom_H_

#define _DlgFatorZoom_H_

#include <afxwin.h> 

#include "resource.h"
#include "PDImagemDoc.h"

class CDlgFatorZoom : public CDialog
{            

friend class CPDImagemView;

// Construction    
public:     
    CView*	m_pView;//é como se fosse uma janela do sistema
	CString m_Titulo;//é o título que vai na janela
    double	*m_pH, *m_pL;//são os fatores de ampliação da imagem

    CDlgFatorZoom(CWnd* pParent = NULL);    // standard constructor
    CDlgFatorZoom(CView* pView, double *pH, double *pL);
    BOOL Create();    

// Dialog Data
    //{{AFX_DATA(CDlgFatorZoom)
    enum { IDD = IDD_FATORZOOM };
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

// Implementation


protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnOK(); 
    virtual void OnCancel();
    afx_msg void OnClickedOk();  
    DECLARE_MESSAGE_MAP()
};

#endif // _DlgFatorZoom_H_

