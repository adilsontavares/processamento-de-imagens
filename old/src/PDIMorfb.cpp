#include "stdafx.h"
#include <afxtempl.h>
#include <memory.h>
#include "pdimagem.h"
#include "pdibase.h"
#include "pdimorfb.h"

extern CPtrList	ListaEstudo;
extern POSITION	pos;

#define COPIA           0x00CC0020L
#define E               0x00EE0086L
#define OU              0x008800C6L
#define SUBTRAI         0x00990066L

typedef struct
        {short  P[5][5];
         short  TamLin, TamCol; } ELEMENTO;
       
         ELEMENTO EE[] =     {   {{0,1,0,0,0,
                                   1,2,1,0,0,
                                   0,1,0,0,0,       // Cruz
                                   0,0,0,0,0,
                                   0,0,0,0,0},3,3},

                                 {{1,2,1,0,0,
                                   0,0,0,0,0,
                                   0,0,0,0,0,       // Linha Horizontal
                                   0,0,0,0,0,
                                   0,0,0,0,0},1,3},

                                 {{1,0,0,0,0,
                                   2,0,0,0,0,
                                   1,0,0,0,0,       // Linha Vertical
                                   0,0,0,0,0,
                                   0,0,0,0,0},3,1},

                                 {{1,1,1,0,0,
                                   1,2,1,0,0,
                                   1,1,1,0,0,       // Quadrado
                                   0,0,0,0,0,
                                   0,0,0,0,0},3,3},

                                 {{0,1,1,1,0,
                                   1,1,1,1,1,
                                   1,1,2,1,1,       // Rhombus
                                   1,1,1,1,1,
                                   0,1,1,1,0},5,5}};


CPDIMorfoB::CPDIMorfoB()
{
}

CPDIMorfoB::~CPDIMorfoB()
{ 

}
                   




int
CPDIMorfoB::Erosao(CDC *pDC)
{

  int             i, j, PCx, PCy, Dx, Dy, Contador;

  int x = (int) GetWidth();
  int y = (int) GetHeight();


  CalculaPC(&PCx, &PCy);


  HBITMAP HImaErosao, HImaAux, HImaOrig, HImaUni;

  CDC DcErosao, DcAux, DcOrig, DcUni;
  DcErosao.CreateCompatibleDC(NULL);
  DcAux.CreateCompatibleDC(NULL);

  HImaAux = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if((::SelectObject(DcAux.m_hDC, HImaAux)) == NULL) return 0;

  if (Condicional && (Mascara != NULL)) {
      DcOrig.CreateCompatibleDC(NULL);
      DcUni.CreateCompatibleDC(NULL);
      HImaOrig = CreateDIBitmap(pDC->m_hDC,Mascara->lpBI,CBM_INIT,
                                Mascara->lpBits,Mascara->lpbmi,DIB_COR);
      HImaUni = CreateDIBitmap(pDC->m_hDC,Mascara->lpBI,CBM_INIT,
                                Mascara->lpBits,Mascara->lpbmi,DIB_COR);
      if((::SelectObject(DcOrig.m_hDC, HImaOrig)) == NULL) return 0;
      if((::SelectObject(DcUni.m_hDC, HImaUni)) == NULL) return 0;
      LogicOper           (DcUni.m_hDC, DcOrig.m_hDC, x, y, 0,  0,  DSTINVERT);
      LogicOper           (DcAux.m_hDC, DcUni.m_hDC, x, y, 0,  0,  OU);
  }

  HImaErosao = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if ((::SelectObject(DcErosao.m_hDC, HImaErosao)) == NULL) return 0;

  for                   (Contador=1;  Contador<=Iteracoes; Contador++)
  { for           (i=0; i<EE[QualElem].TamLin; i++)
    { for         (j=0; j<EE[QualElem].TamCol; j++)
      { if        (EE[QualElem].P[i][j]!=0)
        { Dx            = i - PCx;
          Dy            = j - PCy; 
          LogicOper     (DcErosao.m_hDC, DcAux.m_hDC, x, y, Dx, Dy, E);
        }
      }
    }
    LogicOper           (DcAux.m_hDC, DcErosao.m_hDC, x, y, 0,  0,  COPIA);
  }

  if (Condicional && (Mascara != NULL)) {
         LogicOper           (DcAux.m_hDC, DcOrig.m_hDC, x, y, 0,  0,  E);
         ReleaseDC(NULL,DcOrig.m_hDC);
         ReleaseDC(NULL,DcUni.m_hDC);
         DcOrig.DeleteDC();
         DcUni.DeleteDC();
  }
  
  i = GetDIBits(DcAux.m_hDC,HImaAux,0,(WORD) GetHeight(),lpBits,lpbmi,DIB_COR);

  ReleaseDC(NULL,DcAux.m_hDC);
  ReleaseDC(NULL,DcErosao.m_hDC);
  DcAux.DeleteDC();
  DcErosao.DeleteDC();
  if (HImaAux != NULL)  DeleteObject(HImaAux);
  if (HImaErosao != NULL)  DeleteObject(HImaErosao);
  if (HImaOrig != NULL)  DeleteObject(HImaOrig);
  if (HImaUni != NULL)  DeleteObject(HImaUni);
  return 1;
}


int
CPDIMorfoB::Dilatacao(CDC* pDC)
{

  int             i, j, PCx, PCy, Dx, Dy, Contador;

  int x = (int) GetWidth();
  int y = (int) GetHeight();



  CalculaPC(&PCx, &PCy);


  HBITMAP HImaDil, HImaAux, HImaOrig;

  CDC DcDil, DcAux, DcOrig;
  DcDil.CreateCompatibleDC(NULL);
  DcAux.CreateCompatibleDC(NULL);

  if (Condicional && (Mascara != NULL)) {
      DcOrig.CreateCompatibleDC(NULL);
      HImaOrig = CreateDIBitmap(pDC->m_hDC,Mascara->lpBI,CBM_INIT,Mascara->lpBits,
                                Mascara->lpbmi,DIB_COR);
      if((::SelectObject(DcOrig.m_hDC, HImaOrig)) == NULL) return 0;
  }
  
  HImaAux = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if((::SelectObject(DcAux.m_hDC, HImaAux)) == NULL) return 0;

  HImaDil = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if ((::SelectObject(DcDil.m_hDC, HImaDil)) == NULL) return 0;

  for                   (Contador=1;  Contador<=Iteracoes; Contador++)
  { for           (i=0; i<EE[QualElem].TamLin; i++)
    { for         (j=0; j<EE[QualElem].TamCol; j++)
      { if        (EE[QualElem].P[i][j]!=0)
        { Dx            = i - PCx;
          Dy            = j - PCy; 
          LogicOper     (DcDil.m_hDC, DcAux.m_hDC, x, y, Dx, Dy, OU);
        }
      }
    }
    LogicOper           (DcAux.m_hDC, DcDil.m_hDC, x, y, 0,  0,  COPIA);
    if (Condicional && (Mascara != NULL)) 
        LogicOper           (DcAux.m_hDC, DcOrig.m_hDC, x, y, 0,  0,  E);
  }

  if (Condicional && (Mascara != NULL)) {
      ReleaseDC(NULL,DcOrig.m_hDC);
      DcOrig.DeleteDC();
  }    
  
  i = GetDIBits(DcAux.m_hDC,HImaAux,0,(WORD) GetHeight(),lpBits,lpbmi,DIB_COR);

  ReleaseDC(NULL,DcAux.m_hDC);
  ReleaseDC(NULL,DcDil.m_hDC);
  DcAux.DeleteDC();
  DcDil.DeleteDC();
  if (HImaAux != NULL)  DeleteObject(HImaAux);
  if (HImaDil != NULL)  DeleteObject(HImaDil);
  if (HImaOrig != NULL)  DeleteObject(HImaOrig);
  return 1;
}



int
CPDIMorfoB::Abertura(CDC *pDC)
{

  Erosao(pDC) ;
  Dilatacao(pDC); 
  return 1;

}

int
CPDIMorfoB::Fechamento(CDC *pDC)
{
  Dilatacao(pDC);
  Erosao(pDC);
  return 1;
}


int
CPDIMorfoB::BordasErosao(CDC *pDC)
{
  int i;
  int x = (int) GetWidth();
  int y = (int) GetHeight();

  HBITMAP HImaErosao, HImaAux;

  CDC DcErosao, DcAux;
  DcAux.CreateCompatibleDC(NULL);
  DcErosao.CreateCompatibleDC(NULL);

  HImaAux = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if((::SelectObject(DcAux.m_hDC, HImaAux)) == NULL) return 0;

  

  HImaErosao = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if ((::SelectObject(DcErosao.m_hDC, HImaErosao)) == NULL) return 0;


  LogicOper     (DcErosao.m_hDC, DcAux.m_hDC, x, y, 0,  0, COPIA);
      
  LogicOper     (DcErosao.m_hDC, DcAux.m_hDC, x, y, -1,  0, E);
  LogicOper     (DcErosao.m_hDC, DcAux.m_hDC, x, y,  0, -1, E);
  LogicOper     (DcErosao.m_hDC, DcAux.m_hDC, x, y,  0,  0, E);
  LogicOper     (DcErosao.m_hDC, DcAux.m_hDC, x, y,  0,  1, E);
  LogicOper     (DcErosao.m_hDC, DcAux.m_hDC, x, y,  1,  0, E);

//  Subtracao --> Imagem Original - Imagem Erodida


  LogicOper     (DcErosao.m_hDC, DcAux.m_hDC, x, y,  0,  0, SUBTRAI);
  
  i = GetDIBits(DcErosao.m_hDC,HImaErosao,0,(WORD) GetHeight(),lpBits,lpbmi,DIB_COR);

  ReleaseDC(NULL,DcAux.m_hDC);
  ReleaseDC(NULL,DcErosao.m_hDC);
  DcAux.DeleteDC();
  DcErosao.DeleteDC();
  if (HImaAux != NULL)  DeleteObject(HImaAux);
  if (HImaErosao != NULL)  DeleteObject(HImaErosao);
  return 1;
}
 

int CPDIMorfoB::BordasDilatacao(CDC *pDC)
{        
  int i;
  int x = (int) GetWidth();
  int y = (int) GetHeight();

  HBITMAP HImaDil, HImaAux;

  CDC DcDil, DcAux;
  DcDil.CreateCompatibleDC(NULL);
  DcAux.CreateCompatibleDC(NULL);

  HImaAux = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if((::SelectObject(DcAux.m_hDC, HImaAux)) == NULL) return 0;

  HImaDil = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if ((::SelectObject(DcDil.m_hDC, HImaDil)) == NULL) return 0;


//  Dilatacao Elmento Cruz;

  LogicOper     (DcDil.m_hDC, DcAux.m_hDC, x, y, 0,  0, COPIA);
      
  LogicOper     (DcDil.m_hDC, DcAux.m_hDC, x, y, -1,  0, OU);
  LogicOper     (DcDil.m_hDC, DcAux.m_hDC, x, y,  0, -1, OU);
  LogicOper     (DcDil.m_hDC, DcAux.m_hDC, x, y,  0,  0, OU);
  LogicOper     (DcDil.m_hDC, DcAux.m_hDC, x, y,  0,  1, OU);
  LogicOper     (DcDil.m_hDC, DcAux.m_hDC, x, y,  1,  0, OU);


//  Subtracao --> Imagem Original - Imagem Erodida
  LogicOper     (DcDil.m_hDC, DcAux.m_hDC, x, y,  0,  0, SUBTRAI);

  
  i = GetDIBits(DcDil.m_hDC,HImaDil,0,(WORD) GetHeight(),lpBits,lpbmi,DIB_COR);

  ReleaseDC(NULL,DcAux.m_hDC);
  ReleaseDC(NULL,DcDil.m_hDC);
  DcAux.DeleteDC();
  DcDil.DeleteDC();
  if (HImaAux != NULL)  DeleteObject(HImaAux);
  if (HImaDil != NULL)  DeleteObject(HImaDil);  
  return i;
}

int
CPDIMorfoB::BordasDilEro(CDC *pDC)
{
  int i;
  int x = (int) GetWidth();
  int y = (int) GetHeight();

  HBITMAP HImaDil, HImaAux, HImaErosao;

  CDC DcDil, DcAux, DcErosao;
  DcDil.CreateCompatibleDC(NULL);
  DcErosao.CreateCompatibleDC(NULL);
  DcAux.CreateCompatibleDC(NULL);

  HImaAux = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if((::SelectObject(DcAux.m_hDC, HImaAux)) == NULL) return 0;

  HImaDil = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if ((::SelectObject(DcDil.m_hDC, HImaDil)) == NULL) return 0;

  HImaErosao = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if ((::SelectObject(DcErosao.m_hDC, HImaErosao)) == NULL) return 0;


 //  Dilatacao e Erosao - Elmento Cruz;

  LogicOper     (DcDil.m_hDC, DcAux.m_hDC, x, y, -1,  0, OU);
  LogicOper     (DcDil.m_hDC, DcAux.m_hDC, x, y,  0, -1, OU);
  LogicOper     (DcDil.m_hDC, DcAux.m_hDC, x, y,  0,  0, OU);
  LogicOper     (DcDil.m_hDC, DcAux.m_hDC, x, y,  0,  1, OU);
  LogicOper     (DcDil.m_hDC, DcAux.m_hDC, x, y,  1,  0, OU);

//  DilatacaoCruz(pDC, DcDil.m_hDC, DcDil.m_hDC);

  LogicOper     (DcErosao.m_hDC, DcAux.m_hDC, x, y, -1,  0, E);
  LogicOper     (DcErosao.m_hDC, DcAux.m_hDC, x, y,  0, -1, E);
  LogicOper     (DcErosao.m_hDC, DcAux.m_hDC, x, y,  0,  0, E);
  LogicOper     (DcErosao.m_hDC, DcAux.m_hDC, x, y,  0,  1, E);
  LogicOper     (DcErosao.m_hDC, DcAux.m_hDC, x, y,  1,  0, E);

//  ErosaoCruz(pDC, DcErosao.m_hDC, DcErosao.m_hDC);

//  Subtracao --> Imagem Original - Imagem Erodida
  LogicOper     (DcDil.m_hDC, DcErosao.m_hDC, x, y,  0,  0, SUBTRAI);

  
  i = GetDIBits(DcDil.m_hDC,HImaDil,0,(WORD) GetHeight(),lpBits,lpbmi,DIB_COR);

  ReleaseDC(NULL,DcAux.m_hDC);
  ReleaseDC(NULL,DcDil.m_hDC);
  ReleaseDC(NULL,DcErosao.m_hDC);
  DcAux.DeleteDC();
  DcDil.DeleteDC();
  DcErosao.DeleteDC();
  if (HImaAux != NULL)  DeleteObject(HImaAux);
  if (HImaDil != NULL)  DeleteObject(HImaDil);
  if (HImaErosao != NULL)  DeleteObject(HImaErosao);
  return 1;

}

int
CPDIMorfoB::Esqueletizacao(CDC* pDC)
{
  int x = (int) GetWidth();
  int y = (int) GetHeight();

  int lini;

  BOOL ret;
  int primeira; 

  HBITMAP HErosao, HErosaoAnt,HEsqAtu, HAbe;

  CDC DcAux, DcErosao, DcErosaoAnt, DcEsqAtu, DcAbe;
  
  DcErosao.CreateCompatibleDC(NULL);
  DcAbe.CreateCompatibleDC(NULL);
  DcErosaoAnt.CreateCompatibleDC(NULL);
  DcEsqAtu.CreateCompatibleDC(NULL);

  HEsqAtu = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if ((::SelectObject(DcEsqAtu.m_hDC, HEsqAtu)) == NULL) return 0;

  HErosao = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if ((::SelectObject(DcErosao.m_hDC, HErosao)) == NULL) return 0;

  HErosaoAnt = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if ((::SelectObject(DcErosaoAnt.m_hDC, HErosaoAnt)) == NULL) return 0;

  HAbe = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if ((::SelectObject(DcAbe.m_hDC, HAbe)) == NULL) return 0;


// Escolha do elemento
   switch (QualElem)                               


   {
    case 0:
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y, -1,  0, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  0, -1, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  0,  1, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  1,  0, E);

            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y, -1,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  0, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  1,  0, OU);

            break;
    case 1: 
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  0, -1, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  0,  1, E);

            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  0, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  1, OU);

            break;
    
    case 2:
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y, -1,  0, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  1,  0, E);

            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y, -1,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  1,  0, OU);

            break;
    case 3:
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y, -1,  0, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  0, -1, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  0,  1, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  1,  0, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y, -1, -1, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y, -1,  1, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  1, -1, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  1,  1, E);

            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y, -1,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  0, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  1,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y, -1, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y, -1,  1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  1, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  1,  1, OU);

            break;
    case 4:
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y, -1,  0, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  0, -1, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  0,  1, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  1,  0, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y, -1, -1, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y, -1,  1, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  1, -1, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  1,  1, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y, -1,  0, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  0, -1, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  0,  1, E);
            LogicOper     (DcErosaoAnt.m_hDC, DcEsqAtu.m_hDC, x, y,  1,  0, E);
//
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y, -1,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  0, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  1,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y, -1, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y, -1,  1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  1, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  1,  1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y, -1,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  0, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  1,  0, OU);

     }

//

  LogicOper     (DcAbe.m_hDC, DcEsqAtu.m_hDC, x, y,  0,  0, SUBTRAI);
  LogicOper     (DcEsqAtu.m_hDC, DcAbe.m_hDC, x, y,  0,  0, COPIA);

     
//   BYTE mostra;
   primeira = 1;
   lini = 0;
//   achou = _fmemchr(lpBits,0,y*bmWidthBytes);
   ret = TemInfo(&lini);
   while (ret) {

      switch (QualElem) 
      {
        case 0:
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y, -1,  0, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  0, -1, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  1, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  1,  0, E);

            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y, -1,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  0, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  0,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  0,  1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  1,  0, OU);

            break;
        case 1: 
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  0, -1, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  1, E);

            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  0, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  0,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  0,  1, OU);

            break;
        case 2:
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y, -1,  0, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  1,  0, E);

            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y, -1,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  0,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  1,  0, OU);

            break;
         case 3:
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y, -1,  0, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  0, -1, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  1, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  1,  0, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y, -1, -1, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y, -1,  1, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  1, -1, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  1,  1, E);

            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y, -1,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  0, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  0,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  0,  1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  1,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y, -1, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y, -1,  1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  1, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  1,  1, OU);

            break;
    
         case 4:
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y, -1,  0, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  0, -1, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  1, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  1,  0, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y, -1, -1, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y, -1,  1, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  1, -1, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  1,  1, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y, -1,  0, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  0, -1, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  1, E);
            LogicOper     (DcErosao.m_hDC, DcErosaoAnt.m_hDC, x, y,  1,  0, E);
//
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y, -1,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  0, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  0,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  0,  1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  1,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y, -1, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y, -1,  1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  1, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  1,  1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y, -1,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  0, -1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  0,  0, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  0,  1, OU);
            LogicOper     (DcAbe.m_hDC, DcErosao.m_hDC, x, y,  1,  0, OU);

     }

       LogicOper     (DcAbe.m_hDC, DcErosaoAnt.m_hDC, x, y,  0,  0, SUBTRAI);
       LogicOper     (DcEsqAtu.m_hDC, DcAbe.m_hDC, x, y,  0,  0, OU);
       LogicOper     (DcErosaoAnt.m_hDC, DcErosao.m_hDC, x, y,  0,  0, COPIA);

       GetDIBits(DcErosao.m_hDC,HErosao,0,(WORD) GetHeight(),lpBits,lpbmi,DIB_COR);
//       achou = _fmemchr(lpBits,0,y*bmWidthBytes);
       ret = TemInfo(&lini);  
}                             
  GetDIBits(DcEsqAtu.m_hDC,HEsqAtu,0,(WORD) GetHeight(),lpBits,lpbmi,DIB_COR);

  ReleaseDC(NULL,DcEsqAtu.m_hDC);
  ReleaseDC(NULL,DcErosao.m_hDC);
  ReleaseDC(NULL,DcErosaoAnt.m_hDC);
  ReleaseDC(NULL,DcAbe.m_hDC);
  DcEsqAtu.DeleteDC();
  DcAbe.DeleteDC();
  DcErosao.DeleteDC();
  DcErosaoAnt.DeleteDC();
  if (HEsqAtu != NULL)  DeleteObject(HEsqAtu);
  if (HAbe != NULL)  DeleteObject(HAbe);
  if (HErosao != NULL)  DeleteObject(HErosao); 
  if (HErosaoAnt != NULL)  DeleteObject(HErosaoAnt); 
  return 1;
}


int
CPDIMorfoB::UltimaErosao(CDC* pDC)
{
  int x = (int) GetWidth();
  int y = (int) GetHeight();

  int lini, lin, col;

  BOOL ret;
  int primeira; 

  HBITMAP HEroAnt, HErodida, HUlt;

  CDC DcEroAnt, DcErodida, DcUlt;
  
  DcErodida.CreateCompatibleDC(NULL);
  DcEroAnt.CreateCompatibleDC(NULL);
  DcUlt.CreateCompatibleDC(NULL);

  HErodida = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if ((::SelectObject(DcErodida.m_hDC, HErodida)) == NULL) return 0;

  HEroAnt = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if ((::SelectObject(DcEroAnt.m_hDC, HEroAnt)) == NULL) return 0;

  HUlt = CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
  if ((::SelectObject(DcUlt.m_hDC, HUlt)) == NULL) return 0;


   int n = 0;  
   primeira = 1;
   BYTE * lpT = lpBits;

   lini = 0;
   ret = TemInfo(&lini); 

   COLORREF cor_branca, cor_preto;
   cor_branca = RGB(255,255,255);
   cor_preto = RGB(0,0,0);

   CBrush* pBrush1 = new CBrush();
   CBrush* pBrushAnt1 = new CBrush();

   pBrush1->CreateSolidBrush(cor_branca);
   pBrushAnt1 = DcEroAnt.SelectObject(pBrush1);

   while (ret) {
       
       
       LogicOper     (DcEroAnt.m_hDC, DcErodida.m_hDC, x, y, 0,0, COPIA);

// erosao
   switch (QualElem) 
   {
    case 0:
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y, -1,  0, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  0, -1, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  0,  1, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  1,  0, E);
            break;
    case 1: 
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  0, -1, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  0,  1, E);
            break;
    
    case 2:
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y, -1,  0, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  1,  0, E);
            break;
    case 3:
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y, -1,  0, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  0, -1, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  0,  1, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  1,  0, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y, -1, -1, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y, -1,  1, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  1, -1, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  1,  1, E);
            break;
    case 4:
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y, -1,  0, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  0, -1, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  0,  1, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  1,  0, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y, -1, -1, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y, -1,  1, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  1, -1, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  1,  1, E);

            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y, -1,  0, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  0, -1, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  0,  0, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  0,  1, E);
            LogicOper     (DcErodida.m_hDC, DcEroAnt.m_hDC, x, y,  1,  0, E);
     }
     
       GetDIBits(DcErodida.m_hDC,HErodida,0,(WORD) GetHeight(),lpBits,lpbmi,DIB_COR);
       ret = TemInfo(&lini);  

// reconstrucao          // mudar ordem varredura
       lpT = lpBits;
       for (lin = 0; lin < y; lin++, lpT += bmWidthBytes)  {
           for (col = 0; col < x; col++)  {
              if (lpT[col] == 0) {
                 DcEroAnt.FloodFill(col,(y - lin),cor_branca);  
              }
           }
       }

       n++;
// resultados - uniao ultimas erosoes  
       if (primeira == 1)
          {
           LogicOper     (DcUlt.m_hDC, DcEroAnt.m_hDC, x, y,  0,  0, COPIA);
           primeira = 0;
       }
       else 
          LogicOper     (DcUlt.m_hDC, DcEroAnt.m_hDC, x, y,  0,  0, OU);

}                             
  GetDIBits(DcUlt.m_hDC,HUlt,0,(WORD) GetHeight(),lpBits,lpbmi,DIB_COR);

  DcEroAnt.SelectObject(pBrushAnt1);
/*  
  if (pBrush1) delete pBrush1;
  if (pBrush2) delete pBrush2;
  if (pBrushAnt1) delete pBrushAnt1;
  if (delete pBrushAnt2;
*/   
  ReleaseDC(NULL,DcErodida.m_hDC);
  ReleaseDC(NULL,DcEroAnt.m_hDC);
  ReleaseDC(NULL,DcUlt.m_hDC);
  DcUlt.DeleteDC();
  DcErodida.DeleteDC();
  DcEroAnt.DeleteDC();
  if (HUlt != NULL)  DeleteObject(HUlt);
  if (HErodida != NULL)  DeleteObject(HErodida); 
  if (HEroAnt != NULL)  DeleteObject(HEroAnt); 
  return 1;
}

BOOL
CPDIMorfoB::TemInfo(int *lini)
{
	int x,y,lin, col;                 
	x = GetWidth();
	y = GetHeight();
	BYTE * lpTemp = lpBits;
	lpTemp = lpTemp + (*lini) * bmWidthBytes;
	for (lin = *lini; lin < y; lin++, lpTemp += bmWidthBytes)
	{
		for (col = 0; col < x; col++)
		{
			if (lpTemp[col] == 0)
			{
				*lini = lin;
				return TRUE;      
			}     
		}
	}    
	return FALSE;
} 

int 
CPDIMorfoB::CalculaPC(int *x, int *y)
{
	for		(*x=0; *x<EE[QualElem].TamLin; (*x)++)
	{
		for	(*y=0; *y<EE[QualElem].TamCol; (*y)++)           
		{
			if (EE[QualElem].P[*x][*y]==2)
				return 1;
		}
	}

	return    0;
}

int CPDIMorfoB::LogicOper (HDC hdcDestino, HDC hdcOrigem, int x, int y, int Dx, int Dy, DWORD Raster)
{
	if	(Dx>=0 && Dy >=0)
	{
		BitBlt  (hdcDestino, Dx, Dy, x, y, hdcOrigem, 0,  0, Raster);
		return  1;
	}

	if	(Dx>=0 && Dy <0)
	{ 
		BitBlt  (hdcDestino, Dx, 0,  x, y, hdcOrigem, 0, abs(Dy), Raster);
		return  1;
	}

	if	(Dx<0 && Dy >=0)
	{ 
		BitBlt  (hdcDestino, 0,  Dy, x, y, hdcOrigem, abs(Dx), 0, Raster);
		return  1;
	}

	if	(Dx<0 && Dy <0)
	{ 
		BitBlt  (hdcDestino, 0,  0,  x, y, hdcOrigem, abs(Dx), abs(Dy), Raster);
		return  1;
	}

	return 0;
}

int
CPDIMorfoB::Fecha(CDC *pDC)
{

	int             i;

	int x = (int) GetWidth();
	int y = (int) GetHeight();


	HBITMAP HImaFecha, HAux;

	CDC DcFecha, DcAux;
	DcFecha.CreateCompatibleDC(NULL);
	DcAux.CreateCompatibleDC(NULL);

	HImaFecha	= CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
	if((::SelectObject(DcFecha	.m_hDC, HImaFecha	)) == NULL) 
		return 0;

	HAux		= CreateDIBitmap(pDC->m_hDC,lpBI,CBM_INIT,lpBits,lpbmi,DIB_COR);
	if((::SelectObject(DcAux	.m_hDC, HAux		)) == NULL)
		return 0;

	COLORREF cor_branca, cor_preta, cor_cinza; 

	cor_branca	= RGB(255,255,255);
	cor_cinza	= RGB(190,190,190);
	cor_preta	= RGB(0,0,0);

	CBrush* pBrushC = new CBrush();
	CBrush* pBrushAnt1;

	pBrushC->CreateSolidBrush(cor_preta);

	pBrushAnt1 = DcFecha.SelectObject(pBrushC);

	DcFecha.FloodFill(1,1,cor_preta);               

	BitBlt  (DcFecha.m_hDC, 0, 0, x, y, DcFecha.m_hDC, 0,  0, DSTINVERT);
	BitBlt  (DcFecha.m_hDC, 0, 0, x, y, DcAux.m_hDC, 0,  0, OU);

	i = GetDIBits(DcFecha.m_hDC,HImaFecha,0,(WORD) GetHeight(),lpBits,lpbmi,DIB_COR);

	DcFecha.SelectObject(pBrushAnt1);
	pBrushC->DeleteObject();
	ReleaseDC(NULL,DcFecha.m_hDC);
	ReleaseDC(NULL,DcAux.m_hDC);
	DcAux.DeleteDC();
	DcFecha.DeleteDC();
	
	if (HImaFecha != NULL)
		DeleteObject(HImaFecha);
	if (HAux != NULL)
		DeleteObject(HAux);

	return 1;
}

#define C(x,y)	x  ,y
#define N(x,y)	x  ,y-1
#define NE(x,y) x+1,y-1	
#define NO(x,y) x-1,y-1
#define S(x,y)	x  ,y+1
#define SE(x,y) x+1,y+1	
#define SO(x,y) x-1,y+1	
#define L(x,y)	x+1,y
#define O(x,y)	x-1,y

typedef struct tagPonto{
	int x,y;
}Ponto;

BOOL CPDIMorfoB::V(int x,int y)
{
	if(GetPixel(x,y)==0)
		return(TRUE);
	else
		return(FALSE);
}

BOOL CPDIMorfoB::Edge(int x,int y)
{
	// Verifica se os vizinhos pretos estao entre 2 e 6...

	int vizinhos=V(NO(x,y))+V(N (x,y))+V(NE(x,y))+V(O (x,y))+V(L (x,y))+V(SO(x,y))+V(S (x,y))+V(SE(x,y));

	if(vizinhos<2 || vizinhos>6)
		return(FALSE);

	// Verifica conectividade é igual 1...

	int conectividade=0;
	if(V(N (x,y))==FALSE && V(NE(x,y))==TRUE)
		conectividade++;
	if(V(NE(x,y))==FALSE && V(L (x,y))==TRUE)
		conectividade++;
	if(V(L (x,y))==FALSE && V(SE(x,y))==TRUE)
		conectividade++;
	if(V(SE(x,y))==FALSE && V(S (x,y))==TRUE)
		conectividade++;
	if(V(S (x,y))==FALSE && V(SO(x,y))==TRUE)
		conectividade++;
	if(V(SO(x,y))==FALSE && V(O (x,y))==TRUE)
		conectividade++;
	if(V(O (x,y))==FALSE && V(NO(x,y))==TRUE)
		conectividade++;
	if(V(NO(x,y))==FALSE && V(N (x,y))==TRUE)
		conectividade++;

	if(conectividade==1)
		return(TRUE);
	else
		return(FALSE);
}
