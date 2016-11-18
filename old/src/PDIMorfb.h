 
#ifndef PDIMORFB_H

#define PDIMORFB_H 

class CPDIMorfoB : public CPDIBase
{  

  public:
         int QualElem;
         int Iteracoes;
         BOOL Condicional; 
         CPDIMorfoB* Mascara; 
 		 CPDIMorfoB *RgbtoXyz;
		 CPDIMorfoB *onRGBtoYCrCb;
		 CPDIMorfoB *onYCrCbtoRGB;	
		 CPDIMorfoB *OnRGBtoHSI;
		 CPDIMorfoB *OnHSItoRGB;
		 CPDIMorfoB *OnHSIExtrairH;
		 CPDIMorfoB *OnHSIExtrairS;
         CPDIMorfoB();
         ~CPDIMorfoB();
         int   Erosao(CDC *pDC);
         int   Dilatacao(CDC *pDC);
         int   Abertura(CDC *pDC);
         int   Fechamento(CDC *pDC);
         int   BordasErosao(CDC *pDC);
         int   BordasDilatacao(CDC *pDC);
         int   BordasDilEro(CDC *pDC);
         int   Esqueletizacao(CDC *pDC);  
         int   UltimaErosao(CDC* pDC);
         int   Fecha(CDC* pDC);
         BOOL  TemInfo(int *lini);
         int   CalculaPC(int *x, int *y);
		 int   LogicOper (HDC hdcDestino, HDC hdcOrigem, int x, int y, int Dx, int Dy, DWORD Raster);
			
		BOOL	Edge(int x,int y);
		BOOL	V	(int x,int y);
};

#endif


