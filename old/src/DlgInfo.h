// DlgInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInfo dialog
#ifndef _DlgInfo_H_

#define _DlgInfo_H_

#include "resource.h"
#include "PDImagemDoc.h"

class CDlgInfo : public CDialog
{
// Construction
public:                                          
    CPDImagemDoc* m_pDoc;
    CView* m_pView;
    CDlgInfo(CWnd* pParent = NULL);   // standard constructor
    CDlgInfo(CView* pView, CPDImagemDoc* pDoc);

// Dialog Data
    //{{AFX_DATA(CDlgInfo)
    enum { IDD = IDD_INFO };
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

// Implementation
protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CDlgInfo)
    virtual void OnOK(); 
    afx_msg void OnDrawItem(int ident, LPDRAWITEMSTRUCT lpDIS);

    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // _DlgInfo_H_
