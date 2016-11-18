// DlgFastAdaptiveContrastParameters.cpp : implementation file
//

#include "stdafx.h"
#include "PDImagem.h"
#include "DlgFastAdaptiveContrastParameters.h"
#include "afxdialogex.h"


// DlgFastAdaptiveContrastParameters dialog

IMPLEMENT_DYNAMIC(DlgFastAdaptiveContrastParameters, CDialog)

DlgFastAdaptiveContrastParameters::DlgFastAdaptiveContrastParameters(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_FASTADAPTIVECONTRAST_LIMIARRUIDO, pParent)
{
	m_pLimiarRuido = new BYTE();
}

DlgFastAdaptiveContrastParameters::DlgFastAdaptiveContrastParameters(CView * pView, BYTE * pLimiarRuido)
	: CDialog(DlgFastAdaptiveContrastParameters::IDD)
{
	m_pView = pView;
	m_pLimiarRuido = pLimiarRuido;
}

DlgFastAdaptiveContrastParameters::~DlgFastAdaptiveContrastParameters()
{
}

BOOL DlgFastAdaptiveContrastParameters::OnInitDialog()
{
	*m_pLimiarRuido = 10;

	return CDialog::OnInitDialog();
}

void DlgFastAdaptiveContrastParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_LIMIARRUIDO, *m_pLimiarRuido);
}


BEGIN_MESSAGE_MAP(DlgFastAdaptiveContrastParameters, CDialog)
END_MESSAGE_MAP()


// DlgFastAdaptiveContrastParameters message handlers
