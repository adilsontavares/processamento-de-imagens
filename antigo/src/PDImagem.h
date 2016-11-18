// PDImagem.h : main header file for the PDImagem application
//

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#define WM_DOREALIZE   (WM_USER + 0)
#define DIB_COR	DIB_RGB_COLORS

/////////////////////////////////////////////////////////////////////////////
// CPDImagemApp:
// See PDImagem.cpp for the implementation of this class
//

class CPDImagemApp : public CWinApp
{
public:
    CPDImagemApp();
	~CPDImagemApp();
// Overrides
    virtual BOOL InitInstance();

// Implementation

    //{{AFX_MSG(CPDImagemApp)
    afx_msg void OnAppAbout();
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
