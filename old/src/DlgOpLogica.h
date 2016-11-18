// DlgOpLogica.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOpLogica dialog

#ifndef _DlgOpLogica_H_

#define _DlgOpLogica_H_

#include "resource.h"
#include "PDImagemDoc.h"

#define E               0x00EE0086L
#define OU              0x008800C6L
#define COPIA           0x00CC0020L
#define SUBTRAI         0x00990066L

class CDlgOpLogica : public CDialog
{
// Construction
public: 
   
    CPDImagemDoc*	pDocC;
    BOOL			Visualizou; 
    CComboBox*		pBoxA, *pBoxB; 
    CView*			m_pView;

			CDlgOpLogica(CView* pView);
			CDlgOpLogica(CWnd* pParent = NULL);    // standard constructor
    BOOL	Create();    

// Dialog Data
    //{{AFX_DATA(CDlgOpLogica)
    enum { IDD = IDD_OPLOGICA };
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

// Implementation
protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CDlgOpLogica)
    afx_msg void OnClickedOpcao();
    virtual void OnCancel();
    virtual void OnOK();
    afx_msg void OnDrawItem(int ident, LPDRAWITEMSTRUCT lPDIS);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // _DlgOpLogica_H_