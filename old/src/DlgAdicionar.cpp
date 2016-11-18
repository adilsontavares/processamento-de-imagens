// DlgAdicionar.cpp : implementation file
//

#include "stdafx.h"
#include "PDImagem.h"
#include "DlgAdicionar.h"
#include "afxdialogex.h"


// CDlgAdicionar dialog

IMPLEMENT_DYNAMIC(CDlgAdicionar, CDialog)

CDlgAdicionar::CDlgAdicionar(CView *pView, BYTE *pValor)
	: CDialog(CDlgAdicionar::IDD)
{
	m_pView = pView;
	m_pValor = pValor;
}

CDlgAdicionar::CDlgAdicionar(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ADICIONAR, pParent)
{
	m_pValor = new BYTE();
}

CDlgAdicionar::~CDlgAdicionar()
{
}

BOOL CDlgAdicionar::OnInitDialog()
{
	*m_pValor = 30;

	return CDialog::OnInitDialog();
}

void CDlgAdicionar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_VALOR, *m_pValor);
}


BEGIN_MESSAGE_MAP(CDlgAdicionar, CDialog)
END_MESSAGE_MAP()


// CDlgAdicionar message handlers