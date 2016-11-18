// pdimadlg.cpp : implementation file
//

#include "stdafx.h" 
#ifndef __PDIMAGEM_H__
   #include "pdimagem.h"
#endif
#include "pdibase.h"
#include "pdidlgsg.h"

extern HGLOBAL Hvh;
extern HGLOBAL Hvs;
extern HGLOBAL Hvi;
extern float *vh;
extern float *vi;
extern float *vs;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPdiDlgSeg dialog

CPdiDlgSeg::CPdiDlgSeg(CWnd* pParent /*=NULL*/)
    : CDialog(CPdiDlgSeg::IDD, pParent)
{
    m_pView = NULL;
    //{{AFX_DATA_INIT(CPdiDlgSeg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

                                        
CPdiDlgSeg::CPdiDlgSeg(CPdimagemView* pView, CPdiBase* pImg, CPdimagemDoc* pImgd, CString* Titulo) 
    : CDialog(IDD_DIALOG1)
{  
   m_Titulo = *Titulo;
   m_pView = pView;
   m_pImg = pImg;
   m_pImgd = pImgd;
   CWnd::SetWindowText(m_Titulo);
}   
   

BOOL CPdiDlgSeg::OnInitDialog()
{
   int style;
   CButton* pButton = (CButton*) GetDlgItem(IDC_VISUAL);
   style = pButton->GetButtonStyle();
   pButton->SetButtonStyle(style,0);

   return CDialog::OnInitDialog();
}
   
BOOL CPdiDlgSeg::Create()                
{  
   BOOL ret; 
   ret = CDialog::Create(IDD_DIALOG7);
   CheckRadioButton(IDC_X1,IDC_X3,IDC_X1);
   CWnd::SetWindowText(m_Titulo);
   return ret;
}
      
void CPdiDlgSeg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPdiDlgSeg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

void CPdiDlgSeg::OnOK()
{  


    CDialog::OnOK();

    GlobalUnlock(Hvh);
    GlobalFree(Hvh);
    GlobalUnlock(Hvs);
    GlobalFree(Hvs);
    GlobalUnlock(Hvi);
    GlobalFree(Hvi);
                   
    CString TitleWindow;
    TitleWindow = "Segmentacao"; 
    m_pImgd->SetTitle(TitleWindow);
    CFrameWnd* pFrameL;
    pFrameL = m_pView->CreateChild(m_pImgd,m_pImgd);
    if (m_pView != NULL) {
       m_pView->SendMessage(ID_GOODBYE, IDOK);
    }   
}
 
void CPdiDlgSeg::OnCancel()
{
        
    CDialog::OnCancel();
    GlobalUnlock(Hvh);
    GlobalFree(Hvh);
    GlobalUnlock(Hvs);
    GlobalFree(Hvs);
    GlobalUnlock(Hvi);
    GlobalFree(Hvi);
    if (m_pView != NULL) {
        m_pView->SendMessage(ID_GOODBYE, IDCANCEL);
    }   
}
 
 
BEGIN_MESSAGE_MAP(CPdiDlgSeg, CDialog)
    
    ON_BN_CLICKED(IDC_VISUAL, OnClickedVisualizar)
    ON_WM_DRAWITEM()
                   
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPdiDlgSeg message handlers

void CPdiDlgSeg::OnClickedVisualizar()
{                                      


      HCURSOR OldCursor; 
      OldCursor = SetCursor(LoadCursor(NULL,IDC_WAIT)); 
      CPoint ptLeft, ptRight;
      DWORD dwColor;
      int tamanho;      
      char v3[3];
      CString Aux;
      
      tamanho = m_pImg->GetHeight() - 1;
      
      ptLeft = m_pImg->rect.TopLeft(); 
      ptRight = m_pImg->rect.BottomRight(); 
         
//      ptLeft.y = tamanho - ptLeft.y;
//      ptRight.y = tamanho - ptRight.y;
      BYTE * lp = m_pImg->lpBits;
      lp = lp + (ptRight.y * m_pImg->bmWidthBytes);
                                                 
      float X1, X2, X3, AUX1, AUX2, MaiorX1, MenorX1, MaiorX2, MaiorX3, MenorX2, MenorX3;                  
      int lin, col, i;
      long int ind;                                           
      float MaiorS, MaiorI, MaiorH, MenorS, MenorI, MenorH;
      MaiorS = MaiorI = MaiorH = 0;
      MenorS = MenorI = MenorH = 99999.9999;                                           
      for (lin = ptLeft.y; lin < ptRight.y; lin++, lp += m_pImg->bmWidthBytes)  // uma linha
           for (col = ptLeft.x; col < ptRight.x; col++) {

                dwColor = m_pImg->GetPixel(col,lin); 
                m_pImg->peValue.peRed = GetRValue(dwColor);
                m_pImg->peValue.peGreen = GetGValue(dwColor);
                m_pImg->peValue.peBlue = GetBValue(dwColor);
                m_pImg->ConverteHSI();
                
                if (m_pImg->hue > MaiorH) MaiorH = m_pImg->hue;
                if (m_pImg->saturation > MaiorS) MaiorS = m_pImg->saturation;
                if (m_pImg->intensity > MaiorI) MaiorI = m_pImg->intensity;

                if (m_pImg->hue < MenorH) MenorH = m_pImg->hue;
                if (m_pImg->saturation < MenorS) MenorS = m_pImg->saturation;
                if (m_pImg->intensity < MenorI) MenorI = m_pImg->intensity;   
           }     
                       
          _itoa((int) (MaiorH*255), v3, 10);
          Aux = v3;
          SetDlgItemText(IDC_HMA, Aux);

          _itoa((int) (MenorH*255), v3, 10);
          Aux = v3;
          SetDlgItemText(IDC_HME, Aux);

          _itoa((int) (MaiorS*255), v3, 10);
          Aux = v3;
          SetDlgItemText(IDC_SMA, Aux);

          _itoa((int) (MenorS*255), v3, 10);
          Aux = v3;
          SetDlgItemText(IDC_SME, Aux);

          _itoa((int) (MaiorI*255), v3, 10);
          Aux = v3;
          SetDlgItemText(IDC_IMA, Aux);
          
          _itoa((int) (MenorI*255), v3, 10);
          Aux = v3;
          SetDlgItemText(IDC_IME, Aux);
                 
                       
          AUX1 = m_pImgd->Imagem.v[1][1] * MaiorH +
                 m_pImgd->Imagem.v[1][2] * MaiorS +
                 m_pImgd->Imagem.v[1][3] * MaiorI;
          AUX2 = m_pImgd->Imagem.v[1][1] * MenorH +
                 m_pImgd->Imagem.v[1][2] * MenorS +
                 m_pImgd->Imagem.v[1][3] * MenorI;

          if (AUX1 < AUX2) { 
                         MaiorX1 = AUX2;
                         MenorX1 = AUX1;
                           }
           else         {       
                         MaiorX1 = AUX1;
                         MenorX1 = AUX2;
                        }                       

          AUX1 = m_pImgd->Imagem.v[2][1] * MaiorH +
                 m_pImgd->Imagem.v[2][2] * MaiorS +
                 m_pImgd->Imagem.v[2][3] * MaiorI;
          AUX2 = m_pImgd->Imagem.v[2][1] * MenorH +
                 m_pImgd->Imagem.v[2][2] * MenorS +
                 m_pImgd->Imagem.v[2][3] * MenorI;

          if (AUX1 < AUX2) { 
                         MaiorX2 = AUX2;
                         MenorX2 = AUX1;
                           }
           else         {       
                         MaiorX2 = AUX1;
                         MenorX2 = AUX2;
                        }                       

          AUX1 = m_pImgd->Imagem.v[3][1] * MaiorH +
                 m_pImgd->Imagem.v[3][2] * MaiorS +
                 m_pImgd->Imagem.v[3][3] * MaiorI;
          AUX2 = m_pImgd->Imagem.v[3][1] * MenorH +
                 m_pImgd->Imagem.v[3][2] * MenorS +
                 m_pImgd->Imagem.v[3][3] * MenorI;

          if (AUX1 < AUX2) { 
                         MaiorX3 = AUX2;
                         MenorX3 = AUX1;
                           }
           else         {       
                         MaiorX3 = AUX1;
                         MenorX3 = AUX2;
                        }                       

   int tamx = m_pImgd->Imagem.GetWidth();
   int tamy = m_pImgd->Imagem.GetHeight();  
   int x,y; 
   x=0;
   y=0;                              
   float Mex, Max;
   Mex = 9999.9999;
   Max = 0;

// DEFINE OPCAO

   int opcao, botaoH, botaoS, botaoI;
   opcao = 9;
   opcao = 0; 
   botaoH = IsDlgButtonChecked(IDC_X1);
   botaoS = IsDlgButtonChecked(IDC_X2);
   botaoI = IsDlgButtonChecked(IDC_X3);
   
   if (botaoH == 1)
        opcao = 1;
   if (botaoS == 1)
        opcao = 2;
   if (botaoI == 1)
        opcao = 3;
   if (botaoH == 1 && botaoS == 1) opcao = 4;     
   if (botaoH == 1 && botaoI == 1) opcao = 5;     
   if (botaoS == 1 && botaoI == 1) opcao = 6;     
   if (botaoH == 1 && botaoS == 1 && botaoI == 1) opcao = 7;     


   for (ind = 0; ind < ((float)tamy*tamx); ind++) {

                X1 = m_pImgd->Imagem.v[1][1] * vh[ind] +
                     m_pImgd->Imagem.v[1][2] * vs[ind] +
                     m_pImgd->Imagem.v[1][3] * vi[ind];
                X2 = m_pImgd->Imagem.v[2][1] * vh[ind] +
                     m_pImgd->Imagem.v[2][2] * vs[ind] +
                     m_pImgd->Imagem.v[2][3] * vi[ind];
                X3 = m_pImgd->Imagem.v[3][1] * vh[ind] +
                     m_pImgd->Imagem.v[3][2] * vs[ind] +
                     m_pImgd->Imagem.v[3][3] * vi[ind];

                if (x >= tamx) {
                    x = 0;
                    y++;        
                }    
                     

        switch (opcao) {
        
            case 1: if (X1>= MenorX1 && X1 <= MaiorX1)
                        m_pImgd->Imagem.SetPixel(x,y,0);
                    else         
                        m_pImgd->Imagem.SetPixel(x,y,255);                 
                    break;
            case 2: if (X2>= MenorX2 && X2 <= MaiorX2)
                        m_pImgd->Imagem.SetPixel(x,y,0);
                    else         
                        m_pImgd->Imagem.SetPixel(x,y,255);                 
                    break;
            case 3: if (X3>= MenorX3 && X3 <= MaiorX3)
                        m_pImgd->Imagem.SetPixel(x,y,0);
                    else         
                        m_pImgd->Imagem.SetPixel(x,y,255);                 
                    break;
            case 4: if ((X1>= MenorX1 && X1 <= MaiorX1) &&
                       (X2>= MenorX2 && X2 <= MaiorX2)) 
                        m_pImgd->Imagem.SetPixel(x,y,0);
                    else         
                        m_pImgd->Imagem.SetPixel(x,y,255);                 
                    break;
            case 5: if ((X1>= MenorX1 && X1 <= MaiorX1) &&
                       (X3>= MenorX3 && X3 <= MaiorX3)) 
                        m_pImgd->Imagem.SetPixel(x,y,0);
                    else         
                        m_pImgd->Imagem.SetPixel(x,y,255);                 
                    break;
            case 6: if ((X2>= MenorX2 && X2 <= MaiorX2) &&
                       (X3>= MenorX3 && X3 <= MaiorX3)) 
                        m_pImgd->Imagem.SetPixel(x,y,0);
                    else         
                        m_pImgd->Imagem.SetPixel(x,y,255);                 
                    break;
           default: if ((X1>= MenorX1 && X1 <= MaiorX1) &&
                        (X2>= MenorX2 && X2 <= MaiorX2) && 
                        (X3>= MenorX3 && X3 <= MaiorX3)) 
                        m_pImgd->Imagem.SetPixel(x,y,0);
                    else         
                        m_pImgd->Imagem.SetPixel(x,y,255);                 
          }
            x++;

   }

      int style;
      CButton* pButton = (CButton*) GetDlgItem(IDC_VISUAL);
      style = pButton->GetButtonStyle();
      pButton->SetButtonStyle(style,1);
      GetDlgItem(IDC_AREAVISUAL)->Invalidate();

      SetCursor(OldCursor);
}


void CPdiDlgSeg::OnDrawItem(int ident, LPDRAWITEMSTRUCT lpDIS)
{

      // draw the whole button
      CDC* pDC = CDC::FromHandle(lpDIS->hDC);
      CRect rc;
      rc.CopyRect(&lpDIS->rcItem);

      CPalette* pOldPal = NULL;
       
      if (m_pImgd->Imagem.IsPaletted())  {
         pOldPal = pDC->SelectPalette(m_pImgd->Imagem.pal, TRUE);
         pDC->RealizePalette();         
      }   
 
       
       
      int cxDIB = (int) m_pImgd->Imagem.GetWidth();         // Size of DIB - x
      int cyDIB = (int) m_pImgd->Imagem.GetHeight();        // Size of DIB - y
      CRect rcDIB;
      rcDIB.top = rcDIB.left = 0;
      rcDIB.right = cxDIB;
      rcDIB.bottom = cyDIB;
      CRect rcDest;
 
      rcDest = rc;
      m_pImgd->Imagem.PaintDIB(pDC->m_hDC, &rcDest, &rcDIB);

      if (pOldPal != NULL)
         pDC->SelectPalette(pOldPal, FALSE);


} 



