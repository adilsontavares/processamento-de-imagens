// AboutDlg.h
//

#ifndef _ABOUTDLG_H_

#define _ABOUTDLG_H_

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg
// See AboutDlg.cpp for the implementation of this class
//

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //{{AFX_MSG(CAboutDlg)
        // No message handlers
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif
/////////////////////////////////////////////////////////////////////////////