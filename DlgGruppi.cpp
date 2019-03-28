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
	m_sql = _T("");
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
	ON_BN_CLICKED(IDC_BTNADD, &DlgGruppi::SetEtichetta)
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

	// Popolo combobx delle etichette
	m_nomi.RemoveAll();
	dbconfig.GetGruppi(&m_nomi);
	for (int j = 0; j < m_nomi.GetCount(); j++)
		m_cmbLabel.AddString(m_nomi.GetAt(j));
	
	m_cmbLabel.SetCurSel(0);
	return TRUE;
}


// Aggiungo un'etichetta
void DlgGruppi::SetEtichetta()
{
	CString lbl, buf = _T("");	
	m_txtAdd.GetWindowTextW(lbl);

	// Caratteri speciali sostituiti con uno spazio
	char specialChar[10] = "\"\\/:*|<>";
	for (short i = 0; i < 10; i++)
	{
		buf.Format(_T("%c"), specialChar[i]);
		if (lbl.Find(buf) != -1 && lbl.Find(buf) != '\0')
			lbl.Replace(buf, _T(" "));
	}
	// Controllo spazi ad inizio e fine
	int start = lbl.Find(L" ");
	if (start == 0)
		lbl.Delete(start, 1);

	int end = lbl.Find(L" ");
	if (end == lbl.GetLength() - 1)
		lbl.Delete(end, lbl.GetLength() - 1);

	m_sql.Format(_T("INSERT INTO [ETICHETTE](NOME) VALUES('%s')"),lbl);
	
	if (!dbconfig.SetSerie(m_sql))
		return;
	
	m_nomi.Add(lbl);
	m_cmbLabel.AddString(m_nomi.GetAt(m_nomi.GetCount()-1));
}
