// PDImagemView.cpp : implementation of the CPDImagemView class
//

#include "stdafx.h"

#include "PDImagemView.h"

#include "MainFrm.h"
#include "PDImagem.h"
#include "PDImagemDoc.h"
#include "PDIBase.h"  

#include <math.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include  <time.h>
#include<string>
    
extern CPtrList	ListaEstudo;
extern CObList	ObjectList;
extern POSITION pos;

extern BOOL	Inserir; 

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define cubo(x) (x*x*x)

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CPDImagemView
// Controle de mensagens menu para chamada de funcoes 

IMPLEMENT_DYNCREATE(CPDImagemView, CScrollView)
                                                         
BEGIN_MESSAGE_MAP(CPDImagemView, CScrollView)
	//{{AFX_MSG_MAP(CPDImagemView)
	ON_BN_CLICKED(ID_MOSTRA, OnMostra)
	//}}AFX_MSG_MAP
	// Standard printing commands 
	ON_MESSAGE(WM_DOREALIZE, OnDoRealize)
	ON_MESSAGE(ID_GOODBYE, OnGoodBye)

	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

	ON_COMMAND(ID_IMAGEM_HISTOGRAMA, OnImagemHistograma)

	ON_COMMAND(ID_IMAGEM_EQUALIZACAO, OnImagemEqualizacao)
	ON_COMMAND(ID_IMAGEM_EQUALIZACAO_YIQ, OnImagemEqualizacaoYIQ)
	ON_COMMAND(ID_YIQ_APRIMORAMENTODENITIDEZ, OnAprimoramentoNitidez)

	ON_COMMAND(ID_IMAGEM_LIMIARIZACAO_OTSU, OnImagemLimiarizacaoOtsu)
	ON_COMMAND(ID_IMAGEM_LIMIARIZACAO_KITTLER, OnImagemLimiarizacaoKittler)
	ON_COMMAND(ID_IMAGEM_LIMIARIZACAO_ENTROPIA, OnImagemLimiarizacaoEntropia)
	ON_COMMAND(ID_IMAGEM_LIMIARIZACAO_ANISOTROPIA, OnImagemLimiarizacaoAnisotropia)
	ON_COMMAND(ID_IMAGEM_LIMIARIZACAO_MANUAL, OnImagemLimiarizacaoManual)

	ON_COMMAND(ID_MORFOLOGIA_BINARIA_EROSAO, OnMorfologiaBinariaErosao)
	ON_COMMAND(ID_MORFOLOGIA_BINARIA_DILATACAO, OnMorfologiaBinariaDilatacao)
	ON_COMMAND(ID_MORFOLOGIA_BINARIA_ABERTURA, OnMorfologiaBinariaAbertura)
	ON_COMMAND(ID_MORFOLOGIA_BINARIA_FECHAMENTO, OnMorfologiaBinariaFechamento)
	ON_COMMAND(ID_MORFOLOGIA_BINARIA_BORDAS_EROSAO, OnMorfologiaBinariaBordasErosao)
	ON_COMMAND(ID_MORFOLOGIA_BINARIA_BORDAS_DILATACAO, OnMorfologiaBinariaBordasDilatacao)
	ON_COMMAND(ID_MORFOLOGIA_BINARIA_BORDAS_DILERO, OnMorfologiaBinariaBordasDilEro)
	ON_COMMAND(ID_MORFOLOGIA_BINARIA_ESQUELETIZACAO, OnMorfologiaBinariaEsqueletizacao)
	ON_COMMAND(ID_MORFOLOGIA_BINARIA_ULTIMAEROSAO, OnMorfologiaBinariaUltimaErosao)

	ON_COMMAND(ID_OPERACOES_LOGICAS, OnOperacoesLogicas)
	ON_COMMAND(ID_IMAGEM_ZOOM, OnImagemZoom)
	ON_COMMAND(ID_IMAGEM_INVERTE, OnImagemInverte)
	// Conversão RGB -> HSV -> RGB
	ON_COMMAND(ID_HSV_EXTRAIR_CANAIS, OnHsvExtrairCanais)
	ON_COMMAND(ID_HSV_EXTRAIR_CANAIS_CARDANI, OnHsvExtrairCanais_Cardani)
	ON_COMMAND(ID_ESPAOSDECOR_HSI_CONVERSORGBPARAHSIHSIPARARGB, OnHSI)
	ON_COMMAND(ID_ESPACOSDECOR_XYZ, OnRGBtoXYZ)
	ON_COMMAND(ID_ESPAOSDECOR_YCRCB_CONVERTER_RGB_YCRCB, OnRGBtoYCrCb)
	ON_COMMAND(ID_ESPACOSDECOR_YIQ, OnYiq)
	ON_COMMAND(ID_APRIMORANITIDEZCALCULO1, OnAprimoraNitidezCalculo1)
	ON_COMMAND(ID_APRIMORANITIDEZCALCULO2, OnAprimoraNitidezCalculo2)
	ON_COMMAND(ID_APRIMORANITIDEZCALCULO3, OnAprimoraNitidezCalculo3)
	ON_COMMAND(ID_YCRCBEQUALIZACAO, OnImagemEqualizacaoYCrCb)

	// Novos comandos
	ON_COMMAND(ID_ADILSONTAVARES_ADICIONAR, OnAdiciona)
	ON_COMMAND(ID_ADILSONTAVARES_FASTADAPTIVECONTRAST, OnFastAdaptiveContrast)

    ON_WM_LBUTTONDOWN	()
    ON_WM_LBUTTONUP		()
    ON_WM_LBUTTONDBLCLK	()
    ON_WM_RBUTTONDOWN	()
    ON_WM_MOUSEMOVE		()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPDImagemView construction/destruction
CPDImagemView::CPDImagemView() 
{                                    
	m_pDlgElemEstr1		= NULL;
	m_pDlgElemEstr2		= NULL;
	m_pDlgFatorZoom		= NULL;
	m_pDlgHistograma	= NULL;
	m_pDlgLimiarManual	= NULL;
	m_pDlgOpLogica		= NULL;
	m_pDlgAprimoraNitidezCalculo1 = NULL;
	m_pDlgAprimoraNitidezCalculo2 = NULL;
	m_pDlgAprimoraNitidezCalculo3 = NULL;
}

// Destrutor da classe CPDImagemView                               
CPDImagemView::~CPDImagemView()
{
}

// Funcao para visualizacao de imagens
void CPDImagemView::OnDraw(CDC* pDC)
{
	CPalette*		pOldPal = NULL;
	CPDImagemDoc*	pDocIn	= GetDocument();  

	if (pDocIn->Imagem.IsPaletted())
	{
		pOldPal = pDC->SelectPalette(pDocIn->Imagem.pal, TRUE);
		pDC->RealizePalette();         
	}   

	if (pDocIn->Imagem.hDIB != NULL)
	{
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDocIn->Imagem.hDIB);
		int cxDIB = (int) pDocIn->Imagem.GetWidth();         // Size of DIB - x
		int cyDIB = (int) pDocIn->Imagem.GetHeight();        // Size of DIB - y
		::GlobalUnlock((HGLOBAL) pDocIn->Imagem.hDIB);               
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		CRect rcDest;
		if (pDC->IsPrinting())   // printer DC
		{
			// get size of printer page (in pixels)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			// get printer pixels per inch
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);

			//
			// Best Fit case -- create a rectangle which preserves
			// the DIB's aspect ratio, and fills the page horizontally.
			//
			// The formula in the "->bottom" field below calculates the Y
			// position of the printed bitmap, based on the size of the
			// bitmap, the width of the page, and the relative size of
			// a printed pixel (cyInch / cxInch).
			//
			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch)
				/ ((double)cxDIB * cxInch));
			rcDest.right = cxPage;
		}
		else   // not printer DC
		{
			rcDest = rcDIB;
		}
		pDocIn->Imagem.PaintDIB(pDC->m_hDC, &rcDest, &rcDIB);
	}

	if (pDocIn->Imagem.Region) 
	{ 
		oldROP2 = pDC->GetROP2();
		pDC->SetROP2(R2_NOTXORPEN);
		BlackBrush = new CBrush(RGB(0,0,0));
		OldBrush = (CBrush*) pDC->SelectObject(BlackBrush);
		pDocIn->Imagem.pRgn = &pDocIn->Imagem.rgn;
		pDocIn->Imagem.rgn.SetRectRgn(pDocIn->Imagem.rect); 
		pDC->FrameRgn(pDocIn->Imagem.pRgn,BlackBrush,2,2);
		pDC->SelectObject(OldBrush);
		delete BlackBrush;
		pDC->SetROP2(oldROP2);
	}                     

	if (pOldPal != NULL)
		pDC->SelectPalette(pOldPal, FALSE);
}

// Calcula o histograma da imagem
void CPDImagemView::OnImagemHistograma()
{
	CPDImagemDoc*	pDocIn = GetDocument();
	CString			Titulo;

	Titulo = pDocIn->GetTitle(); 

	m_pDlgHistograma = new CDlgHistograma(this, &pDocIn->Imagem, &Titulo); 

	if(m_pDlgHistograma->GetSafeHwnd() == 0)
		m_pDlgHistograma->Create();
}  


// Equaliza a imagem
void CPDImagemView::OnImagemEqualizacao()
{
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pdE = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pdE->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pdE->Imagem.InitDIBData(); 
	pdE->Imagem.GrayPalette();
	pdE->Imagem.Equalizacao();
	SetCursor(OldCursor);

	TitleWindow = "Equalizacao"; 
	pdE->SetTitle(TitleWindow);
	CFrameWnd* pFrameE;
	pFrameE = CreateChild(pdE, pdE);
}

// Binarizacao por Otsu
void CPDImagemView::OnImagemLimiarizacaoOtsu()
{
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette();
	pDocOut->Imagem.LimiarOtsu();
	SetCursor(OldCursor);

	TitleWindow =  "Limiarizacao - Otsu"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}


// Binarizacao por Kittler
void CPDImagemView::OnImagemLimiarizacaoKittler()
{
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette(); 
	pDocOut->Imagem.LimiarKittler();
	SetCursor(OldCursor);

	TitleWindow = "Limiarizacao - Kittler"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

// Binarizacao por Entropia
void CPDImagemView::OnImagemLimiarizacaoEntropia()
{
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette(); 
	pDocOut->Imagem.LimiarEntropia();
	SetCursor(OldCursor);

	TitleWindow = "Limiarizacao - Entropia"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

// Binarizacao por Anisotropia
void CPDImagemView::OnImagemLimiarizacaoAnisotropia()
{
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette(); 
	pDocOut->Imagem.LimiarAnisotropia();
	SetCursor(OldCursor);

	TitleWindow = "Limiarizacao - Anisotropia"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}


// Binarizacao Manual
void CPDImagemView::OnImagemLimiarizacaoManual()
{
    CString TitleWindow;
    HCURSOR OldCursor;
    CString Titulo;  
    CPDImagemDoc* pDocIn = GetDocument();  
    
    Titulo = pDocIn->GetTitle(); 
    
    OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

    Inserir = FALSE;

    CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
    CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
    Inserir = TRUE;                                       
    pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
    pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette(); 
    
    OnImagemHistograma();

    m_pDlgLimiarManual = new CDlgLimiarManual(this, &pDocIn->Imagem, &pDocOut->Imagem, &Titulo); 
    
	if (m_pDlgLimiarManual->DoModal() == IDCANCEL) 
		return;

	TitleWindow = "Limiarizacao - Manual"; 
    pDocOut->SetTitle(TitleWindow);
    CFrameWnd* pFrameO;
    pFrameO = CreateChild(pDocOut, pDocOut);
    ObjectList.AddTail(pDocOut);
    SetCursor(OldCursor);
}

////////////////////////
// Morfologia Binaria //
////////////////////////

void CPDImagemView::OnMorfologiaBinariaErosao()
{
    CString TitleWindow;
    HCURSOR OldCursor;
    CString Titulo;  
    CPDImagemDoc* pDocIn = GetDocument();  
    
    Titulo = pDocIn->GetTitle(); 

    Inserir = FALSE;
    CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
    CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
    Inserir = TRUE;
                                           
    pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
    pDocOut->Imagem.InitDIBData(); 

    m_pDlgElemEstr2 = new CDlgElemEstr2(this, &pDocIn->Imagem, &pDocOut->Imagem, &Titulo); 
    
	if (m_pDlgElemEstr2->DoModal() == IDCANCEL) 
		return;

    OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

    CDC* pDC;  
    pDC = GetDC();
    pDocOut->Imagem.Erosao(pDC);
    ReleaseDC(pDC);
    TitleWindow = "Erosao"; 
    pDocOut->SetTitle(TitleWindow);
    CFrameWnd* pFrameO;
    pFrameO = CreateChild(pDocOut, pDocOut);
    ObjectList.AddTail(pDocOut);
    SetCursor(OldCursor);
}

void CPDImagemView::OnMorfologiaBinariaDilatacao()
{
    CString TitleWindow;
    HCURSOR OldCursor;
    CString Titulo;  
    CPDImagemDoc* pDocIn = GetDocument();  
    
    Titulo = pDocIn->GetTitle(); 
    
    Inserir = FALSE;
    CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
    CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
    Inserir = TRUE;
                                        
    pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
    pDocOut->Imagem.InitDIBData(); 

    m_pDlgElemEstr2 = new CDlgElemEstr2(this, &pDocIn->Imagem, &pDocOut->Imagem, &Titulo); 

    if (m_pDlgElemEstr2->DoModal() == IDCANCEL) 
		return;

    OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

    CDC* pDC;  
    pDC = GetDC();
    pDocOut->Imagem.Dilatacao(pDC);
    ReleaseDC(pDC);
    TitleWindow = "Dilatacao"; 
    pDocOut->SetTitle(TitleWindow);
    CFrameWnd* pFrameO;
    pFrameO = CreateChild(pDocOut, pDocOut);
    ObjectList.AddTail(pDocOut);
    SetCursor(OldCursor);
}                        

void CPDImagemView::OnMorfologiaBinariaAbertura()
{
    CString TitleWindow;
    HCURSOR OldCursor;
    CString Titulo;  
    CPDImagemDoc* pDocIn = GetDocument();  
    
    Titulo = pDocIn->GetTitle(); 

    Inserir = FALSE;
    CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
    CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
    Inserir = TRUE;
                                           
    pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
    pDocOut->Imagem.InitDIBData(); 

    m_pDlgElemEstr2 = new CDlgElemEstr2(this, &pDocIn->Imagem, &pDocOut->Imagem, &Titulo); 

	if (m_pDlgElemEstr2->DoModal() == IDCANCEL) 
		 return;

    OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 
      
    CDC* pDC;  
    pDC = GetDC();
    pDocOut->Imagem.Abertura(pDC);
    ReleaseDC(pDC);

    TitleWindow = "Abertura"; 
    pDocOut->SetTitle(TitleWindow);
    CFrameWnd* pFrameO;
    pFrameO = CreateChild(pDocOut, pDocOut);
    ObjectList.AddTail(pDocOut);
    SetCursor(OldCursor);
}                        


void CPDImagemView::OnMorfologiaBinariaFechamento()
{
    CString TitleWindow;
    HCURSOR OldCursor;
    CString Titulo;  
    CPDImagemDoc* pDocIn = GetDocument();  
    
    Titulo = pDocIn->GetTitle(); 
    
    Inserir = FALSE;
    CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
    CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
    Inserir = TRUE;
                                           
    pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
    pDocOut->Imagem.InitDIBData(); 
      
    m_pDlgElemEstr2 = new CDlgElemEstr2(this, &pDocIn->Imagem, &pDocOut->Imagem, &Titulo); 
    
	if (m_pDlgElemEstr2->DoModal() == IDCANCEL)
		return;

    OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 
    CDC* pDC;  
    pDC = GetDC();
    pDocOut->Imagem.Fechamento(pDC);
    ReleaseDC(pDC);
    TitleWindow = "Fechamento"; 
    pDocOut->SetTitle(TitleWindow);
    CFrameWnd* pFrameO;
    pFrameO = CreateChild(pDocOut, pDocOut);
    ObjectList.AddTail(pDocOut);
    SetCursor(OldCursor);
}                        

void CPDImagemView::OnMorfologiaBinariaBordasErosao()
{
    CString TitleWindow;
    HCURSOR OldCursor;
    CString Titulo;  
    CPDImagemDoc* pDocIn = GetDocument();  
    
    Titulo = pDocIn->GetTitle(); 

    CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
    CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                           
    pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
    pDocOut->Imagem.InitDIBData(); 

    OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 
    CDC* pDC;  
    pDC = GetDC();
    pDocOut->Imagem.BordasErosao(pDC);
    ReleaseDC(pDC);
    TitleWindow = "Bordas_Ero"; 
    pDocOut->SetTitle(TitleWindow);
    CFrameWnd* pFrameO;
    pFrameO = CreateChild(pDocOut, pDocOut);
    SetCursor(OldCursor);
}                                             

void CPDImagemView::OnMorfologiaBinariaBordasDilatacao()
{
    CString TitleWindow;
    HCURSOR OldCursor;
    CString Titulo;  
    CPDImagemDoc* pDocIn = GetDocument();  
    
    Titulo = pDocIn->GetTitle(); 

    CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
    CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                           
    pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
    pDocOut->Imagem.InitDIBData(); 

    OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 
    CDC* pDC;  
    pDC = GetDC();
    pDocOut->Imagem.BordasDilatacao(pDC);
    ReleaseDC(pDC);
    TitleWindow ="Bordas_Dilatacao"; 
    pDocOut->SetTitle(TitleWindow);
    CFrameWnd* pFrameO;
    pFrameO = CreateChild(pDocOut, pDocOut);
    SetCursor(OldCursor);
}                                                  

void CPDImagemView::OnMorfologiaBinariaBordasDilEro()
{
    
    CString TitleWindow;
    HCURSOR OldCursor;
    CString Titulo;  
    CPDImagemDoc* pDocIn = GetDocument();  
    
    Titulo = pDocIn->GetTitle(); 
    
    CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
    CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                           
    pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
    pDocOut->Imagem.InitDIBData(); 

    OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 
    CDC* pDC;  
    pDC = GetDC();
    pDocOut->Imagem.BordasDilEro(pDC);
    ReleaseDC(pDC);
    TitleWindow = "Bordas_Dil_Ero"; 
    pDocOut->SetTitle(TitleWindow);
    CFrameWnd* pFrameO;
    pFrameO = CreateChild(pDocOut, pDocOut);
    SetCursor(OldCursor);
}                        

void CPDImagemView::OnMorfologiaBinariaEsqueletizacao()
{
	CString TitleWindow;
	HCURSOR OldCursor;
	CString Titulo;  
	CPDImagemDoc* pDocIn = GetDocument();  

	Inserir = FALSE;
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
	Inserir = TRUE;                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
  
	Titulo = "NAOITERAR";  

	m_pDlgElemEstr2 = new CDlgElemEstr2(this, &pDocIn->Imagem, &pDocOut->Imagem, &Titulo); 

	if (m_pDlgElemEstr2->DoModal() == IDCANCEL) 
		return;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 
	CDC* pDC;  
	pDC = GetDC();
	pDocOut->Imagem.Esqueletizacao(pDC);
	ReleaseDC(pDC);
	TitleWindow = "Esqueletizacao"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
	ObjectList.AddTail(pDocOut);
	SetCursor(OldCursor);
}                        

void CPDImagemView::OnMorfologiaBinariaUltimaErosao()
{
	CString TitleWindow;
	HCURSOR OldCursor;
	CString Titulo;  
	CPDImagemDoc* pDocIn = GetDocument();  

	Inserir = FALSE;
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
	Inserir = TRUE;                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 

	Titulo = "NAOITERAR";  

	m_pDlgElemEstr2 = new CDlgElemEstr2(this, &pDocIn->Imagem, &pDocOut->Imagem, &Titulo); 

	if (m_pDlgElemEstr2->DoModal() == IDCANCEL) 
		return;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 
	CDC* pDC;  
	pDC = GetDC();
	pDocOut->Imagem.UltimaErosao(pDC);
	ReleaseDC(pDC);
	TitleWindow = "Ultima Erosao"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
	ObjectList.AddTail(pDocOut);
	SetCursor(OldCursor);
}                        



// pedro

//int operation (int x, int y, int (*functocall)(int,int))

void CPDImagemView::MaiorMenor(double *maior, double *menor, CPDImagemDoc* pDocIn, char tipo){
	int col, lin, cor;
	double r, g, t, l, u, v, y, i, q, cor2;

	for (col = 1; col < (int) pDocIn->Imagem.GetWidth()-1; col++) 
	{
		for (lin = 1; lin < (int) pDocIn->Imagem.GetHeight()-1; lin++)  
		{
  		    cor = (int) pDocIn->Imagem.GetPixel((DWORD) col,(DWORD) lin);
  		    cor2 = pDocIn->Imagem.GetPixel((DWORD) col,(DWORD) lin);

			if (tipo == 'T' || tipo == 'S'){

				r = (float)GetRValue(cor) / (GetRValue(cor) + GetGValue(cor) + GetBValue(cor));
				g = (float)GetGValue(cor) / (GetRValue(cor) + GetGValue(cor) + GetBValue(cor));
				r = r - 1/3;
				g = g - 1/3;

				if (tipo == 'T') t = CalculaT(r,g);
				if (tipo == 'S') t = CalculaS(r,g);
			}
			else
				if (tipo == 'L' || tipo == 'U' || tipo == 'V'){
					CalculaLUV(cor2, &l, &u, &v);
					if (tipo == 'L') t = l;
					if (tipo == 'U') t = u;
					if (tipo == 'V') t = v;
			}
			else
			if(tipo == 'Y' || tipo == 'I' || tipo == 'Q'){
				CalculaYIQ(cor2, &y, &i, &q);
				if (tipo == 'Y') t = y;
				if (tipo == 'I') t = i;
				if (tipo == 'Q') t = q;
			}

			if (col == 1 && lin == 1) *maior = *menor = t;
			else {
				if (t > *maior) *maior = t;
				if (t < *menor) *menor = t;
			}

		}
	}
}

double CPDImagemView::CalculaT(double r, double g){
	double t;
	if (g > 0)
		t = atan(r/g) / 2 * PI + 1/4;
	  else
		if (g < 0)
			t = atan(r/g) / 2 * PI + 3/4;
		  else
			t = 0;	
	return t;
}

double CPDImagemView::CalculaL(int cor){
	double l;
	l = 0.299 * (float)GetRValue(cor) + 0.587 * (float)GetGValue(cor) + 0.114 * (float)GetBValue(cor);
	return l;
}

double CPDImagemView::CalculaS(double r, double g){
	double s;
	s = sqrt(9/5 * (r*r + g*g));
	return s;
}


void	CPDImagemView::CalculaLUV(double cor, double *l, double *u, double *v){
	
const double XYZ[3][3] = {	{  0.4125,  0.3576,  0.1804 },
							{  0.2125,  0.7154,  0.0721 },
							{  0.0193,  0.1192,  0.9502 }	};
	
	double X, Y, Z, l0, u0, v0, base;
	double Xw = 0.9422, Yw = 3.0395, Zw = 1.0888, uw = 0.19784977571475, vw = 0.46834507665248;
	int r, g, b;

//	FILE *arq2;
//	arq2 = fopen("arqLUV.txt", "w");

	r = GetRValue((int)cor);
	g = GetGValue((int)cor);
	b = GetBValue((int)cor);

//	fprintf(arq2, "r=%f ", r);
//	fprintf(arq2, "g=%f ", g);
//	fprintf(arq2, "b=%f ", b);

	// conversao d RGB p/ XYZ
	X	= XYZ[0][0]*r + XYZ[0][1]*g + XYZ[0][2]*b;
	Y	= XYZ[1][0]*r + XYZ[1][1]*g + XYZ[1][2]*b;
	Z	= XYZ[2][0]*r + XYZ[2][1]*g + XYZ[2][2]*b;

//	fprintf(arq2, "X=%f ", X);
//	fprintf(arq2, "Y=%f ", Y);
//	fprintf(arq2, "Z=%f ", Z);

	base = X+15*Y+3*Z;
//	fprintf(arq2, "base=%f ", base);

	if (base == 0){
		u0 = 4;
		v0 = 9 / 15;
	}
	else{
		u0 = 4 * X / base;
		v0 = 9 * Y / base;
	}
//	fprintf(arq2, "u0=%f ", u0);
//	fprintf(arq2, "v0=%f ", v0);


	l0 = Y / Yw;
//	l0 = Y / 255;
//	fprintf(arq2, "l0=%f ", l0);
	
	if (l0 > 0.008856)
		*l = 116 * pow(l0, 0.33333) - 16;
	  else
		*l = 903.3 * l0;
	
	*u = 13 * *l * (u0 - uw);
	*v = 13 * *l * (v0 - vw);

//	fprintf(arq2, "l=%f ", *l);
//	fprintf(arq2, "u=%f ", *u);
//	fprintf(arq2, "v=%f ", *v);
//	fclose(arq2);
//	exit(0);
}

/*
double CPDImagemView::NormalizaCores(double r2, double r1, double pe, double pe1, double pe2) {
//double CPDIBase::NormalizaIntensidade(double pe, double pe1, double pe2)
 onde:
r2 - valor maximo possivel na escala de normalizacao (RGB - max. 255)
r1 - valor minimo possivel na escala de normalizacao (RGB - min. 0)
pe = valor da intensidade no ponto em questão da imagem já processada (atual, que não é a original)
pe1 = valor mínimo que a intensidade pode assumir na imagem já processada (atual, que não é a original)
pe2 = valor máximo que a intensidade pode assumir na imagem já processada (atual, que não é a original)

	double norm;
	norm = ((r2-r1) / (pe2-pe1)) * (pe-pe1) + r1;
	return (norm);
}
*/

double CPDImagemView::NormalizaCores(double r2, double r1, double pe, double pe1, double pe2) {
/* onde:
r2 - valor maximo possivel na escala de normalizacao (RGB - max. 255)
r1 - valor minimo possivel na escala de normalizacao (RGB - min. 0)
pe = valor da intensidade no ponto em questão da imagem já processada (atual, que não é a original)
pe1 = valor mínimo que a intensidade pode assumir na imagem já processada (atual, que não é a original)
pe2 = valor máximo que a intensidade pode assumir na imagem já processada (atual, que não é a original)
*/
	double norm;
// formula do pedro (regra de tres)
	norm = ((pe-pe1) / (pe2 - pe1)) * (r2-r1);
//	norm = ((r2-r1) / (pe2-pe1)) * (pe-pe1) + r1;
	return (norm);
	/* DIBNumColors()-0 = valor maior menos o zero (menor valor possível) da imagem	*/
}

void CPDImagemView::TratamentoCores(char tipo, int canal, CPDImagemDoc* pDocIn, CPDImagemDoc* pDocOut) {

	FILE *arq;
	arq = fopen("arq.txt", "w");
	int	col, lin, cor, novaCor;
	double r, g, b, s, t, l, u, v, maior, menor, cor2;
	int c1, c2, c3;
	if (tipo == 'T') MaiorMenor(&maior, &menor, pDocIn, 'T');
	if (tipo == 'S') MaiorMenor(&maior, &menor, pDocIn, 'S');

	if (tipo == 'L') MaiorMenor(&maior, &menor, pDocIn, 'L');
	if (tipo == 'U') MaiorMenor(&maior, &menor, pDocIn, 'U');
	if (tipo == 'V') MaiorMenor(&maior, &menor, pDocIn, 'V');

	
	for (col = 1; col < (int) pDocIn->Imagem.GetWidth()-1; col++) 
	{
		for (lin = 1; lin < (int) pDocIn->Imagem.GetHeight()-1; lin++)  
		{
  		    cor = (int) pDocIn->Imagem.GetPixel((DWORD) col,(DWORD) lin);
  		    cor2 = pDocIn->Imagem.GetPixel((DWORD) col,(DWORD) lin);
			r = (float)GetRValue(cor) / (GetRValue(cor) + GetGValue(cor) + GetBValue(cor));
			g = (float)GetGValue(cor) / (GetRValue(cor) + GetGValue(cor) + GetBValue(cor));
			b = (float)GetBValue(cor) / (GetRValue(cor) + GetGValue(cor) + GetBValue(cor));

			switch (tipo){

					case 'L' :	// CIE - LUV
					case 'U' :	// CIE - LUV
					case 'V' :	// CIE - LUV
								r = (float)GetRValue((int)cor2);
								g = (float)GetGValue((int)cor2);
								b = (float)GetBValue((int)cor2);
								fprintf(arq, "cor2=%f ", cor2);
								fprintf(arq, "r=%f ", r);
								fprintf(arq, "g=%f ", g);
								fprintf(arq, "b=%f ", b);
								CalculaLUV(cor2, &l, &u, &v);

								fprintf(arq, "l=%f ", l);
								fprintf(arq, "u=%f ", u);
								fprintf(arq, "v=%f ", v);

								c1 = (int) NormalizaCores(255,0,l,menor,maior); 
								c2 = (int) NormalizaCores(255,0,u,menor,maior); 
								c3 = (int) NormalizaCores(255,0,v,menor,maior); 
								fprintf(arq, "maior=%f ", maior);
								fprintf(arq, "menor=%f ", menor);
								fprintf(arq, "c1=%d ", c1);
								fprintf(arq, "c2=%d ", c2);
								fprintf(arq, "c3=%d\n", c3);

								
								break;
					case 'T' :	// TSL
					case 'S' :	// TSL
					case 'l' :	// TSL
								s = CalculaS(r,g);
								t = CalculaT(r,g);
								l = CalculaL(cor);
								
//								fprintf(arq, "t=%f ", t);
//								fprintf(arq, "s=%f ", s);
//								fprintf(arq, "l=%f ", l);
								c1 = (int) NormalizaCores(255,0,t,menor,maior);
//								fprintf(arq, "maior=%f ", maior);
//								fprintf(arq, "menor=%f ", menor);
//								fprintf(arq, "c1=%d ", c1);
								c2 = (int) NormalizaCores(255,0,s,menor,maior);
//								fprintf(arq, "c2=%d ", c2);
								c3 = (int) (l * 255);
//								fprintf(arq, "c3=%d\n", c3);
								break;

			}
			if (canal == 1)	novaCor = RGB(c1,c1,c1);
			if (canal == 2)	novaCor = RGB(c2,c2,c2);
			if (canal == 3)	novaCor = RGB(c3,c3,c3);
			pDocOut->Imagem.SetPixel((DWORD) col,(DWORD) lin, novaCor); 
		} 
	}
	fclose(arq);
}

void  CPDImagemView::OnRGBGeraHistograma() {

	FILE *arq;
	arq = fopen("histRGB2.txt", "w");
	int	col, lin, cor, pontos;
	int	   histR[256], histG[256], histB[256];
	float  percR[256], percG[256], percB[256];
	double h, r, s, mediaR, desvioR, mediaG, desvioG, mediaB, desvioB;

	mediaR = desvioR = mediaG = desvioG = mediaB = desvioB = 0;

//	s = 1; // quem é s ????????????????

//	r = 0.4 * s; // subSegmentação
	r = 0.3 * s; // superSegmentação
//	r = 0.2 * s; // quantização

//	h = 2*r+1; // janela de pesquisa no espaco de atributos de cores

	h = 21; // tamanho da janela de pesquisa = igual TESE.PDF

	CPDImagemDoc* pDocIn = GetDocument();  
	pontos = pDocIn->Imagem.GetWidth() * pDocIn->Imagem.GetHeight();

	//zera vetores do histograma
	for (col = 0; col <= 255; col++) {
		histR[col] = histG[col] = histB[col] = 0;
		percR[col] = percG[col] = percB[col] = 0;
	}

	for (col = 1; col < (int) pDocIn->Imagem.GetWidth()-1; col++) 
	{
		for (lin = 1; lin < (int) pDocIn->Imagem.GetHeight()-1; lin++)  
		{
  		    cor = (int) pDocIn->Imagem.GetPixel((DWORD) col,(DWORD) lin);
			histR[GetRValue(cor)]++;
			histG[GetGValue(cor)]++;
			histB[GetBValue(cor)]++;
			mediaR = mediaR + GetRValue(cor);
			mediaG = mediaG + GetGValue(cor);
			mediaB = mediaB + GetBValue(cor);
/*			desvioR = desvioR + pow(GetRValue(cor),2);
			desvioG = desvioG + pow(GetGValue(cor),2);
			desvioB = desvioB + pow(GetBValue(cor),2);
*/		}
	}

/*	desvioR = sqrt(abs(desvioR - mediaR) / (pontos - 1));
	desvioG = sqrt(abs(desvioG - mediaG) / (pontos - 1));
	desvioB = sqrt(abs(desvioB - mediaB) / (pontos - 1));
*/
	mediaR = mediaR / pontos;
	mediaG = mediaG / pontos;
	mediaB = mediaB / pontos;

	fprintf(arq, "Vermelho\n");
	for (col = 0; col <= 255; col++) 
	{
		desvioR = desvioR + (histR[col]*col - mediaR);
		percR[col] = ((float)histR[col] / pontos);
		fprintf(arq, " %3d=[%6d-%5.2f%%]", col, histR[col], percR[col] * 100);
		if ((col+1)%10 == 0) fprintf(arq, "\n");
	}	
	fprintf(arq, "\nVerde\n");
	for (col = 0; col <= 255; col++) 
	{
		desvioG = desvioG + (histG[col]*col - mediaG);
		percG[col] = ((float)histG[col] / pontos);
		fprintf(arq, " %3d=[%6d-%5.2f%%]", col, histG[col], percG[col] * 100);
		if ((col+1)%10 == 0) fprintf(arq, "\n");
	}	
	fprintf(arq, "\nAzul\n");
	for (col = 0; col <= 255; col++) 
	{
		desvioB = desvioB + (histB[col]*col - mediaB);
		percB[col] = ((float)histB[col] / pontos);
		fprintf(arq, " %3d=[%6d-%5.2f%%]", col, histB[col], percB[col] * 100);
		if ((col+1)%10 == 0) fprintf(arq, "\n");
	}	


	desvioR = sqrt(desvioR / (pontos - 1));
	desvioG = sqrt(desvioG / (pontos - 1));
	desvioB = sqrt(desvioB / (pontos - 1));
	fprintf(arq, "\nMedias\n");
	fprintf(arq, " MediaR=[%5.2f], desvioR=[%5.2f]\n", mediaR, desvioR);
	fprintf(arq, " MediaG=[%5.2f], desvioG=[%5.2f]\n", mediaG, desvioG);
	fprintf(arq, " MediaB=[%5.2f], desvioB=[%5.2f]\n", mediaB, desvioB);
	fclose(arq);
}

void  CPDImagemView::OnRGBGeraHistogramaTSL() {

	FILE *arq;
	arq = fopen("histRGB.txt", "w");
	int	col, lin, cor, histR[256], histG[256], histB[256];

	CPDImagemDoc* pDocIn = GetDocument();  

	//zera vetores do histograma
	for (col = 0; col <= 255; col++) {
		histR[col] = histG[col] = histB[col] = 0;
	}

	for (col = 1; col < (int) pDocIn->Imagem.GetWidth()-1; col++) 
	{
		for (lin = 1; lin < (int) pDocIn->Imagem.GetHeight()-1; lin++)  
		{
  		    cor = (int) pDocIn->Imagem.GetPixel((DWORD) col,(DWORD) lin);
			histR[GetRValue(cor)]++;
			histG[GetGValue(cor)]++;
			histB[GetBValue(cor)]++;
		}
	}

	fprintf(arq, "Vermelho\n");
	for (col = 0; col <= 255; col++) 
	{
		fprintf(arq, " %5d = [%6d]", col, histR[col]);
		if (col%9 == 0) fprintf(arq, "\n");
	}	
	fprintf(arq, "Verde\n");
	for (col = 0; col <= 255; col++) 
	{
		fprintf(arq, " %d = [%d]", col, histG[col]);
	}	
	fprintf(arq, "Azul\n");
	for (col = 0; col <= 255; col++) 
	{
		fprintf(arq, " %d = [%d]", col, histB[col]);
	}	
	fclose(arq);
}

void	CPDImagemView::OnRGBLUVCanalL(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData();
	
	TratamentoCores('L', 1, pDocIn, pDocOut); 

	SetCursor(OldCursor);
	TitleWindow = "CIE-LUV Canal L"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

void	CPDImagemView::OnRGBLUVCanalU(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData();
	
	TratamentoCores('U', 2, pDocIn, pDocOut); 

	SetCursor(OldCursor);
	TitleWindow = "CIE-LUV Canal U"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

void	CPDImagemView::OnRGBLUVCanalV(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData();
	
	TratamentoCores('V', 3, pDocIn, pDocOut); 

	SetCursor(OldCursor);
	TitleWindow = "CIE-LUV Canal V"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

void CPDImagemView::OnAdiciona()
{
	CDlgAdicionar *dialog = new CDlgAdicionar();
	if (dialog->DoModal() == IDCANCEL)
		return;

	CString TitleWindow;
	HCURSOR OldCursor;
	OldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

	CPDImagemDoc *pDocIn = GetDocument();
	CDocTemplate *pTemplate = pDocIn->GetDocTemplate();
	CPDImagemDoc *pDocOut = (CPDImagemDoc*)pTemplate->CreateNewDocument();

	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle();
	pDocOut->Imagem.InitDIBData();

	BYTE valor = *dialog->m_pValor;
	pDocOut->Imagem.Adiciona(valor);

	char titleBuff[16];
	sprintf(titleBuff, "Adicionado %d", *dialog->m_pValor);

	SetCursor(OldCursor);
	TitleWindow = titleBuff;
	pDocOut->SetTitle(TitleWindow);

	CFrameWnd *pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

void CPDImagemView::OnFastAdaptiveContrast()
{
	DlgFastAdaptiveContrastParameters *dialog = new DlgFastAdaptiveContrastParameters();
	if (dialog->DoModal() == IDCANCEL)
		return;

	CString TitleWindow;
	HCURSOR OldCursor;
	OldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

	CPDImagemDoc *pDoc = GetDocument();
	pDoc->Imagem.FastAdaptiveContrast(5, *dialog->m_pLimiarRuido);

	SetCursor(OldCursor);

	MessageBox("Os dados dos pixels foram exportados para o arquivo saida.txt");
}

void	CPDImagemView::OnRGBTSLCanalT(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData();
	
	TratamentoCores('T', 1, pDocIn, pDocOut); 

	SetCursor(OldCursor);
	TitleWindow = "TSL Canal T"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}


void	CPDImagemView::OnRGBTSLCanalS(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData();
	
	TratamentoCores('S', 2, pDocIn, pDocOut); 

	SetCursor(OldCursor);
	TitleWindow = "TSL Canal S"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

void	CPDImagemView::OnRGBTSLCanalL(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData();
	
	TratamentoCores('l', 3, pDocIn, pDocOut); 

	SetCursor(OldCursor);
	TitleWindow = "TSL Canal L"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

void	CPDImagemView::OnRGBNormalizado(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData();
	
	TratamentoCores('N', 1, pDocIn, pDocOut); 
	//pDocOut->Imagem.GrayPalette();
	//pDocOut->Imagem.TonsCinza();

	SetCursor(OldCursor);
	TitleWindow = "RGB Normalizado"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}


void	CPDImagemView::OnMatrizCoOcorrencia(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette();
// inicio
//	char *texto;
	int cor, aux, posicao, col, lin, colLM, linLM, mat [9][2][2];//norte[2][2], leste[2][2], sul[2][2], oeste[2][2], nordeste[2][2], sudeste[2][2], sudoeste[2][2], noroeste[2][2];
	// zera as matrizes
	for (posicao = 0; posicao < 9; posicao++){
		for (col = 0; col < 2; col++) {
  		    for (lin = 0; lin < 2; lin++) {
				mat[posicao][col][lin] = 0;
			}
		}
	}

	for (col = 1; col < (int) pDocIn->Imagem.GetWidth()-1; col++) 
	{
		for (lin = 1; lin < (int) pDocIn->Imagem.GetHeight()-1; lin++)  
		{
			cor = (int) pDocIn->Imagem.GetPixel((DWORD) col-1,(DWORD) lin-1);
			if (cor != 0)
				cor = 1; 
			posicao = 0;
			for (colLM = -1; colLM < 1; colLM++) {
				for (linLM = -1; linLM < 1; linLM++) {
					aux = (int) pDocIn->Imagem.GetPixel((DWORD) col-1+colLM,(DWORD) lin-1-linLM);
					if (aux != 0) aux = 1;
					mat[posicao][cor][aux]++;
					posicao++;
				}
			}
		}
	}
	
/*	strcpy(texto, "");
	for (posicao = 0; posicao < 9; posicao++){
		for (col = 0; col < 2; col++) {
  		    for (lin = 0; lin < 2; lin++) {
				strcat(texto, (char*) mat[posicao][col][lin]);
			}
		}
	}
*/
//MessageBox(NULL, texto, NULL, MB_OK);
// fim
//	pDocOut->Imagem.TonsCinza();

//	SetCursor(OldCursor);
//	TitleWindow = "TONSDECINZA"; 
//	pDocOut->SetTitle(TitleWindow);
//	CFrameWnd* pFrameO;
//	pFrameO = CreateChild(pDocOut, pDocOut);
}

void	CPDImagemView::OnImagemTonsDeCinza(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette();
	pDocOut->Imagem.TonsCinza();

	SetCursor(OldCursor);
	TitleWindow = "TONSDECINZA"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

void	CPDImagemView::OnImagemTonsDeCinzaNTSC(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette();
	pDocOut->Imagem.TonsCinzaNTSC();

	SetCursor(OldCursor);
	TitleWindow = "TONSDECINZA-NTSC"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

void CPDImagemView::Masks(char tipo, CPDImagemDoc* pDocIn, CPDImagemDoc* pDocOut) {
	int A = 1;
	int LaplacianMask[5][5] = {{0,0,-1,0,0},{0,-1,-2,-1,0},{-1,-2,16,-2,-1},{0,-1,-2,-1,0},{0,0,-1,0,0}};
	int LMHorizontal[3][3] = {{-1,-1,-1},{ 2, 2, 2},{-1,-1,-1}};
	int LMMais45	[3][3] = {{-1,-1, 2},{-1, 2,-1},{ 2,-1,-1}};
	int LMMenos45	[3][3] = {{ 2,-1,-1},{-1, 2,-1},{-1,-1, 2}};
	int LMVertical	[3][3] = {{-1, 2,-1},{-1, 2,-1},{-1, 2,-1}};
	int PDetection	[3][3] = {{-1,-1,-1},{-1, 8,-1},{-1,-1,-1}};
	int Prewitt		[3][3] = {{ 0, 1, 1},{-1, 0, 1},{-1,-1, 0}};
	int HighBoost	[3][3] = {{ 0, -1, 0},{-1, A+4, -1},{0,-1, 0}};
	int Roberts1	[2][2] = {{ 1, 0}	,{ 0,-1}			  };

	int	col, lin, cor, novaCor, linLM, colLM, acumulaCor, tam, C, inicio, LMTot[4], acumulaLMTot[4];
	switch (tipo){
		case 'L' : tam = 5; break;
		case 'R' : tam = 2; break;
		case 'N' : tam = 1; break;
		default:
			tam = 3;
	}

	inicio = 1;

	C = 95; // constante da nonLinear Compression
	
	for (col = inicio; col < (int) pDocIn->Imagem.GetWidth()-1; col++) 
	{
		for (lin = inicio; lin < (int) pDocIn->Imagem.GetHeight()-1; lin++)  
		{
			acumulaCor=acumulaLMTot[0]=acumulaLMTot[1]=acumulaLMTot[2]=acumulaLMTot[3]=0;

			for (linLM = 0; linLM < tam; linLM++)  
			{
				for (colLM = 0; colLM < tam; colLM++) 
				{
					cor = (int) pDocIn->Imagem.GetPixel((DWORD) col-inicio+colLM,(DWORD) lin-inicio+linLM);
					switch (tipo){
						case 'H' :	cor = cor * LMHorizontal[colLM][linLM];
									break;
						case '+' :	cor = cor * LMMais45[colLM][linLM];
									break;
						case '-' :	cor = cor * LMMenos45[colLM][linLM];
									break;
						case 'V' :  cor = cor * LMVertical[colLM][linLM];
									break;
						case 'T' :  LMTot[0] = cor * LMVertical[colLM][linLM];
									LMTot[1] = cor * LMHorizontal[colLM][linLM];
									LMTot[2] = cor * LMMais45[colLM][linLM];
									LMTot[3] = cor * LMMenos45[colLM][linLM];
									break;
						case 'P' :  cor = cor * PDetection[colLM][linLM];
									break;
						case 'W' :  cor = cor * Prewitt[colLM][linLM];
									break;
						case 'R' :  cor = cor * Roberts1[colLM][linLM];
									break;
						case 'L' :  cor = cor * LaplacianMask[colLM][linLM];
									break;
						case 'B' :  cor = cor * HighBoost[colLM][linLM];
									break;
//						case 'N' :  cor = C * log(1 + cor);
//									break;
					}
					if (tipo == 'T') {
						acumulaLMTot[0] += LMTot[0];
						acumulaLMTot[1] += LMTot[1];
						acumulaLMTot[2] += LMTot[2];
						acumulaLMTot[3] += LMTot[3];
					}
				    else
					   acumulaCor += cor;
				}
			}
			if (tipo == 'T') {
				int maior = acumulaLMTot[0];
				for (int x=1;x<=3;x++) {
					if (acumulaLMTot[x] > maior)
						maior = acumulaLMTot[x];
				}
				novaCor = maior;
			}
			else
			  novaCor = acumulaCor;
			if (novaCor > 255)   novaCor = 255;
			if (novaCor <   0)   novaCor = 0;
			pDocOut->Imagem.SetPixel((DWORD) col,(DWORD) lin, novaCor); 
		} 
	}
}


void	CPDImagemView::OnLineMasksTotal				(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette();

	
	Masks('T', pDocIn, pDocOut);
	
	
//	pDocOut->Imagem.LineMasks('H');
	SetCursor(OldCursor);

	TitleWindow =  "Line Masks - Total"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}


void	CPDImagemView::OnLineMasksHorizontal				(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette();

	
	Masks('H', pDocIn, pDocOut);
	
	
//	pDocOut->Imagem.LineMasks('H');
	SetCursor(OldCursor);

	TitleWindow =  "Line Masks - Horizontal"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

void	CPDImagemView::OnLineMasksVertical					(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette();
//	pDocOut->Imagem.LineMasks('V');
	Masks('V', pDocIn, pDocOut);

	SetCursor(OldCursor);

	TitleWindow =  "Line Masks - Vertical"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

void	CPDImagemView::OnLineMasksMais45		 	 		(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette();
//	pDocOut->Imagem.LineMasks('+');
	Masks('+', pDocIn, pDocOut);
	SetCursor(OldCursor);

	TitleWindow =  "Line Masks - Mais 45 graus"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

void	CPDImagemView::OnLineMasksMenos45					(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette();
//	pDocOut->Imagem.LineMasks('-');
	Masks('-', pDocIn, pDocOut);

	SetCursor(OldCursor);

	TitleWindow =  "Line Masks - Menos 45 graus"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

void	CPDImagemView::OnPointDetection					(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette();
//	pDocOut->Imagem.LineMasks('-');
	Masks('P', pDocIn, pDocOut);

	SetCursor(OldCursor);

	TitleWindow =  "Point Detection"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

void    CPDImagemView::OnNonLinearCompression		(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette();
//	pDocOut->Imagem.LineMasks('-');
	Masks('N', pDocIn, pDocOut);

	SetCursor(OldCursor);

	TitleWindow =  "NonLinear Compression"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

void    CPDImagemView::OnLaplacianMask				(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette();
//	pDocOut->Imagem.LineMasks('-');
	Masks('L', pDocIn, pDocOut);

	SetCursor(OldCursor);

	TitleWindow =  "Laplacian Mask"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}


void	CPDImagemView::OnPrewitt					(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette();
//	pDocOut->Imagem.LineMasks('-');
	Masks('W', pDocIn, pDocOut);

	SetCursor(OldCursor);

	TitleWindow =  "Prewitt"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

void	CPDImagemView::OnHighBoost					(){
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOut->Imagem.InitDIBData(); 
	pDocOut->Imagem.GrayPalette();
//	pDocOut->Imagem.LineMasks('-');
	Masks('B', pDocIn, pDocOut);

	SetCursor(OldCursor);

	TitleWindow =  "High Boost"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

///////
/*
void CPDImagemView::OnImagemZoom()
{
//	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 
	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    

	pDocOut->Imagem.hDIB = pDocIn->Imagem.AumentaDIB(150, 150); 
	pDocOut->Imagem.InitDIBData(); 
	//ImagemZoomSi(pDocIn, pDocOut);
	SetCursor(OldCursor);
	TitleWindow = "ZOOM"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}
*/

void CPDImagemView::ImagemZoomSi(CPDImagemDoc *pDocIn, CPDImagemDoc *pDocOut)
{
	int 			largurazoom, alturazoom;
	int    		    pontoA, pontoB, pontoC, pontoD;
	int 			colzoom, linzoom, colinteiro, lininteiro,coloriginal,linoriginal;
	double 			deltacol, deltalin,ContribAeB, ContribCeD;

	int				width, height;

	CString TitleWindow;
	//HCURSOR OldCursor;
	int lin;

	width	= (int) pDocIn->Imagem.GetWidth();
	height	= (int) pDocIn->Imagem.GetHeight();

	DWORD COR, COR1, COR2, COR3 ,COR4, COR5, COR6, COR7, COR8;
	int A, B, C, D;

	int maiorlin, menorlin, maiorcol, menorcol;

	maiorlin=maiorcol=-1;
	menorcol=menorlin=9999;

	for (lin = 0; lin < (int) pDocIn->Imagem.GetHeight(); lin ++)
		for (int col = 0; col < (int) pDocIn->Imagem.GetWidth(); col++)
		{
			A=B=C=D=0;
			COR1=COR2=COR3=COR4=COR5=COR6=COR7=COR8=255;
			COR = pDocIn->Imagem.GetPixel((DWORD) col,(DWORD) lin);
			if ((lin+1) < height)
			{ 
				COR1	= pDocIn->Imagem.GetPixel((DWORD) col,(DWORD) lin+1);
				A		= 1;
			}
			if ((lin-1) >= 0) 
			{
				COR2	= pDocIn->Imagem.GetPixel((DWORD) col,(DWORD) lin-1);
				B		= 1;
			}
			if ((col-1) >= 0)
			{
				COR3	= pDocIn->Imagem.GetPixel((DWORD) col-1,(DWORD) lin);
				C		= 1;
			}
			if ((col+1) < width) 
			{
				COR4 = pDocIn->Imagem.GetPixel((DWORD) col+1,(DWORD) lin);
				D = 1;
			}
			if (A && C)
				COR5 = pDocIn->Imagem.GetPixel((DWORD) col-1,(DWORD) lin+1);
			if (A && D)
				COR6 = pDocIn->Imagem.GetPixel((DWORD) col+1,(DWORD) lin+1);
			if (B && C)
				COR7 = pDocIn->Imagem.GetPixel((DWORD) col-1,(DWORD) lin-1);
			if (B && D)
				COR8 = pDocIn->Imagem.GetPixel((DWORD) col+1,(DWORD) lin-1);
			if (COR == 0)
			{
				if   (COR1 == 0 || COR2 == 0 || COR3 == 0 || COR4 == 0
				   || COR5 == 0 || COR6 == 0 || COR7 == 0 || COR8 == 0)
				{
				   if (lin > maiorlin)
					   maiorlin = lin;
				   if (lin < menorlin)
					   menorlin = lin;
				   if (col > maiorcol)
					   maiorcol = col;
				   if (col < menorcol)
					   menorcol = col;
				}
			}
		}

	float altura = (float) (maiorlin - menorlin);
	float largura = (float) (maiorcol - menorcol);

	float fatorampliacao_l = (float) (24. / largura);
	float fatorampliacao_h = (float) (40. / altura); 

	float resultadointer;
	//char buf[30];

	largurazoom = (int)(fatorampliacao_l * width  + 0.5);		/* inicializacao */
	alturazoom  = (int)(fatorampliacao_h * height  + 0.5);  

//	pDocOut->Imagem.hDIB = pDocIn->Imagem.AumentaDIB(alturazoom, largurazoom); 
//	pDocOut->Imagem.InitDIBData(); 

	DWORD bmWidthBytesNew = 4 * ((200 * pDocIn->Imagem.GetBitsPerPixel() + 31) / 32 );

	for (linzoom = 0; linzoom < alturazoom; linzoom++)  
	{       
		linoriginal = (int) (linzoom / fatorampliacao_h);
		lininteiro = linoriginal;
		deltalin = (double) (linoriginal - lininteiro);

		for (colzoom = 0; colzoom < largurazoom; colzoom++)
		{
			coloriginal = (int) (colzoom / fatorampliacao_l);	       
			colinteiro = coloriginal;               
			deltacol = (double) (coloriginal - colinteiro);

			if( (colinteiro < (int) width) && (lininteiro < height) )
			{       
				pontoA = (int) *(pDocIn->Imagem.lpBits+lininteiro*pDocIn->Imagem.bmWidthBytes + colinteiro);

				if( (colinteiro+1 < width) && (lininteiro+1 < height) )
				{       
					pontoB = (int) *(pDocIn->Imagem.lpBits+(lininteiro)*pDocIn->Imagem.bmWidthBytes + colinteiro+1);
					pontoC = (int) *(pDocIn->Imagem.lpBits+(lininteiro+1)*pDocIn->Imagem.bmWidthBytes + colinteiro);
					pontoD = (int) *(pDocIn->Imagem.lpBits+(lininteiro+1)*pDocIn->Imagem.bmWidthBytes+ colinteiro+1);

					ContribAeB = deltacol * ((double)pontoB - pontoA) + pontoA;
					ContribCeD = deltacol * ((double)pontoD - pontoC) + pontoC;
					resultadointer = (float) (0.5 + ContribAeB + (ContribCeD - ContribAeB)*deltalin);
					*(pDocOut->Imagem.lpBits+(linzoom+60)*bmWidthBytesNew + (colzoom+60)) = (resultadointer < 128.) ? 0 : 255; /* caso imagem binaria */ 
//					*(pDocOut->Imagem.lpBits+linzoom*pDocOut->bmWidthBytesNew + colzoom) = 0.5 + ContribAeB + (ContribCeD - ContribAeB)*deltalin;
				} /* fim do if(colinteiro+1 < width) */
				else
					*(pDocOut->Imagem.lpBits+(linzoom+60)*bmWidthBytesNew + (colzoom+60)) = pontoA ;
			}/* fim do if(colinteiro+1 < width) */
		}/* fim do for */
	}/* fim do for */  
}
        
// Operacoes Logicas a partir de duas imagens
void CPDImagemView::OnOperacoesLogicas()
{
    HCURSOR OldCursor;
    OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

    m_pDlgOpLogica = new CDlgOpLogica(this); 
    if(m_pDlgOpLogica->GetSafeHwnd() == 0) 
		m_pDlgOpLogica->DoModal();

	SetCursor(OldCursor);
}

void CPDImagemView::OnImagemInverte()
{
    CString TitleWindow;
    HCURSOR OldCursor;
    
    OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

    CPDImagemDoc* pDocIn = GetDocument();  
    CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

    CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                           
    pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
    pDocOut->Imagem.InitDIBData(); 
    pDocOut->Imagem.InverteImagem();

    SetCursor(OldCursor);
    TitleWindow = "Inverte"; 
    pDocOut->SetTitle(TitleWindow);
    CFrameWnd* pFrameO;
    pFrameO = CreateChild(pDocOut, pDocOut);

}

// Rotina responsavel pela criacao de uma nova janela com 
// os resultados de um processamento
CFrameWnd* CPDImagemView::CreateChild(CPDImagemDoc* pd, CPDImagemDoc* pDocIn)
{
	CMDIChildWnd* pActiveChild = (CMDIChildWnd*) GetActiveWindow();

	if (pActiveChild == NULL) 
		return NULL;
    
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
	ASSERT_VALID(pTemplate);                         
                                    
	CFrameWnd* pFrame = pTemplate->CreateNewFrame(pd,pActiveChild);
	if (pFrame == NULL) 
		return NULL;

	pTemplate->InitialUpdateFrame(pFrame,pd);
	return pFrame;
}

void CPDImagemView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPDImagemDoc* pDocIn = GetDocument();   
	if (pDocIn->Imagem.Region)
	{
		SetCapture();
		Captured = TRUE;                                    

		mouseInic = point;
		pDocIn->Imagem.rect = CRect(point.x,point.y,point.x,point.y);

		if (pDocIn->Imagem.pRgn == NULL)
		{
			pDocIn->Imagem.rgn.CreateRectRgnIndirect(pDocIn->Imagem.rect);
			pDocIn->Imagem.pRgn = &pDocIn->Imagem.rgn;
		} 
		else
		{
			pDocIn->Imagem.rgn.DeleteObject();        
			pDocIn->Imagem.rgn.CreateRectRgnIndirect(pDocIn->Imagem.rect);
			pDocIn->Imagem.pRgn = &pDocIn->Imagem.rgn;
		}

		::SetCursor(::LoadCursor(NULL,IDC_CROSS));
	}    
}

void CPDImagemView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CPDImagemDoc* pDocIn = GetDocument(); 

	if (pDocIn->Imagem.Region)
	{
		CPoint ptLeft, ptRight;
		ptLeft	= pDocIn->Imagem.rect.TopLeft(); 
		ptRight	= pDocIn->Imagem.rect.BottomRight(); 

		if ((ptLeft.y >= ptRight.y) || (ptLeft.x >= ptRight.x))
			return;

		CString TitleWindow;
		HCURSOR OldCursor;
		OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 
		CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
		CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
		pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyRegion();
		pDocOut->Imagem.InitDIBData(); 
		TitleWindow = "Regiao"; 
		pDocOut->SetTitle(TitleWindow);
		CFrameWnd* pFrameO;
		pFrameO = CreateChild(pDocOut, pDocOut);
		SetCursor(OldCursor);
	}      
}

void CPDImagemView::OnLButtonUp(UINT nFlags, CPoint point) 
{ 
	CPDImagemDoc* pDocIn = GetDocument();
	if (pDocIn->Imagem.Region)
	{
		if (Captured)
		{
			Captured = FALSE;
			ReleaseCapture();
		}
	}
}

void CPDImagemView::OnMouseMove(UINT nFlags, CPoint point)
{ 
	CPDImagemDoc* pDocIn = GetDocument();

	if (pDocIn->Imagem.Region)
	{
		if (Captured)
		{
			pDocIn->Imagem.rect = CRect(mouseInic.x,mouseInic.y,point.x,point.y);
			Invalidate(FALSE);
		}
	}         
}


void CPDImagemView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CPDImagemDoc* pDocIn = GetDocument();
	if (pDocIn->Imagem.Pinta)
	{
		HBITMAP HAux;
		CDC DcAux, *pDC;  
		pDC = GetDC();
		DcAux.CreateCompatibleDC(NULL);
		HAux = CreateDIBitmap(pDC->m_hDC,pDocIn->Imagem.lpBI,CBM_INIT,pDocIn->Imagem.lpBits,pDocIn->Imagem.lpbmi,DIB_COR);
		
		if((::SelectObject(DcAux.m_hDC, HAux)) != NULL)
		{
			BlackBrush = new CBrush(RGB(0,0,0));
			OldBrush = (CBrush*) DcAux.SelectObject(BlackBrush); 
			DcAux.FloodFill(point.x, point.y, RGB(0,0,0));
			DcAux.SelectObject(OldBrush);
			delete BlackBrush;
			GetDIBits(DcAux.m_hDC,HAux,0,(WORD) pDocIn->Imagem.GetHeight(),pDocIn->Imagem.lpBits,
			pDocIn->Imagem.lpbmi,DIB_COR);      
		}
		::ReleaseDC(NULL,DcAux.m_hDC);
		ReleaseDC(pDC);
		DcAux.DeleteDC();
		if (HAux != NULL)
			DeleteObject(HAux);
		Invalidate(FALSE);
	}   
}

BOOL CPDImagemView::OnPreparePrinting(CPrintInfo* pInfo)
{
//	default preparation
	return DoPreparePrinting(pInfo);
}

void CPDImagemView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPDImagemView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}               

// Rotina para atender ao fechamento de caixas de dialogo
LONG CPDImagemView::OnGoodBye(UINT wParam, LONG lParam)
{
	if (wParam == 1)
		Pok = TRUE;
	else
		Pok = FALSE;     

	if (m_pDlgElemEstr1 != NULL)
	{
		m_pDlgElemEstr1->DestroyWindow();
		m_pDlgElemEstr1 = NULL;      
	}

	if (m_pDlgElemEstr2 != NULL)
	{
		m_pDlgElemEstr2->DestroyWindow();
		m_pDlgElemEstr2 = NULL;      
	}       

	if (m_pDlgFatorZoom != NULL)
	{
		m_pDlgFatorZoom->DestroyWindow();
		m_pDlgFatorZoom = NULL;      
	}       

	if (m_pDlgHistograma != NULL)
	{
		m_pDlgHistograma->DestroyWindow();
		m_pDlgHistograma = NULL;      
	}       

	if (m_pDlgLimiarManual != NULL)
	{
		m_pDlgLimiarManual->DestroyWindow();
		m_pDlgLimiarManual = NULL;
	}

	if (m_pDlgOpLogica != NULL)
	{
		m_pDlgOpLogica->DestroyWindow();
		m_pDlgOpLogica = NULL;      
	}
	return 0L;
}    

LRESULT CPDImagemView::OnDoRealize(WPARAM wParam, LPARAM)
{
	ASSERT(wParam != NULL);
	CPDImagemDoc* pDocIn = GetDocument();
	if (pDocIn->Imagem.hDIB == NULL)
		return 0L;  // must be a new document

//  CPalette* pPal = pDocIn->GetDocPalette(); 
	if (pDocIn->Imagem.pal != NULL)
	{                                
		CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
		ASSERT(pAppFrame->IsKindOf(RUNTIME_CLASS( CMainFrame )));

		CClientDC appDC(pAppFrame);
		// All views but one should be a background palette.
		// wParam contains a handle to the active view, so the SelectPalette
		// bForceBackground flag is FALSE only if wParam == m_hWnd (this view)
		CPalette* oldPalette = appDC.SelectPalette(pDocIn->Imagem.pal, ((HWND)wParam) != m_hWnd);

		if (oldPalette != NULL)
		{
			UINT nColorsChanged = appDC.RealizePalette();
			if (nColorsChanged > 0)
			pDocIn->UpdateAllViews(NULL);
			appDC.SelectPalette(oldPalette, TRUE);
		}
		else
		{
			TRACE0("\tSelectPalette failed in CDibView::OnPaletteChanged\n");
		}
	}

	return 0L;
}

void CPDImagemView::OnInitialUpdate()
{   
    CScrollView::OnInitialUpdate();
    ASSERT(GetDocument() != NULL);
//  SetWindowPos(NULL,0,0,GetDocument()->Imagem.m_sizeDOC.cy,GetDocument()->Imagem.m_sizeDOC.cx,
//               SWP_NOZORDER);
                                     
    SetScrollSizes(MM_TEXT, GetDocument()->Imagem.m_sizeDOC);
	ResizeParentToFit(FALSE);
}

//CPDImagemView herda de CView o atributo m_pDocument que é retornado quando uma classe solicita, geralmente para fazer uma cópia do documento
CPDImagemDoc* CPDImagemView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPDImagemDoc)));
    return (CPDImagemDoc*) m_pDocument;
}

void CPDImagemView::OnActivateView(BOOL bActivate, CView* pActivateView,CView* pDeactiveView)
{
    CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);

    if (bActivate)
    {
        ASSERT(pActivateView == this);
        OnDoRealize((WPARAM)m_hWnd, 0);   // same as SendMessage(WM_DOREALIZE);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CPDImagemView diagnostics

#ifdef _DEBUG
void CPDImagemView::AssertValid() const
{
    CView::AssertValid();
}

void CPDImagemView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

/*
CPDImagemDoc* CPDImagemView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPDImagemDoc)));
    return (CPDImagemDoc*) m_pDocument;
}
*/
#endif //_DEBUG

void CPDImagemView::OnMostra() 
{
	// TODO: Add your control notification handler code here

}

//HSV - Método Tradicional

void CPDImagemView::OnHsvExtrairCanais() {
	CPDImagemDoc* pDocIn = GetDocument();
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	int h = pDocIn->Imagem.GetHeight();
	int w = pDocIn->Imagem.GetWidth();
	int b = pDocIn->Imagem.GetBitsPerPixel()/8;
	int size = h*w*b;
	int i;

	BYTE* lpTemp = pDocIn->Imagem.lpBits;

	double *matriz = new double[size];
	double maior[3] = {-360, -360, -360};
	double menor[3] = {360, 360, 360};
	double min, max;

	for( i = 0; i < size; i +=3 ) {
		double  Canal_r = ((int)lpTemp[i + 2]);///255.0;
		double  Canal_g = ((int)lpTemp[i + 1]);///255.0;
		double  Canal_b = ((int)lpTemp[i]);///255.0;

		// Passo 1: Calcula-se os valores máximo e mínimo
		max = max(Canal_r,max(Canal_g,Canal_b));
		min = min(Canal_r,min(Canal_g,Canal_b));

		// Passo 2: Calcula-se R', G' e B'
		double Canal_r1 = (max-Canal_r)/(max-min);
		double Canal_g1 = (max-Canal_g)/(max-min);
		double Canal_b1 = (max-Canal_b)/(max-min);
		
		double Canal_h, Canal_s, Canal_v = 0;

		// Passo 3: Calcula-se S e V
		Canal_s = (max - min)/max;
		Canal_v = max/255;

		// Passo 4: Calcula-se a cor (ou H)
		if (max==min) // H é indefinido
			Canal_h = 0;
		else {
			if ((Canal_r==max) && (Canal_g==min))
				Canal_h = 5 +  Canal_b1;
			else if ((Canal_r==max) && (Canal_g!=min))
				Canal_h = 1 -  Canal_g1;
			else if ((Canal_g==max) && (Canal_b==min))
				Canal_h =  Canal_r1 + 1;
			else if ((Canal_g==max) && (Canal_b!=min))
				Canal_h = 3 -  Canal_b1;
			else if ((Canal_r)==max)
				Canal_h = 3 +  Canal_g1;
			else
				Canal_h = 5 -  Canal_r1;
		}

		// Armazenando os valores calculados na matriz
		matriz[i+2] = Canal_h*255.0;
		matriz[i+1] = Canal_s;
		matriz[i]   = Canal_v;

		// Atualizando maior e menor
		for (int j=0; j<3; j++) {
			if (maior[j] < matriz[i+j])
				maior[j] = matriz[i+j];
			if (menor[j] > matriz[i+j])
				menor[j] = matriz[i+j];
		}
	}

	// Realizando a normalização e apresentando na tela
	for (int j=0; j<3; j++) {
		// Criando as referências e Janelas
		CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument(); 
		
		pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle();
		pDocOut->Imagem.InitDIBData();

		for( i = 0; i < size; i +=3 ) {
			double res = matriz[i+j];
			
			//Verifica o tipo de cor, abrangendo todas as cores do espectro, desde o vermelho até o violeta, mais o magenta. Atinge valores de 0 a 360, 
			//mas para algumas aplicações, esse valor é normalizado de 0 a 100%. 
			int novaCor =  (int) NormalizaCores(255,0,res,menor[j],maior[j]);
	
			pDocOut->Imagem.lpBits[i] = (BYTE) (novaCor);
			pDocOut->Imagem.lpBits[i+1] = (BYTE) (novaCor);
			pDocOut->Imagem.lpBits[i+2] = (BYTE) (novaCor);
		}

		// Obtendo o nome da janela
		char* TitleWindow = "";
		switch (j) {
			case 0 : TitleWindow = "HSV - Canal V";
					 break;
			case 1 : TitleWindow = "HSV - Canal S";
					 break;
			case 2 : TitleWindow = "HSV - Canal H";
					 break;
		}
		pDocOut->SetTitle(TitleWindow);
		CFrameWnd* pFrameO;
		pFrameO = CreateChild(pDocOut, pDocOut);
	}

	// Reconstruindo a imagem RGB a partir dos componentes HSV
	// Criando as referências e Janelas
	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument(); 
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle();
	pDocOut->Imagem.InitDIBData();

	for( i = 0; i < size; i +=3 ) {
		double Canal_h = matriz[i+2]/360;
		double Canal_s = matriz[i+1];
		double Canal_v = matriz[i];

		double Canal_r, Canal_g, Canal_b;

		int primaryColor;
		double hex, secondaryColor, a, b, c = 0;
		hex = Canal_h / 60;
		primaryColor = (int)floor(hex);
		secondaryColor = hex - primaryColor;
		a = Canal_v * (1 - Canal_s);
		b = Canal_v * (1 - (Canal_s * secondaryColor));
		c = Canal_v * (1 - (Canal_s * (1 - secondaryColor)));

		switch (primaryColor) {
			case 0: 
				Canal_r = Canal_v;
				Canal_g = c;
				Canal_b = a;
				break;
			case 1:
				Canal_r = b;
				Canal_g = Canal_v;
				Canal_b = a;
				break;
			case 2:
				Canal_r = a;
				Canal_g = Canal_v;
				Canal_b = c;
				break;
			case 3:
				Canal_r = a;
				Canal_g = b;
				Canal_b = Canal_v;
				break;
			case 4:
				Canal_r = c;
				Canal_g = a;
				Canal_b = Canal_v;
				break;
			case 5:
				Canal_r = Canal_v;
				Canal_g = a;
				Canal_b = b;
				break;
		}
	}

	pDocOut->SetTitle("Reconstrução HSV -> RGB");
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);

	delete(matriz);

}

//HSV - Método do artigo de Darrin Cardani

void CPDImagemView::OnHsvExtrairCanais_Cardani() {
	CPDImagemDoc* pDocIn = GetDocument();
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	int h = pDocIn->Imagem.GetHeight();
	int w = pDocIn->Imagem.GetWidth();
	int b = pDocIn->Imagem.GetBitsPerPixel()/8;
	int size = h*w*b;
	int i;

	BYTE* lpTemp = pDocIn->Imagem.lpBits;

	double *matriz = new double[size];
	double maior[3] = {-360, -360, -360};
	double menor[3] = {360, 360, 360};

	for( i = 0; i < size; i +=3 ) {
		double R = ((int)lpTemp[i + 2])/255.0;
		double G = ((int)lpTemp[i + 1])/255.0;
		double B = ((int)lpTemp[i])/255.0;
		
		double h, s, v = 0;

		// Passo 1: Calcula-se o valor do componente value
		double max, min;
		if (R>G) {
			max = R;
			min = G;
		}
		else {
			max = G;
			min = R;
		}
		if (B>max)
			max = B;
		if (B<min)
			min = B;

		v = max;


		// Passo 2: Calcula-se o valor do componente saturation
		if (max != 0)
			s = ((max-min)/max) * 255;
		else
			s = 0;
		
		// Passo 3: Calcula-se o valor do componente H
		if (s ==0)
			h = 0;
		else {
			double dHue, dDelta = 0;
			dDelta = max - min;

			if (R==max)
				dHue = (double)(G-B)/dDelta;
			else {
				if (G==max)
					dHue = 2.0 * (B-R)/dDelta;
				else
					dHue = 4.0 * (R-G)/dDelta;
			}
			dHue *= 60.0;
			if (dHue < 0)
				dHue += 360;
			h = dHue;
		}

		// Armazenando os valores calculados na matriz
		matriz[i+2] = h*255.0;
		matriz[i+1] = s;
		matriz[i]   = v;

		// Atualizando maior e menor
		for (int j=0; j<3; j++) {
			if (maior[j] < matriz[i+j])
				maior[j] = matriz[i+j];
			if (menor[j] > matriz[i+j])
				menor[j] = matriz[i+j];
		}
	}

	// Realizando a normalização e apresentando na tela
	for (int j=0; j<3; j++) {
		// Criando as referências e Janelas
		CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument(); 
		
		pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle();
		pDocOut->Imagem.InitDIBData();

		for( i = 0; i < size; i +=3 ) {
			double res = matriz[i+j];

			int novaCor = (int) NormalizaCores(255,0,res,menor[j],maior[j]);

			pDocOut->Imagem.lpBits[i] = (BYTE) (novaCor);
			pDocOut->Imagem.lpBits[i+1] = (BYTE) (novaCor);
			pDocOut->Imagem.lpBits[i+2] = (BYTE) (novaCor);
		}

		// Obtendo o nome da janela
		char* TitleWindow = "";
		switch (j) {
			case 0 : TitleWindow = "HSV - Canal V";
					 break;
			case 1 : TitleWindow = "HSV - Canal S";
					 break;
			case 2 : TitleWindow = "HSV - Canal H";
					 break;
		}
		pDocOut->SetTitle(TitleWindow);
		CFrameWnd* pFrameO;
		pFrameO = CreateChild(pDocOut, pDocOut);
	}

	// Reconstruindo a imagem RGB a partir dos componentes HSV
	// Criando as referências e Janelas
	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument(); 
	
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle();
	pDocOut->Imagem.InitDIBData();
	for( i = 0; i < size; i +=3 ) {
		double h = matriz[i+2]/360;
		double s = matriz[i+1];
		double v = matriz[i];

		double R, G, B;

		if (s==0) {
			R = v;
			G = v;
			B = v;
		}
		else {
			double fHue, fValue, fSaturation;
			int i;
			double f, p, q, t;

			if (h==360)
				h = 0;

			fHue = h/60;
			i = (int)fHue;
			f = fHue - (double)i;

			fValue = v;
			fSaturation = s/255;

			p = fValue * (1.0 - fSaturation);
			q = fValue * (1.0 - (fSaturation*f));
			t = fValue * (1.0 - (fSaturation*(1.0-f)));

			switch(i) {
				case 6:
				case 0:
					R = fValue ;
					G = t;
					B = p;
					break;
				case 1:
					R = q;
					G = fValue;
					B = p;
					break;
				case 2:
					R = p;
					G = fValue;
					B = t;
					break;
				case 3:
					R = p;
					G = q;
					B = fValue;
					break;
				case 4:
					R = t;
					G = p;
					B = fValue;
					break;
				case 5:
					R = fValue;
					G = p;
					B = q;
					break;
			}

		}

		pDocOut->Imagem.lpBits[i] = (BYTE) (B*255);
		pDocOut->Imagem.lpBits[i+1] = (BYTE) (G*255);
		pDocOut->Imagem.lpBits[i+2] = (BYTE) (R*255);
	}
	pDocOut->SetTitle("Reconstrução HSV -> RGB");
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);

	delete(matriz);

}

/*HSI*/
double CPDImagemView::menor(double a, double b){

	if(a < b)
		return a;
	else
		return b;
}

double CPDImagemView::maior(double a, double b){

	if(a > b)
		return a;
	else
		return b;
}

int CPDImagemView::arred(double valor){


	    double f = floor(valor);
		double dec = valor - (double) f;


		if(dec >= 0.5)
			valor = valor + 1;

		return (int) valor;

}

void CPDImagemView::OnHSI() 
{
	CString		TitleWindowH, TitleWindowS, TitleWindowI2, TitleWindowHSIRGB;
	HCURSOR		OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
	CPDImagemDoc* pDocOutH = (CPDImagemDoc*) pTemplate->CreateNewDocument();
	CPDImagemDoc* pDocOutS = (CPDImagemDoc*) pTemplate->CreateNewDocument();
	CPDImagemDoc* pDocOutI2 = (CPDImagemDoc*) pTemplate->CreateNewDocument();
	CPDImagemDoc* pDocOutHSIRGB = (CPDImagemDoc*) pTemplate->CreateNewDocument();

	

	// H
	pDocOutH->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOutH->Imagem.InitDIBData(); 
	pDocOutH->Imagem.GrayPalette();
	OnHSIExtrairH('H', 1, pDocIn, pDocOutH);
	SetCursor(OldCursor);

	TitleWindowH = "Conversão de RGB para YIQ canal H"; 
	pDocOutH->SetTitle(TitleWindowH);
	CFrameWnd* pFrameHO;
	pFrameHO = CreateChild(pDocOutH, pDocOutH);

	// S
	pDocOutS->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOutS->Imagem.InitDIBData(); 
	pDocOutS->Imagem.GrayPalette();
	OnHSIExtrairS('S', 2, pDocIn, pDocOutS);
	SetCursor(OldCursor);

	TitleWindowS = "Conversão de RGB para HSI canal S"; 
	pDocOutS->SetTitle(TitleWindowS);
	CFrameWnd* pFrameSO;
	pFrameSO = CreateChild(pDocOutS, pDocOutS);

	// I
	pDocOutI2->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOutI2->Imagem.InitDIBData(); 
	pDocOutI2->Imagem.GrayPalette();
	OnHSIExtrairI('I2', 3, pDocIn, pDocOutI2);
	SetCursor(OldCursor);

	TitleWindowI2 = "Conversão de RGB para HSI canal I"; 
	pDocOutI2->SetTitle(TitleWindowI2);
	CFrameWnd* pFrameIO;
	pFrameIO = CreateChild(pDocOutI2, pDocOutI2);

	// RGB
	pDocOutHSIRGB->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOutHSIRGB->Imagem.InitDIBData(); 
	pDocOutHSIRGB->Imagem.GrayPalette();
	OnHSItoRGB(pDocIn, pDocOutHSIRGB);
	SetCursor(OldCursor);

	TitleWindowHSIRGB = "Conversão de HSI para RGB"; 
	pDocOutHSIRGB->SetTitle(TitleWindowHSIRGB);
	CFrameWnd* pFrameHSIRGBO;
	pFrameHSIRGBO = CreateChild(pDocOutHSIRGB, pDocOutHSIRGB);

	MessageBeep(10);	
}

void CPDImagemView::OnHSIExtrairH(char tipo, int canal, CPDImagemDoc* pDocIn, CPDImagemDoc* pDocOut)
{
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 
		
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
	
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle();
	pDocOut->Imagem.InitDIBData();

	int h = pDocIn->Imagem.GetHeight();
	int w = pDocIn->Imagem.GetWidth();
	int z = pDocIn->Imagem.GetBitsPerPixel()/8;
	int size = h*w*z;
	int i, j = 0;

	double max = 0;
	double min = 1000;
	
	double  *img = new double [size];

	BYTE* lpTemp = pDocIn->Imagem.lpBits;


	for( i = 0; i < size ; i +=3 )
	{
		
		double Canal_B = (double) lpTemp[i];	
		double Canal_G = (double) lpTemp[i + 1];
		double Canal_R = (double) lpTemp[i + 2];


		double Canal_r ;
		double Canal_g ;
		double Canal_b ;

		double Canal_T = Canal_R+Canal_G+Canal_B;

		if(Canal_T == 0){

			 Canal_r = 0;
			 Canal_g = 0;
			 Canal_b = 0;
		} else {
		
			 Canal_r = Canal_R / Canal_T;
			 Canal_g = Canal_G / Canal_T;
			 Canal_b = Canal_B / Canal_T;
		}
		
		double res;


		double t1 = (Canal_r-Canal_g)+(Canal_r-Canal_b);
		double t2 = (Canal_r-Canal_b)*(Canal_g-Canal_b);
		double t3 =  Canal_r-Canal_g;
		double t4 = pow(t3,2);
		double t5 = t4+t2;
		double t6 = (0.5*t1)/sqrt(t5);
		

		if(t6 == -1){
			res	 = PI;
		}
		else {
			if(t6 == 0){
				res	 = 1.5708;
			}
			else{
				res	 = acos(t6);
			}
		}		

		if(Canal_b > Canal_g){
			res	 = 2*PI - res;			
		}

		res = res/(2*PI); // [0,1]	

		img[j] = res;		
		j++;
		max = maior(max, res);
		min = menor(min, res);
	}	

	j = 0;
	for( i = 0; i < size; i +=3 )
	{
	
		double hsi_h = (double) img[j];
		j++;	 		

		double  norm;
		norm =  hsi_h*255;	
		
		int Canal_H = arred(norm) ;
		
		pDocOut->Imagem.lpBits[i] =   Canal_H;
		pDocOut->Imagem.lpBits[i+1] = Canal_H;
		pDocOut->Imagem.lpBits[i+2] = Canal_H;	
	}
	char *	TitleWindowH = "HSI - Canal H"; 
}


void CPDImagemView::OnHSIExtrairS(char tipo, int canal, CPDImagemDoc* pDocIn, CPDImagemDoc* pDocOut)
{

	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
	
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle();
	pDocOut->Imagem.InitDIBData();

	int h = pDocIn->Imagem.GetHeight();
	int w = pDocIn->Imagem.GetWidth();
	int b = pDocIn->Imagem.GetBitsPerPixel()/8;
	int size = h*w*b;
	int i;

	double Menor;
	double min = 10;
	double max = 0;
	
	double *img = new double [size];

	BYTE* lpTemp = pDocIn->Imagem.lpBits;

	for( i = 0; i < size; i +=3 )
	{
		double Canal_R = (double) lpTemp[i + 2];
		double Canal_G = (double) lpTemp[i + 1];
		double Canal_B = (double) lpTemp[i];


		double Canal_r =  Canal_R / (Canal_R+Canal_G+Canal_B);
		double Canal_g =  Canal_G / (Canal_R+Canal_G+Canal_B);
		double Canal_b =  Canal_B / (Canal_R+Canal_G+Canal_B);

		
		double res;

		Menor = menor(Canal_r, menor(Canal_g, Canal_b));

		res	 = 1 - 3*Menor;	
		
		max = maior(max, res);
		min = menor(min, res);


		img[i] = res;
		img[i+1] = res;
		img[i+2] = res;
	
	}	

	for( i = 0; i < size; i +=3 )
	{
	
		double hsi_h = (double) img[i];
		double  norm;
		norm = 255*((hsi_h-min)/(max - min));
		int Canal_S = arred(norm);
		pDocOut->Imagem.lpBits[i] = Canal_S;
		pDocOut->Imagem.lpBits[i+1] = Canal_S;
		pDocOut->Imagem.lpBits[i+2] = Canal_S;
	}

	char * TitleWindowS = "HSI - Canal S"; 

}

double *IMG = NULL;

void CPDImagemView::OnHSIExtrairI(char tipo, int canal, CPDImagemDoc* pDocIn, CPDImagemDoc* pDocOut)
{
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
	pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle();
	pDocOut->Imagem.InitDIBData();

	int i;
	int h = pDocIn->Imagem.GetHeight();
	int w = pDocIn->Imagem.GetWidth();
	int z = pDocIn->Imagem.GetBitsPerPixel()/8;
	int size = h*w*z;

	double min = 10;
	double max = 0;
	
	double *img = new double [size];

	double *imgI = new double [size];


	BYTE* lpTemp = pDocIn->Imagem.lpBits;
		
	for( i = 0; i < size; i +=3 )
	{
		double Canal_R = (double) lpTemp[i + 2];
		double Canal_G = (double) lpTemp[i + 1];
		double Canal_B = (double) lpTemp[i];	
		
		double res;

		res	 = (Canal_R+Canal_G+Canal_B)/765;	
		img[i] = res;
		img[i+1] = res;
		img[i+2] = res;

		min = menor(min, res);
		max = maior(max, res);
	}	
	

	for( i = 0; i < size; i +=3 )
	{
	
		double hsi_h = (double) img[i];
		double  norm;

		norm = 255*hsi_h;

			int I = arred(norm);

		pDocOut->Imagem.lpBits[i] = I;
		pDocOut->Imagem.lpBits[i+1] = I;
		pDocOut->Imagem.lpBits[i+2] = I ;
	}

	IMG =imgI;

	char* TitleWindowI2 = "HSI - Canal I"; 

}


void CPDImagemView::OnHSItoRGB(CPDImagemDoc* pDocIn, CPDImagemDoc* pDocOut)
{

	HCURSOR OldCursor;
	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	if(IMG == NULL){
		MessageBox("Imagem HSI não encontrada!");
	} else{
	
			pDocOut->Imagem.hDIB = pDocIn->Imagem.CopyHandle();
			pDocOut->Imagem.InitDIBData();
			pDocOut->Imagem.OnHSItoRGB;


			int h = pDocIn->Imagem.GetHeight();
			int w = pDocIn->Imagem.GetWidth();
			int z = pDocIn->Imagem.GetBitsPerPixel()/8;
			int size = h*w*z;
			int j;
				
			double *imgHSI = new double[size];
			double min = 10;
			double max = 0;
			double *imgH = new double [size];
			double *imgS = new double [size];
			double *imgI = new double [size];

			BYTE* lpTemp = pDocIn->Imagem.lpBits;
		
			for( j = 0; j < size; j +=3 )
			{
				double Canal_H = (double) IMG[j + 2];
				double Canal_S = (double) IMG[j + 1];
				double Canal_I = (double) IMG[j];
				double Canal_r,Canal_g,Canal_b,Canal_x,Canal_y,Canal_z;

				Canal_x = Canal_I*(1-Canal_S);
				Canal_y = Canal_I*(1+((Canal_S*cos(Canal_H))/(cos((PI/3)-Canal_H))));
				Canal_z = 3*Canal_I-(Canal_x+Canal_y);

				if(Canal_H < 2*PI/3){
					Canal_r = Canal_y;
					Canal_g = Canal_z;
					Canal_b = Canal_x;

				} else{
					if(Canal_H >= 2*PI/3 && Canal_H < 4*PI/3){

						Canal_H = Canal_H - 2*PI/3;

						Canal_x = Canal_I*(1-Canal_S);
						Canal_y = Canal_I*(1+((Canal_S*cos(Canal_H))/(cos(PI/3-Canal_H))));
						Canal_z = 3*Canal_I-(Canal_x+Canal_y);

						Canal_r = Canal_x;
						Canal_g = Canal_y;
						Canal_b = Canal_z;

					} else{

						Canal_H = Canal_H - 4*PI/3;

						Canal_x = Canal_I*(1-Canal_S);
						Canal_y = Canal_I*(1+((Canal_S*cos(Canal_H))/(cos(PI/3-Canal_H))));
						Canal_z = 3*Canal_I-(Canal_x+Canal_y);

						Canal_r = Canal_z;
						Canal_g = Canal_x;
						Canal_b = Canal_y;
					
					}			
				
				}
				
				double Canal_R = Canal_r*255;
				double Canal_G = Canal_g*255;
				double Canal_B = Canal_b*255;

			}	
		
			SetCursor(OldCursor);

			char* TitleWindowHSIRGB = "RGB"; 		
	}
}

void CPDImagemView::OnRGBtoXYZ()
{
	
	BYTE* lpTemp[3];
	CString TitleWindow[3];
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pdO[4];

	
	for ( int i = 0; i < 3; i++ )
	{
		pdO[i] = (CPDImagemDoc*) pTemplate->CreateNewDocument();                                  
		pdO[i]->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
		pdO[i]->Imagem.InitDIBData(); 
		pdO[i]->Imagem.RgbtoXyz;
	}
		pdO[3] = (CPDImagemDoc*) pTemplate->CreateNewDocument();                                  
		pdO[3]->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
		pdO[3]->Imagem.InitDIBData(); 
		pdO[3]->Imagem.RgbtoXyz;
	
	int h,w;


	for( int i = 0; i < 3; i++)
	{
		lpTemp[i] = pdO[i]->Imagem.lpBits;
	}

	h = pdO[0]->Imagem.GetHeight();
	w = pdO[0]->Imagem.GetWidth();

	for( int i = 0; i < h*(w*3); i = i + 3)
	{
		lpTemp[2][i] = lpTemp[2][i];
		lpTemp[2][i+1] = lpTemp[2][i];
		lpTemp[2][i+2] = lpTemp[2][i];

		lpTemp[1][i] = lpTemp[1][i + 1];
		lpTemp[1][i+1] = lpTemp[1][i + 1];
		lpTemp[1][i+2] = lpTemp[1][i + 1];

		lpTemp[0][i] = lpTemp[0][i + 2];
		lpTemp[0][i+1] = lpTemp[0][i + 2];
		lpTemp[0][i+2] = lpTemp[0][i + 2];
	}

	CFrameWnd* pFrameE;
	SetCursor(OldCursor);

	TitleWindow[0] = "Canal X";
	TitleWindow[1] = "Canal Y";
	TitleWindow[2] = "Canal Z";
	

	for( int i = 0; i < 3; i++)
	{
		pdO[i]->SetTitle(TitleWindow[i]);
		pFrameE = CreateChild(pdO[i], pdO[i]);
		SetCursor(OldCursor);
	}	
	
	
		pdO[3]->SetTitle("XZY");
		pFrameE = CreateChild(pdO[3], pdO[3]);
		SetCursor(OldCursor);
}


/* Converte de RGB para YCrCb e apresenta os canais em separado*/
void CPDImagemView::OnRGBtoYCrCb()
{
	CString TitleWindow[4];
	HCURSOR OldCursor;
	int h, w, i;
	BYTE *lpTemp[3];

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 
	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
	
	CPDImagemDoc* pdO[4];

	for(i = 0; i < 4; i++)
	{
		pdO[i] = (CPDImagemDoc*) pTemplate->CreateNewDocument();
		pdO[i]->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
		pdO[i]->Imagem.InitDIBData(); 
	
		if(i < 3)
		{
			pdO[i]->Imagem.OnRGBToYCrCb();
		}
		else
		{
			pdO[i]->Imagem.OnYCrCbToRGB();
		}

	}

	for(i = 0; i < 3; i++)
	{
		lpTemp[i] = pdO[i]->Imagem.lpBits;
	}

	h = pdO[0]->Imagem.GetHeight();
	w = pdO[0]->Imagem.GetWidth();

	for(i = 0; i < h*(w*3); i = i + 3)
	{
		lpTemp[2][i] = lpTemp[2][i];
		lpTemp[2][i+1] = lpTemp[2][i];
		lpTemp[2][i+2] = lpTemp[2][i];

		lpTemp[1][i] = lpTemp[1][i + 1];
		lpTemp[1][i+1] = lpTemp[1][i + 1];
		lpTemp[1][i+2] = lpTemp[1][i + 1];

		lpTemp[0][i] = lpTemp[0][i + 2];
		lpTemp[0][i+1] = lpTemp[0][i + 2];
		lpTemp[0][i+2] = lpTemp[0][i + 2];
	}

	CFrameWnd* pFrameE;
	SetCursor(OldCursor);

	TitleWindow[0] = "Canal Y";
	TitleWindow[1] = "Canal Cr";
	TitleWindow[2] = "Canal Cb";
	TitleWindow[3] = "YCrCb to RGB";

	for(i = 0; i < 4; i++)
	{
		pdO[i]->SetTitle(TitleWindow[i]);
		pFrameE = CreateChild(pdO[i], pdO[i]);
		SetCursor(OldCursor);
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
///YIQ
//////////////////////////////////////////////////////////////////////////////////////////////////
void CPDImagemView::ConvertRGBtoYIQ (char tipo, int canal, CPDImagemDoc* pDocIn, CPDImagemDoc* pDocOut)
{


	int		col, lin, c1, c2, c3, novaCor, count=0;
	double  y, i, q, cor, menor, maior;
	int teste = 0;
	teste = (int) (pDocIn->Imagem.GetHeight()*pDocIn->Imagem.bmWidthBytes);
	matrizY = new double[teste];
	matrizI = new double[teste];
	matrizQ = new double[teste];

	if (tipo == 'Y') MaiorMenor(&maior, &menor, pDocIn, 'Y');
	if (tipo == 'I') MaiorMenor(&maior, &menor, pDocIn, 'I');
	if (tipo == 'Q') MaiorMenor(&maior, &menor, pDocIn, 'Q');

	for (lin = 0; lin < (int) pDocIn->Imagem.GetHeight(); lin++)  // uma linha
		for (col = 0; col < (int) pDocIn->Imagem.GetWidth(); col++)
		{
			cor = pDocIn->Imagem.GetPixel((DWORD) col,(DWORD) lin);
			
			CalculaYIQ(cor, &y, &i, &q);

			matrizY[count] = y;
			matrizI[count] = i;
			matrizQ[count] = q;
			count++;

			c1 = (int) NormalizaCores(255,0,y,menor,maior); 
			c2 = (int) NormalizaCores(255,0,i,menor,maior); 
			c3 = (int) NormalizaCores(255,0,q,menor,maior);

			if (canal == 1)	novaCor = RGB(c1,c1,c1);
			if (canal == 2)	novaCor = RGB(c2,c2,c2);
			if (canal == 3)	novaCor = RGB(c3,c3,c3);
			pDocOut->Imagem.SetPixel((DWORD) col,(DWORD) lin, novaCor);
		}
}

//E abaixo segue o código fonte responsável pela conversão YIQ para RGB onde constatamos que nossos cálculos estão corretos:

void CPDImagemView::CalculaRGB(double *r, double *g, double *b, int count) {

	const double YIQtoRGB[3][3] = {	{  1,  0.956,  0.621 },
									{  1, -0.272, -0.647 },
									{  1, -1.105,  1.702 }	};
	double y, i, q;

	y = matrizY[count];
	i = matrizI[count];
	q = matrizQ[count];

	// transforma para YIQ
	*r = (double)(YIQtoRGB[0][0]*y + YIQtoRGB[0][1]*i + YIQtoRGB[0][2]*q);
	*g = (double)(YIQtoRGB[1][0]*y + YIQtoRGB[1][1]*i + YIQtoRGB[1][2]*q);
	*b = (double)(YIQtoRGB[2][0]*y + YIQtoRGB[2][1]*i + YIQtoRGB[2][2]*q);
}


void CPDImagemView::ConvertYIQtoRGB(CPDImagemDoc* pDocIn, CPDImagemDoc* pDocOut)
{	int		col, lin, novaCor, count=0;
	double  r, g, b, cor;

	for (lin = 0; lin < (int) pDocIn->Imagem.GetHeight(); lin++)  // uma linha
		for (col = 0; col < (int) pDocIn->Imagem.GetWidth(); col++)
		{
			
			CalculaRGB(&r, &g, &b, count);
			count++;

			if (r > 255 ) r = 255;
			if (g > 255 ) g = 255;
			if (b > 255 ) b = 255;

			if (r < 0 ) r = 0;
			if (g < 0 ) g = 0;
			if (b < 0 ) b = 0;

			novaCor = RGB(r,g,b);
			pDocOut->Imagem.SetPixel((DWORD) col,(DWORD) lin, novaCor);
		}

}


void CPDImagemView::CalculaYIQ(double cor, double *y, double *i, double *q){

	const double RGBtoYIQ[3][3] = {	{  0.299,  0.587,  0.114 },
									{  0.596, -0.275, -0.321 },
									{  0.212, -0.523,  0.311 }	};
	float   r, g, b;

			r = GetRValue((int)cor);
			g = GetGValue((int)cor);
			b = GetBValue((int)cor);
			// transforma para YIQ
			*y = (double)(RGBtoYIQ[0][0]*r + RGBtoYIQ[0][1]*g + RGBtoYIQ[0][2]*b);
			*i = (double)(RGBtoYIQ[1][0]*r + RGBtoYIQ[1][1]*g + RGBtoYIQ[1][2]*b);
			*q = (double)(RGBtoYIQ[2][0]*r + RGBtoYIQ[2][1]*g + RGBtoYIQ[2][2]*b);
}

void CPDImagemView::OnYiq() 
{
	CString		TitleWindowY, TitleWindowI, TitleWindowQ, TitleWindowRGB;
	HCURSOR		OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
	CPDImagemDoc* pDocOutY = (CPDImagemDoc*) pTemplate->CreateNewDocument();
	CPDImagemDoc* pDocOutI = (CPDImagemDoc*) pTemplate->CreateNewDocument();
	CPDImagemDoc* pDocOutQ = (CPDImagemDoc*) pTemplate->CreateNewDocument();
	CPDImagemDoc* pDocOutRGB = (CPDImagemDoc*) pTemplate->CreateNewDocument();


	// Y
	pDocOutY->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOutY->Imagem.InitDIBData(); 
	pDocOutY->Imagem.GrayPalette();
	ConvertRGBtoYIQ('Y', 1, pDocIn, pDocOutY);
	SetCursor(OldCursor);

	TitleWindowY = "Conversão de RGB para YIQ canal Y"; 
	pDocOutY->SetTitle(TitleWindowY);
	CFrameWnd* pFrameYO;
	pFrameYO = CreateChild(pDocOutY, pDocOutY);

	// I
	pDocOutI->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOutI->Imagem.InitDIBData(); 
	pDocOutI->Imagem.GrayPalette();
	ConvertRGBtoYIQ('I', 2, pDocIn, pDocOutI);
	SetCursor(OldCursor);

	TitleWindowI = "Conversão de RGB para YIQ canal I"; 
	pDocOutI->SetTitle(TitleWindowI);
	CFrameWnd* pFrameIO;
	pFrameIO = CreateChild(pDocOutI, pDocOutI);

	// Q
	pDocOutQ->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOutQ->Imagem.InitDIBData(); 
	pDocOutQ->Imagem.GrayPalette();
	ConvertRGBtoYIQ('Q', 3, pDocIn, pDocOutQ);
	SetCursor(OldCursor);

	TitleWindowQ = "Conversão de RGB para YIQ canal Q"; 
	pDocOutQ->SetTitle(TitleWindowQ);
	CFrameWnd* pFrameQO;
	pFrameQO = CreateChild(pDocOutQ, pDocOutQ);

	// RGB
	pDocOutRGB->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOutRGB->Imagem.InitDIBData(); 
	pDocOutRGB->Imagem.GrayPalette();
	ConvertYIQtoRGB(pDocIn, pDocOutRGB);
	SetCursor(OldCursor);

	TitleWindowRGB = "Conversão de YIQ para RGB"; 
	pDocOutRGB->SetTitle(TitleWindowRGB);
	CFrameWnd* pFrameRGBO;
	pFrameRGBO = CreateChild(pDocOutRGB, pDocOutRGB);

	MessageBeep(10);
}

/*******************************************************************************************************************
*OnImagemEqualizacaoYIQ - Função chamada ao clicar no ítem de menu                                                 *
*Parâmetros:                                                                                                       *
*Retorno:                                                                                                          *
*Criação: Jean Felipe Felsky                                     Data: 06/04/2010                                  *
*Última alteração: Jean Felipe Felsky                            Data: 14/04/2010                                  *
*******************************************************************************************************************/
void CPDImagemView::OnImagemEqualizacaoYIQ()
{

	CString TitleWindowY;
	CString TitleWindowRGB;
	HCURSOR OldCursor;
	int opcao = 1;
		
	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
	CPDImagemDoc* pDocOutY = (CPDImagemDoc*) pTemplate->CreateNewDocument();
	CPDImagemDoc* pDocOutI = (CPDImagemDoc*) pTemplate->CreateNewDocument();
	CPDImagemDoc* pDocOutQ = (CPDImagemDoc*) pTemplate->CreateNewDocument();
	CPDImagemDoc* pDocOutRGB = (CPDImagemDoc*) pTemplate->CreateNewDocument();	
	
	pDocOutY->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOutY->Imagem.InitDIBData(); 
	pDocOutY->Imagem.GrayPalette();
	ConvertRGBtoYIQ('Y', 1, pDocIn, pDocOutY);
	pDocOutY->Imagem.EqualizaHistogramaYIQ('Y', matrizY);
	SetCursor(OldCursor);

	TitleWindowY = "Conversão de RGB para YIQ canal Y"; 
	pDocOutY->SetTitle(TitleWindowY);
	CFrameWnd* pFrameYO;
	pFrameYO = CreateChild(pDocOutY, pDocOutY);

	// RGB
	pDocOutRGB->Imagem.hDIB = pDocOutY->Imagem.CopyHandle(); 
	pDocOutRGB->Imagem.InitDIBData(); 
	pDocOutRGB->Imagem.GrayPalette();
	ConvertYIQtoRGB(pDocOutY, pDocOutRGB);
	SetCursor(OldCursor);

	TitleWindowRGB = "Conversão de YIQ para RGB"; 
	pDocOutRGB->SetTitle(TitleWindowRGB);
	CFrameWnd* pFrameRGBO;
	pFrameRGBO = CreateChild(pDocOutRGB, pDocOutRGB);


}

void CPDImagemView::OnImagemZoom()
{
	int 		largurazoom, alturazoom;
	int			pontoA, pontoB, pontoC, pontoD;
	int 		colzoom, linzoom, colinteiro, lininteiro,coloriginal,linoriginal;
	double		deltacol, deltalin,ContribAeB, ContribCeD;
	int			width, height;

	CString		TitleWindow;
	HCURSOR		OldCursor;

	double		fatorampliacao_l;//fator de ampliação na horizontal, largura
	double		fatorampliacao_h;//fator de ampliação na vertical, altura

	m_pDlgFatorZoom = new CDlgFatorZoom(this, &fatorampliacao_h, &fatorampliacao_l); 
	
	if(m_pDlgFatorZoom->DoModal() == IDCANCEL) 
		return;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT));
	//instancio um doc e pego o documento atual
	CPDImagemDoc* pDocIn = GetDocument();
	//instancio um template e pego o template do doc atual
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
	// instancio um doc de saída e através do template anterior, crio um novo documento
	CPDImagemDoc* pDocOut = (CPDImagemDoc*) pTemplate->CreateNewDocument();

	//char buf[30];
	//do doc de entrada, pego a altura e largura
	width	= (int) pDocIn->Imagem.GetWidth();
	height	= (int) pDocIn->Imagem.GetHeight();

	//faço uma operação simples para ter a altura e largura final do doc de saída
	// porque somar mais .5 na largura do zoom, embora isso vá se perder se os fatores de ampliação forem inteiros
	largurazoom	= (int)(fatorampliacao_l * width	+ 0.5);		// inicializacao
	alturazoom	= (int)(fatorampliacao_h * height	+ 0.5);  
	// agora já tem o tamanho da imagem final, coloca esse valor no dib de saída
	pDocOut->Imagem.hDIB = pDocIn->Imagem.AumentaDIB(alturazoom, largurazoom);
	pDocOut->Imagem.InitDIBData();

	//nova largura da imagem setada na variavel imagem da saída
	DWORD bmWidthBytesNew = 4 * ((largurazoom * pDocIn->Imagem.GetBitsPerPixel() + 31) / 32 );

	pDocOut->Imagem.bmWidthBytes = bmWidthBytesNew;

	double		resultadointer;
	for (linzoom = 0; linzoom < alturazoom; linzoom++)  
	{       
		linoriginal	= (int) (linzoom / fatorampliacao_h) ;//obtendo submultiplos do meu fator de ampliação vertical
		lininteiro	= linoriginal;
		deltalin	= (double) (linoriginal - lininteiro);

		for (colzoom = 0; colzoom < largurazoom; colzoom++)
		{
			coloriginal =(int) (colzoom / fatorampliacao_l);	       
			colinteiro = coloriginal;               
			deltacol = (double) (coloriginal - colinteiro);
/*
			for(int i = 0; i< fatorampliacao_h; i++)
			{
				if
			}
*/			if( (colinteiro < (int) width) && (lininteiro < height) )
			{       
				pontoA = (int) *(pDocIn->Imagem.lpBits+lininteiro*pDocIn->Imagem.bmWidthBytes + colinteiro);
				// se está dentro da boundary da figura original
				if( (colinteiro+1 < width) && (lininteiro+1 < height) )
				{       
					pontoB = (int) *(pDocIn->Imagem.lpBits+lininteiro*pDocIn->Imagem.bmWidthBytes + colinteiro+1);
					pontoC = (int) *(pDocIn->Imagem.lpBits+(lininteiro+1)*pDocIn->Imagem.bmWidthBytes + colinteiro);
					pontoD = (int) *(pDocIn->Imagem.lpBits+(lininteiro+1)*pDocIn->Imagem.bmWidthBytes+ colinteiro+1);	  

					ContribAeB		= deltacol * ((double)pontoB - pontoA) + pontoA;
					ContribCeD		= deltacol * ((double)pontoD - pontoC) + pontoC;
					resultadointer	= (double) (0.5 + ContribAeB + (ContribCeD - ContribAeB)*deltalin);
					*(pDocOut->Imagem.lpBits+linzoom*bmWidthBytesNew + colzoom) = (resultadointer < 128.) ? 0 : 255; // caso imagem binaria  
					*(pDocOut->Imagem.lpBits+linzoom*pDocOut->Imagem.bmWidthBytes/*New*/ + colzoom) = 0.5 + ContribAeB + (ContribCeD - ContribAeB)*deltalin;
				}
				else
					*(pDocOut->Imagem.lpBits+linzoom*bmWidthBytesNew + colzoom) = pontoA ;
			}
		}
	}
     
//	pDocOut->Imagem.ImagemZoom(&pDocIn, &pDocOut);
	SetCursor(OldCursor);
	TitleWindow = "ZOOM"; 
	pDocOut->SetTitle(TitleWindow);
	CFrameWnd* pFrameO;
	pFrameO = CreateChild(pDocOut, pDocOut);
}

void CPDImagemView::OnAprimoramentoNitidez() {

	CString TitleWindowY;
	CString TitleWindowY2;
	CString TitleWindowRGB;
	HCURSOR OldCursor;
	int opcao = 1;
		
	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();
	CPDImagemDoc* pDocOutY = (CPDImagemDoc*) pTemplate->CreateNewDocument();
	CPDImagemDoc* pDocOutY2 = (CPDImagemDoc*) pTemplate->CreateNewDocument();
	CPDImagemDoc* pDocOutRGB = (CPDImagemDoc*) pTemplate->CreateNewDocument();	

	//YIQ alterando
	pDocOutY->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pDocOutY->Imagem.InitDIBData(); 
	pDocOutY->Imagem.GrayPalette();
	ConvertRGBtoYIQ('Y', 1, pDocIn, pDocOutY);
	pDocOutY->Imagem.AprimoraNitidezYIQ(matrizY);
	pDocOutY->Imagem.EqualizaHistogramaYIQmatriz('Y', matrizY);
	SetCursor(OldCursor);

	TitleWindowY = "Conversão de RGB para YIQ canal Y"; 
	pDocOutY->SetTitle(TitleWindowY);
	CFrameWnd* pFrameYO;
	pFrameYO = CreateChild(pDocOutY, pDocOutY);


	// RGB
	pDocOutRGB->Imagem.hDIB = pDocOutY->Imagem.CopyHandle(); 
	pDocOutRGB->Imagem.InitDIBData(); 
	pDocOutRGB->Imagem.GrayPalette();
	ConvertYIQtoRGB(pDocOutY, pDocOutRGB);
	SetCursor(OldCursor);

	TitleWindowRGB = "Conversão de YIQ para RGB"; 
	pDocOutRGB->SetTitle(TitleWindowRGB);
	CFrameWnd* pFrameRGBO;
	pFrameRGBO = CreateChild(pDocOutRGB, pDocOutRGB);


}
void CPDImagemView::OnAprimoraNitidezCalculo1()
{
	double P1;
	m_pDlgAprimoraNitidezCalculo1 = new CDlgAprimoraNitidezCalculo1(this, &P1); 
	
	if(m_pDlgAprimoraNitidezCalculo1->DoModal() == IDCANCEL) 
		return;

    
	CString TitleWindow;
    HCURSOR OldCursor;
    
    OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

    CPDImagemDoc* pDoc  = GetDocument();  
    CDocTemplate* pTemplate = pDoc ->GetDocTemplate();

    CPDImagemDoc* pdO = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                           
    pdO->Imagem.hDIB = pDoc ->Imagem.CopyHandle(); 
    pdO->Imagem.InitDIBData(); 
    pdO->Imagem.AprimoraNitidezYCrCbCalculo1(P1);
    SetCursor(OldCursor);
    TitleWindow = "Aprimoramento de Nitidez - Cálculo 1"; 
    pdO->SetTitle(TitleWindow);
    CFrameWnd* pFrameO;
    pFrameO = CreateChild(pdO, pdO);
	
}

void CPDImagemView::OnAprimoraNitidezCalculo2()
{
	double P2;
	m_pDlgAprimoraNitidezCalculo2 = new CDlgAprimoraNitidezCalculo2(this, &P2); 
	
	if(m_pDlgAprimoraNitidezCalculo2->DoModal() == IDCANCEL) 
		return;

    
	CString TitleWindow;
    HCURSOR OldCursor;
    
    OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

    CPDImagemDoc* pDoc  = GetDocument();  
    CDocTemplate* pTemplate = pDoc ->GetDocTemplate();

    CPDImagemDoc* pdO = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                           
    pdO->Imagem.hDIB = pDoc ->Imagem.CopyHandle(); 
    pdO->Imagem.InitDIBData(); 
    pdO->Imagem.AprimoraNitidezYCrCbCalculo2(P2);
    SetCursor(OldCursor);
    TitleWindow = "Aprimoramento de Nitidez - Cálculo 2"; 
    pdO->SetTitle(TitleWindow);
    CFrameWnd* pFrameO;
    pFrameO = CreateChild(pdO, pdO);
	
}

void CPDImagemView::OnAprimoraNitidezCalculo3()
{
	double P3;
	m_pDlgAprimoraNitidezCalculo3 = new CDlgAprimoraNitidezCalculo3(this, &P3); 
	
	if(m_pDlgAprimoraNitidezCalculo3->DoModal() == IDCANCEL) 
		return;

    
	CString TitleWindow;
    HCURSOR OldCursor;
    
    OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

    CPDImagemDoc* pDoc  = GetDocument();  
    CDocTemplate* pTemplate = pDoc ->GetDocTemplate();

    CPDImagemDoc* pdO = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                           
    pdO->Imagem.hDIB = pDoc ->Imagem.CopyHandle(); 
    pdO->Imagem.InitDIBData(); 
    pdO->Imagem.AprimoraNitidezYCrCbCalculo3(P3);
    SetCursor(OldCursor);
    TitleWindow = "Aprimoramento de Nitidez - Cálculo 3"; 
    pdO->SetTitle(TitleWindow);
    CFrameWnd* pFrameO;
    pFrameO = CreateChild(pdO, pdO);
	
}

void CPDImagemView::OnImagemEqualizacaoYCrCb()
{
	CString TitleWindow;
	HCURSOR OldCursor;

	OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 

	CPDImagemDoc* pDocIn = GetDocument();  
	CDocTemplate* pTemplate = pDocIn->GetDocTemplate();

	CPDImagemDoc* pdO = (CPDImagemDoc*) pTemplate->CreateNewDocument();    
                                       
	pdO->Imagem.hDIB = pDocIn->Imagem.CopyHandle(); 
	pdO->Imagem.InitDIBData(); 
    pdO->Imagem.GrayPalette();
	pdO->Imagem.EqualizacaoYCrCb();
	SetCursor(OldCursor);

	TitleWindow = "Equalizacao no Espaço YCrCB"; 
	pdO->SetTitle(TitleWindow);
	CFrameWnd* pFrameE;
	pFrameE = CreateChild(pdO, pdO);
}




