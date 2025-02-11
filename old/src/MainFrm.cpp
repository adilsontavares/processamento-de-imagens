// mainfrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "pdimagem.h"

#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)

    //{{AFX_MSG_MAP(CMainFrame)
    ON_WM_CREATE()
    ON_WM_PALETTECHANGED()
    ON_WM_QUERYNEWPALETTE()

        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars

// toolbar buttons - IDs are command buttons
static UINT BASED_CODE buttons[] =
{
    // same order as in the bitmap 'toolbar.bmp'
    ID_FILE_NEW,
    ID_FILE_OPEN,
    ID_FILE_SAVE,
        ID_SEPARATOR,
    ID_EDIT_CUT,
    ID_EDIT_COPY,
    ID_EDIT_PASTE,
        ID_SEPARATOR,
    ID_FILE_PRINT,
    ID_APP_ABOUT,
    ID_CONTEXT_HELP,
};

static UINT BASED_CODE indicators[] =
{
    ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
    // TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (!m_wndToolBar.Create(this) ||
        !m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
        !m_wndToolBar.SetButtons(buttons,
          sizeof(buttons)/sizeof(UINT)))
    {
        TRACE("Failed to create toolbar\n");
        return -1;      // fail to create
    }

    if (!m_wndStatusBar.Create(this) ||
        !m_wndStatusBar.SetIndicators(indicators,
          sizeof(indicators)/sizeof(UINT)))
    {
        TRACE("Failed to create status bar\n");
        return -1;      // fail to create
    } 
    
    return 0;
}


void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd)
{
    CMDIFrameWnd::OnPaletteChanged(pFocusWnd);

    // always realize the palette for the active view
    CMDIChildWnd* pMDIChildWnd = MDIGetActive();
    if (pMDIChildWnd == NULL)
        return; // no active MDI child frame
    CView* pView = pMDIChildWnd->GetActiveView();
    ASSERT(pView != NULL);

    // notify all child windows that the palette has changed
    SendMessageToDescendants(WM_DOREALIZE, (WPARAM)pView->m_hWnd);
}


BOOL CMainFrame::OnQueryNewPalette()
{
    // always realize the palette for the active view
    CMDIChildWnd* pMDIChildWnd = MDIGetActive();
    if (pMDIChildWnd == NULL)
        return FALSE; // no active MDI child frame (no new palette)
    CView* pView = pMDIChildWnd->GetActiveView();
    ASSERT(pView != NULL);

    // just notify the target view
    pView->SendMessage(WM_DOREALIZE, (WPARAM)pView->m_hWnd);
    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
