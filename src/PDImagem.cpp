// pdimagem.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "pdimagem.h"
#include <afxtempl.h>

#include "PDImagemDoc.h"
#include "PDImagemView.h"

#include "Mainfrm.h"

#include "AboutDlg.h"

CPtrList	ListaEstudo;
CObList		ObjectList;
POSITION	pos;
BOOL Inserir = TRUE;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPDImagemApp

BEGIN_MESSAGE_MAP(CPDImagemApp, CWinApp)
    //{{AFX_MSG_MAP(CPDImagemApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG_MAP
    // Standard file based document commands
//    ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
    // Standard print setup command
    ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
    // Global help commands
    ON_COMMAND(ID_HELP_INDEX, CWinApp::OnHelpIndex)
    ON_COMMAND(ID_HELP_USING, CWinApp::OnHelpUsing)
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
    ON_COMMAND(ID_CONTEXT_HELP, CWinApp::OnContextHelp)
    ON_COMMAND(ID_DEFAULT_HELP, CWinApp::OnHelpIndex)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPDImagemApp construction

CPDImagemApp::CPDImagemApp()
{
}

CPDImagemApp::~CPDImagemApp()
{
}


/////////////////////////////////////////////////////////////////////////////
// The one and only CPDImagemApp object

CPDImagemApp NEAR theApp;

/////////////////////////////////////////////////////////////////////////////
// CPDImagemApp initialization

BOOL CPDImagemApp::InitInstance()
{
    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

    SetDialogBkColor();        // set dialog background color to gray
    LoadStdProfileSettings();  // Load standard INI file options (including MRU)

    // Register the application's document templates.  Document templates
    //  serve as the connection between documents, frame windows and views.

    AddDocTemplate(new CMultiDocTemplate(IDR_PDIMAGTYPE,
            RUNTIME_CLASS(CPDImagemDoc),
            RUNTIME_CLASS(CMDIChildWnd),        // standard MDI child frame
            RUNTIME_CLASS(CPDImagemView)));

    // create main MDI Frame window
    CMainFrame* pMainFrame = new CMainFrame;
    if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
        return FALSE;

	// The main window has been initialized, so show and update it.
	m_nCmdShow = SW_SHOWMAXIMIZED;
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

    m_pMainWnd = pMainFrame;

    return TRUE;
}

// App command to run the dialog
void CPDImagemApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CPDImagemApp commands