// DlgHistograma.h : header file
//

#ifndef _DlgHistograma_H_

#define _DlgHistograma_H_

/////////////////////////////////////////////////////////////////////////////
// CPdimaDlg dialog

#include "resource.h"
#include "PDIBase.h"

#include <afxwin.h> 

class CDlgHistograma : public CDialog
{            
//
CPDIBase* m_pImg; 
int mOpcao;

// Construction    
public:
    CView* m_pView;
    CString m_Titulo;
    DWORD rCount[256], gCount[256], bCount[256];
    BOOL mR,mG,mB;
    CDlgHistograma(CWnd* pParent = NULL);    // standard constructor
    CDlgHistograma(CView* pView, CPDIBase* pImg, CString* Titulo);
                      
    BOOL Create();    

// Dialog Data
    //{{AFX_DATA(CPdimaDlg)
    enum { IDD = IDD_HISTOGRAMA };
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

// Implementation
protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnOK(); 
 
    afx_msg void OnClickedAzul();
    afx_msg void OnClickedVerde();
    afx_msg void OnClickedVermelho();
    afx_msg void OnClickedRGB();
    afx_msg void OnClickedHSI();
    afx_msg void OnDrawItem(int ident, LPDRAWITEMSTRUCT lpDIS);

    DECLARE_MESSAGE_MAP()
};

#endif  // _DlgHistograma_H_