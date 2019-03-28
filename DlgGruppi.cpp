// DlgGruppi.cpp: file di implementazione
//

#include "stdafx.h"
#include "Note Series.h"
#include "DlgGruppi.h"
#include "Note SeriesView.h"
#include "MainFrm.h"
#include "afxdialogex.h"


// Finestra di dialogo DlgGruppi

IMPLEMENT_DYNAMIC(DlgGruppi, CDialogEx)

DlgGruppi::DlgGruppi(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GRUPPI, pParent)
{
	m_temp = _T("");
	m_rows = 0;

}

DlgGruppi::~DlgGruppi()
{
}

void DlgGruppi::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNADD, m_btnAdd);
	DDX_Control(pDX, IDC_BTNDEL, m_btnDel);
	DDX_Control(pDX, IDC_CMBLABEL, m_cmbLabel);
	DDX_Control(pDX, IDC_TXTLABEL, m_txtAdd);
	DDX_Control(pDX, IDC_BTNASSOC, m_btnAssoc);
	DDX_Control(pDX, IDC_BTNDISSOC, m_btnDis);
	DDX_Control(pDX, IDC_LISTNOMI, m_lstNomi);
	DDX_Control(pDX, IDC_LISTASS, m_lstAss);
}


BEGIN_MESSAGE_MAP(DlgGruppi, CDialogEx)
END_MESSAGE_MAP()


BOOL DlgGruppi::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();
	CNoteSeriesDoc* pDoc = pView->GetDocument();
	// Imposto le colonne
	m_lstNomi.InsertColumn(0, m_currlang.GetDesc(38, m_temp), LVCFMT_CENTER, 150);
	
	for (int i = 0; i < pDoc->m_gridCount; i++)
	{
		m_rows = m_lstNomi.InsertItem(i, _T(""));
		m_lstNomi.SetItemText(m_rows, 0, pDoc->m_griglia[i].nome);
	}


	m_lstAss.InsertColumn(0, m_currlang.GetDesc(38, m_temp), LVCFMT_CENTER, 150);
	return TRUE;
}
