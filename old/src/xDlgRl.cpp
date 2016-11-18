// DlgRL.cpp : implementation file
//

#include "stdafx.h"

#include "PDImagem.h"
#include "PDIDlgrl.h"
#include "PDImagemView.h"
 
extern CObList ObjectList;
extern POSITION pos;
extern BOOL Inserir;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define E               0x00EE0086L
#define OU              0x008800C6L
#define COPIA           0x00CC0020L
#define SUBTRAI         0x00990066L

/////////////////////////////////////////////////////////////////////////////
// CPDIDlgRl dialog

CPDIDlgRl::CPDIDlgRl(CWnd* pParent /*=NULL*/)
    : CDialog(CPDIDlgRl::IDD, pParent)
{
	m_pView = NULL;
	//{{AFX_DATA_INIT(CPDIDlgRl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPDIDlgRl::CPDIDlgRl(CView* pView) 
    : CDialog(IDD_DIALOG8)
{  
	m_pView = pView;
	pDocC = NULL;
}   
   

BOOL CPDIDlgRl::OnInitDialog()
{  
	pBoxA = (CComboBox*) GetDlgItem(IDC_IMAGEMRA);
	pBoxB = (CComboBox*) GetDlgItem(IDC_IMAGEMRB);
	CPDImagemDoc* pDocAux;

	for( pos = ObjectList.GetHeadPosition(); pos != NULL; )
	{
		pDocAux = (CPDImagemDoc*) ObjectList.GetNext( pos );
		pBoxA->InsertString(-1, pDocAux->GetTitle());
		pBoxB->InsertString(-1, pDocAux->GetTitle());
	} 

	pBoxA->SetCurSel(0);
	pBoxB->SetCurSel(0);   

	CButton* pButton = (CButton*) GetDlgItem(IDC_AREAREL);
	int style = pButton->GetButtonStyle();
	pButton->SetButtonStyle(style,1);
	Visualizou = FALSE;
	return CDialog::OnInitDialog();
}
   
BOOL CPDIDlgRl::Create()                
{  
	BOOL ret; 
	ret = CDialog::Create(IDD_DIALOG8);
	return ret;
}


void CPDIDlgRl::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPDIDlgRl)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPDIDlgRl, CDialog)
    //{{AFX_MSG_MAP(CPDIDlgRl)
    ON_BN_CLICKED(IDC_VISUALREL, OnClickedVisualRel)
    ON_WM_DRAWITEM()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPDIDlgRl message handlers

void CPDIDlgRl::OnClickedVisualRel()
{                  
	CPDImagemDoc *pDocA, *pDocB;
	POSITION posA, posB;
	int indA, indB, x,y, lin, col;
	double ImagemA, ImagemB;
	ImagemA = ImagemB = 0.0;
	CString Aux;
	char v6[10], v10[20];

	HCURSOR OldCursor; 
	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	indA = pBoxA->GetCurSel();
	indB = pBoxB->GetCurSel();
	posA = ObjectList.FindIndex(indA);
	posB = ObjectList.FindIndex(indB);

	pDocA = (CPDImagemDoc*) ObjectList.GetAt(posA);
	pDocB = (CPDImagemDoc*) ObjectList.GetAt(posB);

	x = (int) pDocA->Imagem.GetWidth();
	y = (int) pDocA->Imagem.GetHeight();

	CDocTemplate* pTemplate = pDocA->GetDocTemplate();

	if (pDocC != NULL)
	{
		CDocTemplate* pTemplateC = pDocC->GetDocTemplate();
		pTemplateC->RemoveDocument(pDocC);
	}    

	Inserir = FALSE;
	pDocC = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
	Inserir = TRUE;

	CString Titulo = "Relacao";                                     
	pDocC->Imagem.hDIB = pDocA->Imagem.CopyHandle(); 
	pDocC->Imagem.InitDIBData(); 
	pDocC->SetTitle(Titulo);
                      
	BYTE * lpTempA;
	BYTE * lpTempB;
	BYTE * lpTempC;
                                    
	lpTempA = pDocA->Imagem.lpBits;                
	lpTempB = pDocB->Imagem.lpBits;                
	lpTempC = pDocC->Imagem.lpBits;                

	UINT PRETO;
	if (IsDlgButtonChecked(IDC_MANTER_A) == 1)
	PRETO = pDocC->Imagem.GetNearestPaletteIndex(RGB(0,0,0));
	else
	pDocC->Imagem.GrayPalette();

	for (lin = 0; lin < (int) y; lin++, lpTempA += pDocA->Imagem.bmWidthBytes, 
							  lpTempB += pDocB->Imagem.bmWidthBytes,
							  lpTempC += pDocC->Imagem.bmWidthBytes) 
	{
		for (col = 0; col < (int) x; col++)
		{
			if (lpTempA[col] == 0)
				ImagemA++;
			if (lpTempB[col] == 0)
				ImagemB++;

			if (IsDlgButtonChecked(IDC_MANTER_A) == 1)
			{
				if (lpTempB[col] == 0)
					lpTempC[col] = PRETO;
				else
					lpTempC[col] = lpTempA[col];    
			}
			else 
			{
				lpTempC[col] = 255;
				if (lpTempB[col] == 0 && lpTempA[col] == 0)
					lpTempC[col] = 0;
				else
					if (lpTempA[col] == 0)
						lpTempC[col] = 200;
					else
						if (lpTempB[col] == 0)
							lpTempC[col] = 0;
			}
		}
	}

	if (IsDlgButtonChecked(IDC_MANTER_A) == 0)
	{
		sprintf(v10,"%ld",(long)x*y);
		Aux = v10;
		SetDlgItemText(IDC_MCO, Aux);

		sprintf(v10,"%.0f",ImagemA);
		Aux = v10;
		SetDlgItemText(IDC_CSE, Aux);

		sprintf(v10,"%.0f",ImagemB);
		Aux = v10;
		SetDlgItemText(IDC_MUC, Aux);

		sprintf(v6,"%5.2f",((ImagemB / ((long) x*y))*100.0));
		Aux = v6;
		Aux = Aux + "%" ;
		SetDlgItemText(IDC_MUCMCO, Aux);

		sprintf(v6,"%5.2f",(ImagemB/ImagemA*100.0));
		Aux = v6;
		Aux = Aux + "%" ;
		SetDlgItemText(IDC_MUCCSE, Aux);
	}

	SetCursor(OldCursor);
	Visualizou = TRUE;
	GetDlgItem(IDC_AREAREL)->Invalidate();
}

void CPDIDlgRl::OnCancel()
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

void CPDIDlgRl::OnOK()
{
	if (!Visualizou)
		OnClickedVisualRel();

	CFrameWnd* pFrameO;
	POSITION pos = pDocC->GetFirstViewPosition();
	CPDImagemView* pvw = (CPDImagemView*) pDocC->GetNextView(pos);
	pFrameO = pvw->CreateChild(pDocC, pDocC);
	ObjectList.AddTail(pDocC);
	
	CDialog::OnOK();

	if (m_pView != NULL)
		m_pView->SendMessage(ID_GOODBYE, IDOK);
}

void CPDIDlgRl::OnDrawItem(int ident, LPDRAWITEMSTRUCT lpDIS)
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