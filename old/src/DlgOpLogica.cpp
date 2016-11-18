// DlgOpLogica.cpp : implementation file
//

#include "stdafx.h"

#include "PDImagem.h"
#include "DlgOpLogica.h"
#include "PDImagemView.h"
 
extern CObList ObjectList;
extern POSITION pos;
extern BOOL Inserir;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgOpLogica dialog

CDlgOpLogica::CDlgOpLogica(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgOpLogica::IDD, pParent)
{
    m_pView = NULL;
    //{{AFX_DATA_INIT(CDlgOpLogica)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

CDlgOpLogica::CDlgOpLogica(CView* pView) 
    : CDialog(IDD_OPLOGICA)
{  
   m_pView = pView;
   pDocC = NULL;
}   
   

BOOL CDlgOpLogica::OnInitDialog()
{  
  
   pBoxA = (CComboBox*) GetDlgItem(IDC_IMAGEMA);
   pBoxB = (CComboBox*) GetDlgItem(IDC_IMAGEMB);
   CPDImagemDoc* pDocAux;
    
   for( pos = ObjectList.GetHeadPosition(); pos != NULL; )
    {
      pDocAux = (CPDImagemDoc*) ObjectList.GetNext( pos );
      pBoxA->InsertString(-1, pDocAux->GetTitle());
      pBoxB->InsertString(-1, pDocAux->GetTitle());
    } 
  
   pBoxA->SetCurSel(0);
   pBoxB->SetCurSel(0);  
   CheckDlgButton(IDC_OR,1);
   CButton* pButton = (CButton*) GetDlgItem(IDC_AREATES);
   int style = pButton->GetButtonStyle();
   pButton->SetButtonStyle(style,1);
   Visualizou = FALSE;
   return CDialog::OnInitDialog();
}
   
BOOL CDlgOpLogica::Create()                
{  
   BOOL ret; 
   ret = CDialog::Create(IDD_OPLOGICA);
   return ret;
}


void CDlgOpLogica::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgOpLogica)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgOpLogica, CDialog)
    //{{AFX_MSG_MAP(CDlgOpLogica)
    ON_BN_CLICKED(IDC_VISUALIZA, OnClickedOpcao)
    ON_WM_DRAWITEM()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOpLogica message handlers

void CDlgOpLogica::OnClickedOpcao()
{                  
	HBITMAP HImaA, HImaB, HImaC;
	CDC DcImaA, DcImaB, DcImaC;
	CDC* pDC = GetDC();
	DcImaA.CreateCompatibleDC(NULL);
	DcImaB.CreateCompatibleDC(NULL);   
	DcImaC.CreateCompatibleDC(NULL);   
	CPDImagemDoc *pDocA, *pDocB;
	POSITION posA, posB;
	int indA, indB, x,y;

	indA = pBoxA->GetCurSel();
	indB = pBoxB->GetCurSel();
	posA = ObjectList.FindIndex(indA);
	posB = ObjectList.FindIndex(indB);

	pDocA = (CPDImagemDoc*) ObjectList.GetAt(posA);
	pDocB = (CPDImagemDoc*) ObjectList.GetAt(posB);

	x = (int) pDocA->Imagem.GetWidth();
	y = (int) pDocA->Imagem.GetHeight();

	HImaA = CreateDIBitmap(pDC->m_hDC,pDocA->Imagem.lpBI,CBM_INIT,pDocA->Imagem.lpBits,pDocA->Imagem.lpbmi,DIB_COR);
	if((::SelectObject(DcImaA.m_hDC, HImaA)) == NULL) 
		return;

	HImaB = CreateDIBitmap(pDC->m_hDC,pDocB->Imagem.lpBI,CBM_INIT,pDocB->Imagem.lpBits,pDocB->Imagem.lpbmi,DIB_COR);
	if((::SelectObject(DcImaB.m_hDC, HImaB)) == NULL)
		return;

	HImaC = CreateDIBitmap(pDC->m_hDC,pDocB->Imagem.lpBI,CBM_INIT,pDocB->Imagem.lpBits,pDocB->Imagem.lpbmi,DIB_COR);
	if((::SelectObject(DcImaC.m_hDC, HImaC)) == NULL)
		return;

	int Opcao = GetCheckedRadioButton(IDC_OR,IDC_NOR);

	BitBlt  (DcImaC.m_hDC, 0, 0, x, y, DcImaA.m_hDC, 0,  0, COPIA);

	CString Titulo; 

	switch (Opcao) 
	{
		case IDC_OR:
			BitBlt  (DcImaC.m_hDC, 0, 0, x, y, DcImaB.m_hDC, 0,  0, (DWORD) OU);
			Titulo = "OR"; 
			break;  
		case IDC_AND:
			BitBlt  (DcImaC.m_hDC, 0, 0, x, y, DcImaB.m_hDC, 0,  0, (DWORD) E);
			Titulo = "AND";
			break;
		case IDC_XOR:
			BitBlt  (DcImaC.m_hDC, 0, 0, x, y, DcImaB.m_hDC, 0,  0, (DWORD) SUBTRAI);
			Titulo = "XOR"; 
			break; 
		case IDC_NAND:
			BitBlt  (DcImaC.m_hDC, 0, 0, x, y, DcImaB.m_hDC, 0,  0, (DWORD) E);
			BitBlt  (DcImaC.m_hDC, 0, 0, x, y, DcImaC.m_hDC, 0,  0, DSTINVERT);
			Titulo = "NAND"; 
			break; 
		case IDC_NOR: BitBlt  (DcImaC.m_hDC, 0, 0, x, y, DcImaB.m_hDC, 0,  0, (DWORD) OU);
			BitBlt  (DcImaC.m_hDC, 0, 0, x, y, DcImaC.m_hDC, 0,  0, DSTINVERT);
			Titulo = "NOR"; 
			break; 
	}                                                                          

	CDocTemplate* pTemplate = pDocA->GetDocTemplate();

	if (pDocC != NULL)
	{
		CDocTemplate* pTemplateC = pDocC->GetDocTemplate();
		pTemplateC->RemoveDocument(pDocC);
	}    

	Inserir = FALSE;
	pDocC = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
	Inserir = TRUE;
                   
	pDocC->Imagem.hDIB = pDocA->Imagem.CopyHandle(); 
	pDocC->Imagem.InitDIBData(); 
	pDocC->SetTitle(Titulo);


	GetDIBits(DcImaC.m_hDC,HImaC,0,(WORD) pDocC->Imagem.GetHeight(),pDocC->Imagem.lpBits,pDocC->Imagem.lpbmi,DIB_COR);
	Visualizou = TRUE;

	ReleaseDC(pDC);
	::ReleaseDC(NULL,DcImaA.m_hDC);
	DcImaA.DeleteDC();
	::ReleaseDC(NULL,DcImaB.m_hDC);
	DcImaB.DeleteDC();
	::ReleaseDC(NULL,DcImaC.m_hDC);
	DcImaC.DeleteDC();

	if (HImaA != NULL)
		DeleteObject(HImaA);
	if (HImaB != NULL)
		DeleteObject(HImaB);
	if (HImaC != NULL)
		DeleteObject(HImaC);

	GetDlgItem(IDC_AREATES)->Invalidate();
}

void CDlgOpLogica::OnCancel()
{
	if (pDocC != NULL)
	{
		delete pDocC;
		pDocC = NULL;
	}
	
	CDialog::OnCancel();
	if (m_pView != NULL)
		m_pView->SendMessage(ID_GOODBYE, IDCANCEL);
}

void CDlgOpLogica::OnOK()
{
	if (!Visualizou)
		OnClickedOpcao();

	CFrameWnd* pFrameO;
	POSITION pos = pDocC->GetFirstViewPosition();
	CPDImagemView* pvw = (CPDImagemView*) pDocC->GetNextView(pos);
	pFrameO = pvw->CreateChild(pDocC, pDocC);
	ObjectList.AddTail(pDocC);
	CDialog::OnOK();

	if (m_pView != NULL)
		m_pView->SendMessage(ID_GOODBYE, IDOK);
}

void CDlgOpLogica::OnDrawItem(int ident, LPDRAWITEMSTRUCT lpDIS)
{
	if (pDocC == NULL)
		return;

	// draw the whole button
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rc;
	rc.CopyRect(&lpDIS->rcItem);

	CPalette* pOldPal = NULL;

	if (pDocC->Imagem.IsPaletted())
	{
		pOldPal = pDC->SelectPalette(pDocC->Imagem.pal, TRUE);
		pDC->RealizePalette();         
	}

	int cxDIB = (int) pDocC->Imagem.GetWidth();         // Size of DIB - x
	int cyDIB = (int) pDocC->Imagem.GetHeight();        // Size of DIB - y
	CRect rcDIB;
	rcDIB.top = rcDIB.left = 0;
	rcDIB.right = cxDIB;
	rcDIB.bottom = cyDIB;
	CRect rcDest;

	rcDest = rc;
	pDocC->Imagem.PaintDIB(pDC->m_hDC, &rcDest, &rcDIB);

	if (pOldPal != NULL)
		pDC->SelectPalette(pOldPal, FALSE);
}
