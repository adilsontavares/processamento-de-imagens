//==============================================================
// PDImagemView.h : interface of the CPDImagemView class
//==============================================================

#ifndef _PDIMAVIEW_H_

#define _PDIMAVIEW_H_

#include "DlgElemEstr1.h"
#include "DlgElemEstr2.h"
#include "DlgFatorZoom.h"
#include "DlgFatorZoomInterpolacao.h"
#include "DlgHistograma.h"
#include "DlgLimiarManual.h"
#include "DlgOpLogica.h"
#include "DlgPRDistancia.h"
#include "DlgAprimoraNitidezCalculo1.h"
#include "DlgAprimoraNitidezCalculo2.h"
#include "DlgAprimoraNitidezCalculo3.h"

// Novos Dialogs
#include "DlgAdicionar.h"
#include "DlgFastAdaptiveContrastParameters.h"


class CPDImagemView : public CScrollView
{
protected: // create from serialization only
    CPDImagemView();
    DECLARE_DYNCREATE(CPDImagemView)
                                      
// Attributes
public:
	
	CPDImagemDoc* GetDocument();



    CPDImagemDoc*		pDocAux;

	CDlgElemEstr1*		m_pDlgElemEstr1;
	CDlgElemEstr2*		m_pDlgElemEstr2;
    CDlgFatorZoom*		m_pDlgFatorZoom;//usado especificamente no caso do OnImagemZoom()
    CDlgHistograma*		m_pDlgHistograma;
    CDlgLimiarManual*	m_pDlgLimiarManual;
    CDlgOpLogica*		m_pDlgOpLogica;
	CDlgAprimoraNitidezCalculo1* m_pDlgAprimoraNitidezCalculo1;
	CDlgAprimoraNitidezCalculo2* m_pDlgAprimoraNitidezCalculo2;
	CDlgAprimoraNitidezCalculo3* m_pDlgAprimoraNitidezCalculo3;


private:

    BOOL	Captured, CapturedP, fl_regiao, fl_separa, Pok;
    CRect	rect_ant;
    CPoint	mouseInic, mouseFim;
    CPoint	mouseInicP, mouseFimP;
    CBrush  *OldBrush, *BlackBrush; 
    CPen*	WhitePen, *OldPen;
    int		oldROP2;
    
// Implementation
public:
    virtual			~CPDImagemView();
    virtual	void	OnDraw(CDC* pDC);  // overridden to draw this view


#ifdef _DEBUG
    virtual void	AssertValid() const;
    virtual void	Dump(CDumpContext& dc) const;
#endif
    virtual void	OnInitialUpdate();
    virtual void	OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView); 

			void	OnImagemHistograma					();
//pedro			
			void	OnImagemHistogramaRGB				();

			void	OnImagemEqualizacao					();
			
			void	OnImagemLimiarizacaoOtsu			();
			void	OnImagemLimiarizacaoMedia			();
			void	OnImagemLimiarizacaoKittler			();
			void	OnImagemLimiarizacaoEntropia		();
			void	OnImagemLimiarizacaoAnisotropia		();
			void	OnImagemLimiarizacaoManual			();

			void	OnMorfologiaBinariaErosao			();
			void	OnMorfologiaBinariaDilatacao		();
			void	OnMorfologiaBinariaAbertura			();
			void	OnMorfologiaBinariaFechamento		();
			void	OnMorfologiaBinariaBordasErosao		();
			void	OnMorfologiaBinariaBordasDilatacao	();
			void	OnMorfologiaBinariaBordasDilEro		();
			void	OnMorfologiaBinariaEsqueletizacao	();
			void	OnMorfologiaBinariaUltimaErosao		();

			void	OnImagemZoom						();
			void	OnOperacoesLogicas					();
			void	OnImagemInverte						();

//pedro
			void	OnImagemTonsDeCinza					();
			void	OnImagemTonsDeCinzaNTSC				();
			void	OnLineMasksTotal					();
			void	OnLineMasksHorizontal				();
			void	OnLineMasksVertical					();
			void	OnLineMasksMais45		 	 		();
			void	OnLineMasksMenos45					();
			void	OnPointDetection					();
			void	OnPrewitt							();
			void	OnLaplacianMask						();
			void    OnNonLinearCompression				();
			void	Masks(char tipo, CPDImagemDoc* pDocIn, CPDImagemDoc* pDocOut);
			void	OnHighBoost							();
			void	OnMatrizCoOcorrencia				();
			void	OnRGBNormalizado					();
			void	OnRGBTSLCanalT  					();
			void	OnRGBTSLCanalS						();
			void	OnRGBTSLCanalL						();
			void	OnRGBGeraHistograma					();
			void	OnRGBGeraHistogramaTSL				();
			void	OnRGBLUVCanalL						();
			void	OnRGBLUVCanalU						();
			void	OnRGBLUVCanalV						();

			// Novos efeitos
			void	OnAdiciona();
			void	OnFastAdaptiveContrast();


			void	TratamentoCores(char tipo, int canal, CPDImagemDoc* pDocIn, CPDImagemDoc* pDocOut);
			double  NormalizaCores(double r2, double r1, double pe, double pe1, double pe2);
//			void	MaiorMenor(double *maior, double *menor, CPDImagemDoc* pDocIn);
			void	MaiorMenor(double *maior, double *menor, CPDImagemDoc* pDocIn, char tipo);
			double	CalculaT(double r,double g);
			double  CalculaL(int cor);
			double  CalculaS(double r, double g);
			void	CalculaLUV(double cor, double *l, double *u, double *v);

			void	ImagemZoomSi(CPDImagemDoc *pDoc, CPDImagemDoc *pdO);

			CFrameWnd* CreateChild(CPDImagemDoc* pd, CPDImagemDoc* pDoc);    

			//cores
			void OnHsvExtrairCanais();
			void OnHsvExtrairCanais_Cardani();

			void OnHSIExtrairH(char tipo, int canal, CPDImagemDoc* pDocIn, CPDImagemDoc* pDocOut);
			void OnHSIExtrairS(char tipo, int canal, CPDImagemDoc* pDocIn, CPDImagemDoc* pDocOut);
			void OnHSIExtrairI(char tipo, int canal, CPDImagemDoc* pDocIn, CPDImagemDoc* pDocOut);
			void OnHSItoRGB(CPDImagemDoc* pDocIn, CPDImagemDoc* pDocOut);
			double menor(double a, double b);
			double maior(double a, double b);
			int arred(double valor);
			void    OnRGBtoXYZ();
			void    OnRGBtoYCrCb();
			void	OnYiq();
			void	CalculaYIQ(double cor, double *Canal_y, double *Canal_i, double *Canal_q);
			void	CalculaRGB(double *r, double *g, double *b, int count);
			void    ConvertRGBtoYIQ(char tipo, int canal, CPDImagemDoc* pDocIn, CPDImagemDoc* pDocOut);
			void    ConvertYIQtoRGB(CPDImagemDoc* pDocIn, CPDImagemDoc* pDocOut);
			void 	OnImagemEqualizacaoYIQ();
			void 	OnAprimoramentoNitidez();
			void    OnAprimoraNitidezCalculo1();
			void    OnAprimoraNitidezCalculo2();
			void    OnAprimoraNitidezCalculo3();
			void    OnImagemEqualizacaoYCrCb();

			double  *matrizY, *matrizI, *matrizQ;

    // Printing support
protected:
    virtual BOOL	OnPreparePrinting	(CPrintInfo* pInfo);
    virtual void	OnBeginPrinting		(CDC* pDC, CPrintInfo* pInfo);
    virtual void	OnEndPrinting		(CDC* pDC, CPrintInfo* pInfo);
			LONG	OnGoodBye			(UINT wParam, LONG lParam);
// Generated message map functions
protected:
    //{{AFX_MSG(CPDImagemView)
    afx_msg LRESULT OnDoRealize		(WPARAM wParam, LPARAM lParam);  // user message
    afx_msg void	OnRButtonDown	(UINT nFlags, CPoint point);
    afx_msg void	OnLButtonDown	(UINT nFlags, CPoint point);
    afx_msg void	OnLButtonUp		(UINT nFlags, CPoint point);
    afx_msg void	OnLButtonDblClk	(UINT nFlags, CPoint point);
    afx_msg void	OnMouseMove		(UINT nFlags, CPoint point);
    afx_msg void	OnUpdateSepara		(CCmdUI* pCmdUI);
    afx_msg void	OnUpdateRegiao		(CCmdUI* pCmdUI);
	afx_msg void OnMostra();
	afx_msg	void OnHSI										();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

#endif
