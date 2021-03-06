// DlgVersioni.cpp: file di implementazione
//

#include "stdafx.h"
#include "Note Series.h"
#include "DlgVersioni.h"
#include "afxdialogex.h"
extern CDbConn dbconfig;


// Finestra di dialogo DlgVersioni

IMPLEMENT_DYNAMIC(DlgVersioni, CDialogEx)

DlgVersioni::DlgVersioni(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VERSIONI, pParent)
{
	m_sql = _T("");
}

DlgVersioni::~DlgVersioni()
{
}

void DlgVersioni::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTA, m_lista);
}


BEGIN_MESSAGE_MAP(DlgVersioni, CDialogEx)
END_MESSAGE_MAP()


// Gestori di messaggi di DlgVersioni


BOOL DlgVersioni::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// Dimensioni caratteri
	CFont *m_pFont = new CFont();
	CString temp = _T("");
	m_pFont->CreatePointFont(100, _T("Verdana"));
	m_lista.SetFont(m_pFont, TRUE);
	// Disegno griglia
	m_lista.SetExtendedStyle(LVS_EX_GRIDLINES);

	// Colonne
	m_lista.InsertColumn(0, m_currlang.GetDesc(72,temp), LVCFMT_LEFT, 150);
	m_lista.InsertColumn(1, m_currlang.GetDesc(73, temp), LVCFMT_LEFT, 150);
	
	CListCtrl* pList = (CListCtrl *)GetDlgItem(IDC_LISTA); // Puntatore griglia
	m_sql.Format(_T("SELECT * FROM [VERSIONI]"));
	dbconfig.GetVersioni(m_sql, pList);

	return TRUE;
}
