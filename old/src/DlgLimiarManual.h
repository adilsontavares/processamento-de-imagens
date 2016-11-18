// DlgLimiarManual.h : header file                                                                
//

#ifndef _DlgLimiarManual_H_

#define _DlgLimiarManual_H_

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgLimiarManual dialog

#include <afxwin.h> 

class CDlgLimiarManual : public CDialog
{            
//
CPDIBase* m_pImg; 
CPDIBase* m_pImgf; 
CPDIBase* p_impr; 
int mOpcao;

// Construction    
public:
    CView* m_pView;
    CString m_Titulo;
    CDlgLimiarManual(CWnd* pParent = NULL);    // standard constructor
    CDlgLimiarManual(CView* pView, CPDIBase* pImg, CPDIBase* pImgf,CString* Titulo);
    BOOL Create();    

// Dialog Data
    //{{AFX_DATA(CDlgLimiarManual)
    enum { IDD = IDD_LIMIARMANUAL };
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

// Implementation
protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnOK(); 
    virtual void OnCancel(); 
 
    afx_msg void OnClickedVerifica();
    afx_msg void OnDrawItem(int ident, LPDRAWITEMSTRUCT lpDIS);

    DECLARE_MESSAGE_MAP()
};

#endif // _DlgLimiarManual_H_