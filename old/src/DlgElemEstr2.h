//

/////////////////////////////////////////////////////////////////////////////
// CDlgElemEstr2 dialog


#ifndef _DlgElemEstr2_H_

#define _DlgElemEstr2_H_

#include <afxwin.h> 

#include "PDImagemDoc.h"

class CDlgElemEstr2 : public CDialog
{            

friend class CPDImagemView;

//
CPDIMorfoB* m_pImg; 
CPDIMorfoB* m_pImgf; 
CPDIMorfoB* p_impr; 
int mOpcao; 


// Construction    
public:     
    CView* m_pView;
    CString m_Flag; 
    CComboBox* pBox;
    CDlgElemEstr2(CWnd* pParent = NULL);    // standard constructor
    CDlgElemEstr2(CView* pView, CPDIMorfoB* pImg, CPDIMorfoB* pImgf,CString* Titulo);
    BOOL Create();    

// Dialog Data
    //{{AFX_DATA(CPDImaDlg)
    enum { IDD = IDD_ELEMESTR };
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

// Implementation


private:
    CBitmapButton m_cruz;
    CBitmapButton m_tres;
    CBitmapButton m_linhah;
    CBitmapButton m_linhav;
    CBitmapButton m_rombus;
    void OnClickedMascara();

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnOK(); 
    virtual void OnCancel();
     
    afx_msg void OnClickedCruz();
    afx_msg void OnClickedTres();
    afx_msg void OnClickedLinhaH();
    afx_msg void OnClickedLinhaV();  
    afx_msg void OnClickedRombus(); 
    afx_msg void OnClickedTipoOperacao();
    afx_msg void OnClickedOk();  
    afx_msg void OnDrawItem(int ident, LPDRAWITEMSTRUCT lpDIS);
    DECLARE_MESSAGE_MAP()
};

#endif // _DlgElemEstr2_H_