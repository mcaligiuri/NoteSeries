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
	m_selNome = _T("");
	m_id = 0;
	m_rows = 0;
	m_pos = 0;
	pDoc = nullptr;
}

DlgGruppi::~DlgGruppi()
{
}

void DlgGruppi::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// Bottoni
	DDX_Control(pDX, IDC_BTNADD, m_btnAdd);
	DDX_Control(pDX, IDC_BTNDEL, m_btnDel);
	DDX_Control(pDX, IDC_BTNASSOC, m_btnAssoc);
	DDX_Control(pDX, IDC_BTNDISSOC, m_btnDis);
	// ComboBox, ListBox
	DDX_Control(pDX, IDC_CMBLABEL, m_cmbLabel);
	DDX_Control(pDX, IDC_LISTNOMI, m_lstNomi);
	DDX_Control(pDX, IDC_LISTASS, m_lstAss);
	// Caselle di testo
	DDX_Control(pDX, IDC_TXTLABEL, m_txtAdd);
	DDX_Control(pDX, IDC_TXTID, m_txtId);
	// Label
	DDX_Control(pDX, IDC_LBLSELETICHETTA, m_lblSel);
	DDX_Control(pDX, IDC_LBLNOMI, m_lblnomi);
	DDX_Control(pDX, IDC_LBLNOMIE, m_lblNomiE);
}


BEGIN_MESSAGE_MAP(DlgGruppi, CDialogEx)
	ON_BN_CLICKED(IDC_BTNADD, &DlgGruppi::SetEtichetta)
	ON_BN_CLICKED(IDC_BTNDEL, &DlgGruppi::DelEtichetta)
	ON_CBN_SELCHANGE(IDC_CMBLABEL, &DlgGruppi::OnCambioLabel)
	ON_BN_CLICKED(IDC_BTNASSOC, &DlgGruppi::OnSetAssoc)
	ON_BN_CLICKED(IDC_BTNDISSOC, &DlgGruppi::OnDelAssoc)
END_MESSAGE_MAP()


BOOL DlgGruppi::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	pDoc = GetDoc();
	pDoc->m_nodo = pDoc->m_artitoli.GetAt(1); // Ordine alfabetico
	pDoc->UpdateAllViews(NULL);
	// Imposto le colonne
	m_lstNomi.InsertColumn(0, m_currlang.GetDesc(37, m_temp), LVCFMT_CENTER, 150);
	m_lstAss.InsertColumn(0, m_currlang.GetDesc(37, m_temp), LVCFMT_CENTER, 150);

	// Popolo combobx delle etichette
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	m_pos = 0;
	for (int j = 0; j < pMain->m_i; j++)
		m_cmbLabel.AddString(pMain->m_lbl[j].nome);
	
	m_cmbLabel.SetCurSel(0);
	GetNomi();
	GetDescUpdate();
	this->SetBackgroundColor(RGB(255, 255, 255), 1);
	return TRUE;
}

// Descrizioni
void DlgGruppi::GetDescUpdate()
{
	m_btnAdd.SetWindowTextW(pDoc->m_label.GetAt(4));
	m_btnDel.SetWindowTextW(pDoc->m_label.GetAt(3));
	m_lblSel.SetWindowTextW(pDoc->m_label.GetAt(2));
	m_lblnomi.SetWindowTextW(pDoc->m_label.GetAt(5));
	m_lblNomiE.SetWindowTextW(pDoc->m_label.GetAt(6));
	m_btnAdd.SetMouseCursor(LoadCursor(NULL, IDC_HAND));
	m_btnDel.SetMouseCursor(LoadCursor(NULL, IDC_HAND));
	m_btnAssoc.SetMouseCursor(LoadCursor(NULL, IDC_HAND));
	m_btnDis.SetMouseCursor(LoadCursor(NULL, IDC_HAND));
	m_btnAssoc.SetTooltip(pDoc->m_label.GetAt(8));
	m_btnDis.SetTooltip(pDoc->m_label.GetAt(9));
}
// Richiamo i nomi non associati ad un'etichetta
void DlgGruppi::GetNomi()
{
	m_lstNomi.DeleteAllItems();
	for (int i = 0; i < pDoc->m_gridCount; i++)
	{
		if (pDoc->m_griglia[i].idl == 0)
		{
			m_rows = m_lstNomi.InsertItem(i, _T(""));
			m_lstNomi.SetItemText(m_rows, 0, pDoc->m_griglia[i].nome);
		}
	}
	OnCambioLabel();
}

// Aggiungo un'etichetta
void DlgGruppi::SetEtichetta()
{
	CString lbl, buf = _T("");	
	m_txtAdd.GetWindowTextW(lbl);
	if (lbl == _T(""))
		return;

	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;

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

	m_sql.Format(_T("INSERT INTO [ETICHETTE](NOME) VALUES('%s')"), lbl.GetString());
	
	if (!dbconfig.SetSerie(m_sql))
		return;
	
	pMain->GetGruppi();
	m_cmbLabel.ResetContent();
	for (int j = 0; j < pMain->m_i; j++)
		m_cmbLabel.AddString(pMain->m_lbl[j].nome);

	m_cmbLabel.SetCurSel(0);
}

// Elimino un'etichetta
void DlgGruppi::DelEtichetta()
{
	CString buf, msg,temp = _T("");
	m_cmbLabel.GetWindowTextW(buf);
	msg.Format(pDoc->m_label.GetAt(7) + L"?", buf.GetString());
	if (AfxMessageBox(msg, MB_YESNO | MB_ICONWARNING) == IDNO)
		return;
	
	m_sql.Format(_T("DELETE FROM [ETICHETTE] WHERE IDL=%d"), m_id);
	if (!dbconfig.SetSerie(m_sql))
		return;
	
	m_sql.Format(_T("UPDATE %s SET IDL=0 WHERE IDL=%d"), pDoc->m_tabella.GetString(), m_id);
	if (!dbconfig.SetSerie(m_sql))
		return;

	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pMain->GetGruppi();
	m_cmbLabel.ResetContent();
	for (int j = 0; j < pMain->m_i; j++)
		m_cmbLabel.AddString(pMain->m_lbl[j].nome);

	m_cmbLabel.SetCurSel(0);
}


void DlgGruppi::OnCambioLabel()
{
	short nsel = m_cmbLabel.GetCurSel();
	CString id, buf = _T("");
	if (nsel == LB_ERR)
		return;

	m_cmbLabel.GetLBText(nsel, buf);
	m_lstAss.DeleteAllItems();
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	for (int j = 0; j < pMain->m_i; j++)
	{
		if (pMain->m_lbl[j].nome == buf)
		{
			id.Format(_T("%d"), pMain->m_lbl[j].id);
			m_txtId.SetWindowTextW(id);
			m_id = _wtoi(id);
		}
	}
	for (int i = 0; i < pDoc->m_gridCount; i++)
	{
		if (pDoc->m_griglia[i].idl == m_id)
		{
			m_rows = m_lstAss.InsertItem(i, _T(""));
			m_lstAss.SetItemText(m_rows, 0, pDoc->m_griglia[i].nome);
		}
	}
}

// Associa nome selezionato ad un' etichetta
void DlgGruppi::OnSetAssoc()
{
	int item = 0;
	POSITION pos = m_lstNomi.GetFirstSelectedItemPosition();
	item = m_lstNomi.GetNextSelectedItem(pos);
	if (item == -1)
		return;
	m_selNome = m_lstNomi.GetItemText(item, 0);
	m_sql.Format(_T("UPDATE %s SET IDL=%d WHERE NOME='%s'"), pDoc->m_tabella.GetString(), m_id, m_selNome.GetString());
	if (!dbconfig.SetSerie(m_sql))
		return;
	for (int i = 0; i < pDoc->m_gridCount; i++)
	{
		if (pDoc->m_griglia[i].nome == m_selNome)
			pDoc->m_griglia[i].idl = m_id;
	}
	GetNomi();
}

// Dissocio un nome da un'etichetta
void DlgGruppi::OnDelAssoc()
{
	int item = 0;
	POSITION pos = m_lstAss.GetFirstSelectedItemPosition();
	item = m_lstAss.GetNextSelectedItem(pos);
	if (item == -1)
		return;
	m_selNome = m_lstAss.GetItemText(item, 0);
	m_sql.Format(_T("UPDATE %s SET IDL=0 WHERE NOME='%s'"), pDoc->m_tabella.GetString(), m_selNome.GetString());
	if (!dbconfig.SetSerie(m_sql))
		return;
	for (int i = 0; i < pDoc->m_gridCount; i++)
	{
		if (pDoc->m_griglia[i].nome == m_selNome)
			pDoc->m_griglia[i].idl = 0;
	}
	GetNomi();
}

// Puntatore al documento
CNoteSeriesDoc* DlgGruppi::GetDoc()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CNoteSeriesView* pView = (CNoteSeriesView*)pMain->GetActiveView();
	CNoteSeriesDoc* pDoc = pView->GetDocument();
	return pDoc;
}