// DlgElemEstr2.cpp : implementation file
//

#include "stdafx.h" 
#ifndef __PDIMAGEM_H__
#include "PDImagem.h"
#endif

#include "PDIBase.h"
#include "PDIMorfb.h"
#include "DlgElemEstr2.h"
                   
extern CObList ObjectList;
extern POSITION pos;
                   
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgElemEstr2 dialog

CDlgElemEstr2::CDlgElemEstr2(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgElemEstr2::IDD, pParent)
{
    m_pView = NULL;
    //{{AFX_DATA_INIT(CDlgElemEstr2)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}
                                        
CDlgElemEstr2::CDlgElemEstr2(CView* pView, CPDIMorfoB* pImgf, CPDIMorfoB* pImg, CString* Titulo) 
    : CDialog(IDD_ELEMESTR)
{  
   m_Flag = *Titulo;
   m_pView = pView;
   m_pImgf = pImgf;
   m_pImg = pImg;
//   CWnd::SetWindowText(m_Titulo);
}   


BOOL CDlgElemEstr2::OnInitDialog()
{  
	p_impr = m_pImgf;
//	CWnd::SetWindowText(m_Titulo);

	VERIFY(m_cruz.AutoLoad(IDC_CRUZ, this));
	VERIFY(m_tres.AutoLoad(IDC_TRES, this));
	VERIFY(m_linhah.AutoLoad(IDC_LINHAH, this));
	VERIFY(m_linhav.AutoLoad(IDC_LINHAV, this));
	VERIFY(m_rombus.AutoLoad(IDC_ROMBUS, this));
	CheckRadioButton(IDC_RADIOCR,IDC_RADIORB,IDC_RADIOCR);
	CheckRadioButton(IDC_NORMAL,IDC_CONDICIONAL,IDC_NORMAL); 
	m_pImg->Condicional = FALSE;
	m_pImg->QualElem = 0;

	if (m_Flag == "NAOITERAR") 
	{
		GetDlgItem(IDC_ITERAR)->EnableWindow(FALSE);  
		GetDlgItem(IDC_TXITERAR)->EnableWindow(FALSE);  
		GetDlgItem(IDC_MASCARA)->EnableWindow(FALSE);  
		GetDlgItem(IDC_TXMASCARA)->EnableWindow(FALSE);  
		GetDlgItem(IDC_TXOPERACAO)->EnableWindow(FALSE);  
		GetDlgItem(IDC_NORMAL)->EnableWindow(FALSE);  
		GetDlgItem(IDC_CONDICIONAL)->EnableWindow(FALSE);  
	}
	pBox = (CComboBox*) GetDlgItem(IDC_MASCARA);
	CPDImagemDoc* pDocAux;

	GetDlgItem(IDC_MASCARA)->EnableWindow(FALSE);  
	GetDlgItem(IDC_TXMASCARA)->EnableWindow(FALSE);  

	for( pos = ObjectList.GetHeadPosition(); pos != NULL; )
	{
		pDocAux = (CPDImagemDoc*) ObjectList.GetNext( pos );
		pBox->InsertString(-1, pDocAux->GetTitle());
	} 

	pBox->SetCurSel(0);

	m_pImg->Iteracoes = 1;   
	return CDialog::OnInitDialog();
}
   
BOOL CDlgElemEstr2::Create()                
{  
   BOOL ret; 
   ret = CDialog::Create(IDD_ELEMESTR);
   return ret;
   
}
      
void CDlgElemEstr2::DoDataExchange(CDataExchange* pDX)
{

    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_ITERAR, m_pImg->Iteracoes);
    DDV_MinMaxInt(pDX, m_pImg->Iteracoes, 1, 99);
    //{{AFX_DATA_MAP(CDlgElemEstr2)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


void CDlgElemEstr2::OnOK()
{
   // Funcao ficticia - resposta a tecla <enter>    

}



void CDlgElemEstr2::OnCancel()
{
        
    CDialog::OnCancel();
    if (m_pView != NULL) {
        m_pView->SendMessage(ID_GOODBYE, IDCANCEL);
    }   
}

BEGIN_MESSAGE_MAP(CDlgElemEstr2, CDialog)

    ON_BN_CLICKED(IDC_RADIOCR, OnClickedCruz)
    ON_BN_CLICKED(IDC_RADIOQU, OnClickedTres)
    ON_BN_CLICKED(IDC_RADIOLH, OnClickedLinhaH)
    ON_BN_CLICKED(IDC_RADIOLV, OnClickedLinhaV)
    ON_BN_CLICKED(IDC_RADIORB, OnClickedRombus)
    ON_BN_CLICKED(IDC_NORMAL, OnClickedTipoOperacao)
    ON_BN_CLICKED(IDC_CONDICIONAL, OnClickedTipoOperacao)
    ON_BN_CLICKED(IDC_OK, OnClickedOk)
    
END_MESSAGE_MAP()

//////

void CDlgElemEstr2::OnClickedCruz()
{
   CheckRadioButton(IDC_RADIOCR,IDC_RADIORB,IDC_RADIOCR);
   m_pImg->QualElem = 0;
}


void CDlgElemEstr2::OnClickedLinhaH()
{
   CheckRadioButton(IDC_RADIOCR,IDC_RADIORB,IDC_RADIOLH);
   m_pImg->QualElem = 1;
}


void CDlgElemEstr2::OnClickedLinhaV()
{
   CheckRadioButton(IDC_RADIOCR,IDC_RADIORB,IDC_RADIOLV);
   m_pImg->QualElem = 2;
}


void CDlgElemEstr2::OnClickedTres()
{
   CheckRadioButton(IDC_RADIOCR,IDC_RADIORB,IDC_RADIOQU);
   m_pImg->QualElem = 3;
}


void CDlgElemEstr2::OnClickedRombus()
{
   CheckRadioButton(IDC_RADIOCR,IDC_RADIORB,IDC_RADIORB);
   m_pImg->QualElem = 4;
}                            

void CDlgElemEstr2::OnClickedTipoOperacao()
{    
   
   switch (GetCheckedRadioButton(IDC_NORMAL,IDC_CONDICIONAL))
   {
      case IDC_NORMAL: GetDlgItem(IDC_MASCARA)->EnableWindow(FALSE);
                       GetDlgItem(IDC_TXMASCARA)->EnableWindow(FALSE);  
                       m_pImg->Condicional = FALSE;
                       m_pImg->Mascara = NULL; 
                       break;
      case IDC_CONDICIONAL: GetDlgItem(IDC_MASCARA)->EnableWindow(TRUE);  
                            GetDlgItem(IDC_TXMASCARA)->EnableWindow(TRUE);  
                            m_pImg->Condicional = TRUE;
   }                        
}

void CDlgElemEstr2::OnClickedMascara()
{
     POSITION posicao;
     int ind;
     CPDImagemDoc* pDocAux;

     ind = pBox->GetCurSel();
     posicao = ObjectList.FindIndex(ind);
     pDocAux = (CPDImagemDoc*) ObjectList.GetAt(posicao);
     m_pImg->Mascara = &pDocAux->Imagem;
} 

void CDlgElemEstr2::OnClickedOk()
{
    CDialog::OnOK();

    if (m_pImg->Condicional)
        OnClickedMascara();

    if (m_pImg->Iteracoes > 0 && m_pImg->Iteracoes < 100) {      
       if (m_pView != NULL) {
           m_pView->SendMessage(ID_GOODBYE, IDOK);
       }   
    }    
}

void CDlgElemEstr2::OnDrawItem(int ident, LPDRAWITEMSTRUCT lPDIS)
{

}

/////////////////////////////////////////////////////////////////////////////
// CDlgElemEstr2 message handlers


