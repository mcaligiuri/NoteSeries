// DlgCategorie.cpp: file di implementazione
//

#include "stdafx.h"
#include "Note Series.h"
#include "DlgCategorie.h"
#include "afxdialogex.h"
#include "Note SeriesDoc.h"
#include "Note SeriesView.h"
#include "mainfrm.h"


// Finestra di dialogo DlgCategorie

IMPLEMENT_DYNAMIC(DlgCategorie, CDialogEx)

DlgCategorie::DlgCategorie(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MODIFICA_CATEGORIE, pParent)
{
	m_temp = _T("");
	m_cat = _T("");
	m_idcat = _T("");
	m_sql = _T("");
	pDoc = nullptr;
}

DlgCategorie::~DlgCategorie()
{
}

void DlgCategorie::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRPCAT, m_grpCat);
	DDX_Control(pDX, IDC_TXTAGGIUNGI, m_txtAggiungi);
	DDX_Control(pDX, IDC_TXTRINOMINA, m_txtRinomina);
	DDX_Control(pDX, IDC_BTNAGGIUNGI, m_btnAggiungi);
	DDX_Control(pDX, IDC_BTNRINOMINA, m_btnRinomina);
	DDX_Control(pDX, IDC_BTNELIMINA, m_btnElimina);
	DDX_Control(pDX, IDC_LISTCAT, m_lstCat);
	DDX_Control(pDX, IDC_BTNCANCEL, m_btnAnnulla);
}


BEGIN_MESSAGE_MAP(DlgCategorie, CDialogEx)
	ON_BN_CLICKED(IDC_BTNAGGIUNGI, &DlgCategorie::OnClickAggiungi)
	ON_BN_CLICKED(IDC_BTNRINOMINA, &DlgCategorie::OnClickRinomina)
	ON_BN_CLICKED(IDC_BTNELIMINA, &DlgCategorie::OnClickElimina)
	ON_LBN_SELCHANGE(IDC_LISTCAT, &DlgCategorie::OnCambioSelezione)
	ON_BN_CLICKED(IDC_BTNCANCEL, &DlgCategorie::OnBnClickedBtncancel)
END_MESSAGE_MAP()


// Gestori di messaggi di DlgCategorie


BOOL DlgCategorie::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	pDoc = GetDoc();
	GetCateGorie(&m_lstCat);	// Popolo ListBox con le categorie
	GetDescUpdate();			// Aggiorno descrizioni in lingua dei controlli

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ECCEZIONE: le pagine delle proprietà OCX devono restituire FALSE
}

void DlgCategorie::GetCateGorie(CListBox* categorie)
{
	short i = 0;
	dbconfig.dbopen();
	try
	{
		CRecordset righe(dbconfig.serie);
		righe.Open(CRecordset::forwardOnly, L"SELECT * FROM [CATEGORIE]");
		while (!righe.IsEOF())
		{
			righe.GetFieldValue(L"IDCAT", m_idcat);
			righe.GetFieldValue(L"NOMECAT", m_cat);
			
			// Controllo apici singoli per SQL
			if (m_cat.Find(L"'") != -1)
				m_cat.Replace(L"'", L"''");

			categorie->AddString(m_idcat + L")" + m_cat);
			i++;
			righe.MoveNext();
		}
		righe.Close();
		dbconfig.serie->Close();
		
		// Se il numero di record è minore di 2 disattivo il pulsante elimina
		// Una categoria deve essere sempre presente!!!
		if (i < 2) m_btnElimina.EnableWindow(0);
		
		else m_btnElimina.EnableWindow(1);
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("Database error: ") + e->m_strError);
	}
}
void DlgCategorie::GetDescUpdate()
{
	m_btnAggiungi.SetWindowTextW(m_currlang.GetDesc(40, m_temp));		// Aggiungi
	m_btnRinomina.SetWindowTextW(m_currlang.GetDesc(85, m_temp));		// Rinomina
	m_btnElimina.SetWindowTextW(m_currlang.GetDesc(42, m_temp));		// Elimina
	//m_btnAnnulla.SetWindowTextW(m_currlang.GetDesc(14, m_temp));		// Annulla
	m_grpCat.SetWindowTextW(m_currlang.GetDesc(108, m_temp));			// Modifica le categorie (titolo groupbox)
}

// Aggiungo nuova categoria
void DlgCategorie::OnClickAggiungi()
{
	CString testo = _T("");
	m_txtAggiungi.GetWindowTextW(testo);
	if (testo == _T(""))
		return;
	
	// Controllo apici singoli per SQL
	if (testo.Find(L"'") != -1)
		testo.Replace(L"'", L"''");

	m_sql.Format(_T("INSERT INTO [CATEGORIE] (NOMECAT) VALUES ('%s')"),testo.GetString());
	if (!(dbconfig.SetSerie(m_sql)))
	{
		AfxMessageBox(_T("Error"));
		return;
	}
	CEdit* pId = (CEdit *)GetDlgItem(IDC_TXTIDCAT);
	pId->SetWindowTextW(_T(""));
	m_lstCat.ResetContent();
	m_txtRinomina.SetWindowTextW(_T(""));
	GetCateGorie(&m_lstCat);
}


// Rinomino categoria selezionata
void DlgCategorie::OnClickRinomina()
{
	CString testo, msg = _T("");
	m_txtRinomina.GetWindowTextW(testo);
	if (testo == _T("") || m_cat == testo)
		return;
	
	// Controllo apici singoli per SQL
	if (testo.Find(L"'") != -1)
		testo.Replace(L"'", L"''");

	msg.Format((m_currlang.GetDesc(110,m_temp)), m_cat, testo);	// Sicuro di voler rinominare la categoria?
	if (AfxMessageBox(msg, MB_YESNO) == IDNO)
		return;

	m_sql.Format(_T("UPDATE [CATEGORIE] SET NOMECAT='%s' WHERE IDCAT=%d"),testo.GetString(),_wtoi(m_idcat));
	if (!(dbconfig.SetSerie(m_sql)))
		return;

	CEdit* pId = (CEdit *)GetDlgItem(IDC_TXTIDCAT);
	pId->SetWindowTextW(_T(""));
	m_lstCat.ResetContent();
	m_txtRinomina.SetWindowTextW(_T(""));
	GetCateGorie(&m_lstCat);
}


// Elimino categoria selezionata
void DlgCategorie::OnClickElimina()
{
	CString testo,msg = _T("");
	m_txtRinomina.GetWindowTextW(testo);
	if (testo == _T(""))
		return;

	msg.Format((m_currlang.GetDesc(111,m_temp)), m_cat); // Sicuro di voler eliminare la categoria e ogni record associato nelle altre tabelle?
	if ((AfxMessageBox(msg, MB_YESNO | MB_ICONWARNING)) == IDNO)
		return;

	m_sql.Format(_T("DELETE FROM [CATEGORIE] WHERE IDCAT=%d"), _wtoi(m_idcat));
	if (!(dbconfig.SetSerie(m_sql)))
		return;

	m_sql.Format(_T("DELETE FROM [anime] WHERE IDCAT=%d"), _wtoi(m_idcat));
	if (!(dbconfig.SetSerie(m_sql)))
		return;

	m_sql.Format(_T("DELETE FROM [da vedere] WHERE IDCAT=%d"), _wtoi(m_idcat));
	if (!(dbconfig.SetSerie(m_sql)))
		return;

	CEdit* pId = (CEdit *)GetDlgItem(IDC_TXTIDCAT);
	pId->SetWindowTextW(_T(""));
	m_lstCat.ResetContent();
	m_txtRinomina.SetWindowTextW(_T(""));
	GetCateGorie(&m_lstCat);
}

void DlgCategorie::OnCambioSelezione()
{
	short nsel = m_lstCat.GetCurSel();
	if (nsel == LB_ERR)
		return;

	CString item,explode,name = _T("");				// Elemento nella listbox, elemento esploso, nome senza id
	CEdit *pid = (CEdit *)GetDlgItem(IDC_TXTIDCAT); // Puntatore casella di testo, "ID"
	int idc = 0;									// id categoria
	int pos = 0;									// posizione iniziale del cursore per la explode (Tokenize)
	
	// Acquisisco elemento selezionato e 
	// lo esplodo in 2 parti: Nome e ID,
	// usando il carattere ) come separatore
	m_lstCat.GetText(nsel, item);
	explode = item.Tokenize(_T(")"), pos);

	// Nome della categoria
	while (explode != _T(""))
	{
		name.Format(_T("%s"), explode.GetString());
		m_txtRinomina.SetWindowTextW(name);
		m_cat = name;
		explode = item.Tokenize(_T(")"), pos);
	}
	
	// Controllo apici singoli per SQL
	if (m_cat.Find(L"'") != -1)
		m_cat.Replace(L"'", L"''");
		
	// ID della categoria
	idc = _wtoi(item);
	m_idcat.Format(_T("%d"), idc);
	pid->SetWindowTextW(m_idcat);
}

// Pulsante annulla
void DlgCategorie::OnBnClickedBtncancel() { OnOK();}

CNoteSeriesDoc* DlgCategorie::GetDoc()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CNoteSeriesView* pView = (CNoteSeriesView*)pMain->GetActiveView();
	CNoteSeriesDoc* pDoc = pView->GetDocument();

	return pDoc;
}
