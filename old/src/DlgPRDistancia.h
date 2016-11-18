// DlgPRDistancia.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPRDistancia dialog
#ifndef _DlgPRDistancia_H_

#define _DlgPRDistancia_H_

#include "resource.h"
#include "PDIBase.h"

class CDlgPRDistancia : public CDialog
{
	// Construction
public:                                          
    CDlgPRDistancia(CPDIBase*	, 
					CPDIBase*	,
					CString		,
					CString		,
					CString		);

// Dialog Data
    //{{AFX_DATA(CDlgPRDistancia)
    enum { IDD = IDD_PR_DISTANCIA };
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA
private:
	CPDIBase*	m_poImagemPadrao	;
	CPDIBase*	m_poImagemTeste		;
	CString		m_sTitleImagemPadrao;
	CString		m_sTitleImagemTeste	;
	CString		m_sDistancia		; 


// Implementation
protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CDlgPRDistancia)
    virtual void OnOK(); 
    afx_msg void OnDrawItem(int ident, LPDRAWITEMSTRUCT lpDIS);

    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // _DlgPRDistancia_H_
