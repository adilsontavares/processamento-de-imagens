// PDIDlgRl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPDIDlgRl dialog

#ifndef _PDIDLGRL_H_

#define _PDIDLGRL_H_

#include "resource.h"
#include "PDImagemDoc.h"

class CPDIDlgRl : public CDialog
{
// Construction
public: 
   
    CPDImagemDoc* pDocC;
    BOOL Visualizou; 
    CComboBox* pBoxA, *pBoxB; 
    CView* m_pView;
    CPDIDlgRl(CView* pView);
    CPDIDlgRl(CWnd* pParent = NULL);    // standard constructor
    BOOL Create();    

// Dialog Data
    //{{AFX_DATA(CPDIDlgRl)
    enum { IDD = IDD_DIALOG8 };
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

// Implementation
protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CPDIDlgRl)
    afx_msg void OnClickedVisualRel();
    virtual void OnCancel();
    virtual void OnOK();
    afx_msg void OnDrawItem(int ident, LPDRAWITEMSTRUCT lpDIS);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // _PDIDLGRL_H_
