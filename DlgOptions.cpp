// DlgOptions.cpp: file di implementazione
//

#include "stdafx.h"
#include "Note Series.h"
#include "DlgOptions.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "Note SeriesDoc.h"
#include "Note SeriesView.h"

#include <iostream>
#include <fstream> 

// Finestra di dialogo DlgOptions

IMPLEMENT_DYNAMIC(DlgOptions, CDialogEx)

DlgOptions::DlgOptions(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPZIONI, pParent)
{
	retUser = _T("");
	len = 255;
	temp = _T("");
	lingua = FALSE;
	m_table = 0;
	dir = _T("");
}

DlgOptions::~DlgOptions()
{
}

void DlgOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UTENTE, m_utente);
	DDX_Control(pDX, IDC_TEL, m_tel);
	DDX_Control(pDX, IDC_SOCIETA, m_soc);
	DDX_Control(pDX, IDC_MAIL, m_mail);
	DDX_Control(pDX, IDC_ARCHIVI, m_archivi);
	DDX_Control(pDX, IDC_CARTELLA, m_cartella);
	DDX_Control(pDX, IDC_LINGUA, m_lingua);
	DDX_Control(pDX, IDC_LBLUSER, m_lblUser);
	DDX_Control(pDX, IDC_LBLSOC, m_lblSoc);
	DDX_Control(pDX, IDC_LBLCART, m_lblFolder);
	DDX_Control(pDX, IDC_LBLARC, m_lblDb);
	DDX_Control(pDX, IDC_LBLLINGUA, m_lblLingua);
	DDX_Control(pDX, IDC_OPTSALVA, m_btnSalva);
	DDX_Control(pDX, IDC_ANNULLA, m_btnAnnulla);
	DDX_Control(pDX, IDC_APRISERIE, m_btnSerie);
	DDX_Control(pDX, IDC_BTNDB, m_btnDb);
	DDX_Control(pDX, IDC_BTNEXP, m_btnExp);
	DDX_Control(pDX, IDC_LOGHI, m_loghi);
	DDX_Control(pDX, IDC_APRILOGHI, m_btnLoghi);
	DDX_Control(pDX, IDC_LBLLOGHI, m_lblLoghi);
	DDX_Control(pDX, IDC_EXPORT, m_export);
	DDX_Control(pDX, IDC_LBLEXP, m_lblExport);
	DDX_Control(pDX, IDC_GRPADV, m_grpadv);
	DDX_Control(pDX, IDC_CHECKADV, m_chkadv);
	DDX_Control(pDX, IDC_BTNBACK, m_btnback);
	DDX_Control(pDX, IDC_BTNDELOLD, m_btnold);
	DDX_Control(pDX, IDC_BTNDELDB, m_btndel);
	DDX_Control(pDX, IDC_CHKAUTO, m_chkauto);
	DDX_Control(pDX, IDC_OLDDB, m_lblold);
	DDX_Control(pDX, IDC_DELDB, m_lbldel);
	DDX_Control(pDX, IDC_ESPSQL, m_lblesp);
	DDX_Control(pDX, IDC_CHKALL, m_chkall);
	DDX_Control(pDX, IDC_CHKVISTE, m_chkviste);
	DDX_Control(pDX, IDC_CHKFUT, m_chkfut);
	DDX_Control(pDX, IDC_CHKCAT, m_chkcat);
	DDX_Control(pDX, IDC_BTNSQL, m_btnsql);
	DDX_Control(pDX, IDC_TITOLO, m_lbltitle);
}


BEGIN_MESSAGE_MAP(DlgOptions, CDialogEx)
	ON_BN_CLICKED(IDC_APRISERIE, &DlgOptions::OnBnClickedApriserie)
	ON_BN_CLICKED(IDC_OPTSALVA, &DlgOptions::OnBnClickedOptsalva)
	ON_BN_CLICKED(IDC_ANNULLA, &DlgOptions::OnBnClickedAnnulla)
	ON_BN_CLICKED(IDC_APRILOGHI, &DlgOptions::OnBnClickedApriloghi)
	ON_BN_CLICKED(IDC_BTNDB, &DlgOptions::OnBnClickedBtndb)
	ON_BN_CLICKED(IDC_BTNEXP, &DlgOptions::OnBnClickedBtnexp)
	ON_CBN_SELCHANGE(IDC_LINGUA, &DlgOptions::OnCbnSelchangeLingua)
	ON_BN_CLICKED(IDC_CHECKADV, &DlgOptions::OnMostraAvanzate)
	ON_BN_CLICKED(IDC_BTNBACK, &DlgOptions::EseguiBackup)
	ON_BN_CLICKED(IDC_BTNDELDB, &DlgOptions::DistruggiDatabase)
	ON_BN_CLICKED(IDC_BTNDELOLD, &DlgOptions::RipristinaBackup)
	ON_BN_CLICKED(IDC_CHKAUTO, &DlgOptions::CheckBackup)
	ON_BN_CLICKED(IDC_CHKALL, &DlgOptions::EsportaTutto)
	ON_BN_CLICKED(IDC_BTNSQL, &DlgOptions::EsportaInSQL)
	ON_BN_CLICKED(IDC_CHKVISTE, &DlgOptions::checkViste)
	ON_BN_CLICKED(IDC_CHKFUT, &DlgOptions::checkFuture)
	ON_BN_CLICKED(IDC_CHKCAT, &DlgOptions::checkCat)
END_MESSAGE_MAP()


// Gestori di messaggi di DlgOptions


BOOL DlgOptions::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Aggiungo riferimenti CEdit a CArray (NON serve a un cavolo, ma va beh)
	m_edit.Add(&m_utente); 
	m_edit.Add(&m_tel); 
	m_edit.Add(&m_soc);
	m_edit.Add(&m_mail);
	// Leggo le chiavi di registro nella sezione "Settings"
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings"))) {
		for (short i = 0; i < m_edit.GetSize(); i++) {
			chiave.QueryStringValue((txtKey[i]), retUser.GetBufferSetLength(len), &len);
			m_edit[i]->SetWindowTextW(retUser);
			len = 255;
		}
		retUser.ReleaseBuffer();
	}

	// Leggo le chiavi di registro nella sezione "Path"
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path"))) {
		m_edit.RemoveAll(); // Svuoto CArray
		m_edit.Add(&m_archivi);
		m_edit.Add(&m_cartella);
		m_edit.Add(&m_loghi);
		m_edit.Add(&m_export);
		for (short j = 0; j < m_edit.GetSize(); j++) {
			chiave.QueryStringValue((txtKeyPath[j]), retUser.GetBufferSetLength(len), &len);
			m_edit[j]->SetWindowTextW(retUser);
			len = 255;
		}
		retUser.ReleaseBuffer(); 
	}
	// Sfondo bianco
	this->SetBackgroundColor(RGB(255, 255, 255), 1);
	// Caratte del titolo
	CFont *m_SizeTitle = new CFont();
	m_SizeTitle->CreatePointFont(110, _T("Verdana"));
	m_lbltitle.SetFont(m_SizeTitle, TRUE);
	// Controllo se è attivo il backup automatico
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings")))
	{
		DWORD reg = 0;
		chiave.QueryDWORDValue(_T("Backup"), reg);
		if (reg != 1)
			m_chkauto.SetCheck(0);
		else
			m_chkauto.SetCheck(1);
	}
	OnMostraAvanzate();
	GetDescUpdate();
	return TRUE; 
}


// Apro cartella dove l'utente intende salvarsi eventuali altri dati (esempio ep scaricati da internet)
void DlgOptions::OnBnClickedApriserie()
{
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path"))) {
		len = 255;
		chiave.QueryStringValue((_T("Cartella")), retUser.GetBufferSetLength(len), &len);
		ShellExecute(NULL, L"open", retUser, NULL, NULL, SW_SHOWMAXIMIZED);
		retUser.ReleaseBuffer();
	}
}

// Apro cartella dove vengono copiati i loghi
void DlgOptions::OnBnClickedApriloghi()
{
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path"))) {
		len = 255;
		chiave.QueryStringValue((_T("Loghi")), retUser.GetBufferSetLength(len), &len);
		ShellExecute(NULL, L"open", retUser, NULL, NULL, SW_SHOWMAXIMIZED);
		retUser.ReleaseBuffer();
	}
}

// Apro cartella degli archivi
void DlgOptions::OnBnClickedBtndb()
{
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path"))) {
		len = 255;
		chiave.QueryStringValue((_T("SharedMDB")), retUser.GetBufferSetLength(len), &len);
		ShellExecute(NULL, L"open", retUser, NULL, NULL, SW_SHOWMAXIMIZED);
		retUser.ReleaseBuffer();
	}
}

// Apro cartella Export
void DlgOptions::OnBnClickedBtnexp()
{
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path"))) {
		len = 255;
		chiave.QueryStringValue((_T("Export")), retUser.GetBufferSetLength(len), &len);
		ShellExecute(NULL, L"open", retUser, NULL, NULL, SW_SHOWMAXIMIZED);
		retUser.ReleaseBuffer();
	}
}


// Descrizioni
void DlgOptions::GetDescUpdate()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();
	CNoteSeriesDoc* pDoc = pView->GetDocument();
	// Combo della lingua
	m_lingua.AddString(m_currlang.GetDesc(5,temp));   // italiano
	m_lingua.AddString(m_currlang.GetDesc(6, temp));  // inglese
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings")))
	{
		len = 255;
		chiave.QueryStringValue((_T("Lingua")), retUser.GetBufferSetLength(len), &len);
		retUser.ReleaseBuffer();
	}

	if(retUser == m_currlang.GetDesc(6, temp))
		m_lingua.SetCurSel(0); // ITALIANO
	else
		m_lingua.SetCurSel(1); // INGLESE
	
	// Label traduzioni
	m_lblUser.SetWindowTextW(pDoc->m_setting.GetAt(0));
	m_lblDb.SetWindowTextW(pDoc->m_setting.GetAt(1));
	m_lblFolder.SetWindowTextW(pDoc->m_setting.GetAt(2));
	m_lblSoc.SetWindowTextW(pDoc->m_setting.GetAt(3));
	m_lblLingua.SetWindowTextW(pDoc->m_setting.GetAt(4));
	m_lblLoghi.SetWindowTextW(pDoc->m_setting.GetAt(5));
	m_lblExport.SetWindowTextW(pDoc->m_setting.GetAt(6));
	
	// Bottoni traduzioni
	m_btnAnnulla.SetWindowTextW(pDoc->m_setting.GetAt(7));
	m_btnDb.SetWindowTextW(pDoc->m_setting.GetAt(8));
	m_btnSalva.SetWindowTextW(pDoc->m_setting.GetAt(9));
	m_btnSerie.SetWindowTextW(pDoc->m_setting.GetAt(10));
	m_btnExp.SetWindowTextW(pDoc->m_setting.GetAt(11));
	m_btnLoghi.SetWindowTextW(pDoc->m_setting.GetAt(12));
	m_btndel.SetWindowTextW(pDoc->m_setting.GetAt(21));

	// Avanzate traduzioni
	m_chkadv.SetWindowTextW(pDoc->m_setting.GetAt(13));
	m_grpadv.SetWindowTextW(pDoc->m_setting.GetAt(13));
	m_chkauto.SetWindowTextW(pDoc->m_setting.GetAt(14));
	m_lblold.SetWindowTextW(pDoc->m_setting.GetAt(15));
	m_lbldel.SetWindowTextW(pDoc->m_setting.GetAt(16));
	m_lblesp.SetWindowTextW(pDoc->m_setting.GetAt(17));
	m_chkall.SetWindowTextW(pDoc->m_setting.GetAt(18));
	m_btnback.SetWindowTextW(pDoc->m_setting.GetAt(19));
	m_btnsql.SetWindowTextW(pDoc->m_setting.GetAt(19));
	m_btnold.SetWindowTextW(pDoc->m_setting.GetAt(20));
	m_chkfut.SetWindowTextW(pDoc->m_setting.GetAt(22));
	m_chkviste.SetWindowTextW(pDoc->m_setting.GetAt(23));
	m_chkcat.SetWindowTextW(pDoc->m_setting.GetAt(24));
	m_lbltitle.SetWindowTextW(pDoc->m_setting.GetAt(25));
}

// Aggiorno le chiavi di registro con le modifiche inserite dall'utente nel dialog
void DlgOptions::OnBnClickedOptsalva()
{
	if ((AfxMessageBox(m_currlang.GetDesc(7, temp), MB_YESNO) != IDYES))
		return;

	CString newKey = _T("");

	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings")))
	{
		m_utente.GetWindowText(newKey);
		chiave.SetStringValue(_T("Utente"), (newKey));

		m_lingua.GetWindowText(newKey);
		chiave.SetStringValue(_T("Lingua"), (newKey));

		m_soc.GetWindowText(newKey);
		chiave.SetStringValue(_T("Società"), (newKey));

		m_tel.GetWindowText(newKey);
		chiave.SetStringValue(_T("Tel"), (newKey));

		m_mail.GetWindowText(newKey);
		chiave.SetStringValue(_T("Mail"), (newKey));
	}

	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
	{
		m_cartella.GetWindowText(newKey);
		chiave.SetStringValue(_T("Cartella"), (newKey));

		m_loghi.GetWindowText(newKey);
		chiave.SetStringValue(_T("Loghi"), (newKey));
		
		m_archivi.GetWindowText(newKey);
		chiave.SetStringValue(_T("SharedMDB"), (newKey));

		m_export.GetWindowText(newKey);
		chiave.SetStringValue(_T("Export"), (newKey));
	}

	// Attivo o disattivo il backup automatico
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings")))
	{
		if (m_chkauto.GetCheck())
			chiave.SetDWORDValue(L"Backup", 1);
		else
			chiave.SetDWORDValue(L"Backup", 0);
	}

	// Se non espando la combo della lingua
	// Chiudo il dialog normalmente
	// altrimenti riavvio il programma
	if (!lingua)
		CDialogEx::OnOK();
	else
	{
		AfxMessageBox(m_currlang.GetDesc(83, temp));
		PostQuitMessage(0);

		// Quando in release 
		ShellExecute(NULL, L"open", L"Note Series.exe", NULL, NULL, SW_RESTORE);

		// Quando in debug, ignorare assert
		// ShellExecute(NULL, L"open", L"Debug\\Note Series.exe", NULL, NULL, SW_RESTORE);
	}
}


void DlgOptions::OnBnClickedAnnulla()
{
	CDialogEx::OnCancel();
}

// Serve per capire se l'utente ha toccato la combo della lingua
// in tal caso è necessario riavviare il programma per 
// vedere l'effettivo cambio
void DlgOptions::OnCbnSelchangeLingua()
{
	lingua = TRUE;
}


void DlgOptions::OnMostraAvanzate()
{
	if (m_chkadv.GetCheck())
	{
		m_btndel.EnableWindow(1);
		
		// Senza il backup automatico, disabilito
		// il ripristino
		if (m_chkauto.GetCheck())
			m_btnold.EnableWindow(1);
		else
			m_btnold.EnableWindow(0);
		
		m_btnback.EnableWindow(1);
		m_chkauto.EnableWindow(1);
		m_lblold.EnableWindow(1);
		m_lbldel.EnableWindow(1);
		m_lblesp.EnableWindow(1);
		m_chkall.EnableWindow(1);
		m_chkviste.EnableWindow(1);
		m_chkfut.EnableWindow(1);
		m_chkcat.EnableWindow();
		m_btnsql.EnableWindow(1);
	}
	else
	{
		m_btndel.EnableWindow(0);
		m_btnold.EnableWindow(0);
		m_btnback.EnableWindow(0);
		m_chkauto.EnableWindow(0);
		m_lblold.EnableWindow(0);
		m_lbldel.EnableWindow(0);
		m_lblesp.EnableWindow(0);
		m_chkall.EnableWindow(0);
		m_chkviste.EnableWindow(0);
		m_chkfut.EnableWindow(0);
		m_chkcat.EnableWindow(0);
		m_btnsql.EnableWindow(0);
	}
}

// Esegui immediatamente una copia di backup
void DlgOptions::EseguiBackup()
{
	CString m_ret = _T("");
	CString data = CTime::GetCurrentTime().Format("%d-%m-%Y");
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
	{
		ULONG m_len = 255;
		chiave.QueryStringValue((_T("SharedMDB")), m_ret.GetBufferSetLength(m_len), &m_len);
		m_ret.ReleaseBuffer();
	}
	// Creo copia del database "serie.mdb"
	if (!CopyFileExW(m_ret + L"\\serie.mdb", m_ret + L"\\backup_manuale_serie_" + data + L".mdb", NULL, NULL, FALSE, COPY_FILE_ALLOW_DECRYPTED_DESTINATION))
		AfxMessageBox(m_currlang.GetDesc(74, temp),MB_ICONERROR);
	else
		AfxMessageBox(m_currlang.GetDesc(34,temp),MB_ICONINFORMATION);
}

// Funzione per svuotare il database
void DlgOptions::DistruggiDatabase()
{
	if (AfxMessageBox(m_currlang.GetDesc(139, temp), MB_OKCANCEL) != IDOK)
		return;
	
	// Elimino database
	CString m_ret = _T("");
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
	{
		ULONG m_len = 255;
		chiave.QueryStringValue((_T("SharedMDB")), m_ret.GetBufferSetLength(m_len), &m_len);
		m_ret.ReleaseBuffer();
	}
	if (!DeleteFileW(m_ret + L"\\" + L"serie.mdb"))
	{
		AfxMessageBox(m_currlang.GetDesc(74, temp), MB_ICONERROR);
		return;
	}
	// Copio "serzap.mdb" come nuovo database
	if (!CopyFileExW(m_ret + L"\\serzap.mdb", m_ret + L"\\serie.mdb", NULL, NULL, FALSE, COPY_FILE_ALLOW_DECRYPTED_DESTINATION))
	{
		AfxMessageBox(m_currlang.GetDesc(74, temp), MB_ICONERROR);
		return;
	}
	// Ricarico la vista col nuovo database
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;				// Puntatore alla mainframe
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();		// Puntatore alla vista principale
	CNoteSeriesDoc* pDoc = pView->GetDocument();							// Puntatore al documento

	pDoc->SendInitialUpdate();
	AfxMessageBox(m_currlang.GetDesc(34, temp), MB_ICONINFORMATION);
}

// Ripristino l'ultimo backup automatico
void DlgOptions::RipristinaBackup()
{
	if (AfxMessageBox(m_currlang.GetDesc(139, temp), MB_OKCANCEL) != IDOK)
		return;

	// Elimino database
	CString m_ret = _T("");
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
	{
		ULONG m_len = 255;
		chiave.QueryStringValue((_T("SharedMDB")), m_ret.GetBufferSetLength(m_len), &m_len);
		m_ret.ReleaseBuffer();
	}
	if (!DeleteFileW(m_ret + L"\\" + L"serie.mdb"))
	{
		AfxMessageBox(m_currlang.GetDesc(74, temp), MB_ICONERROR);
		return;
	}
	// Copio l'ultimo backup come nuovo database
	if (!CopyFileExW(m_ret + L"\\backup_serie.mdb", m_ret + L"\\serie.mdb", NULL, NULL, FALSE, COPY_FILE_ALLOW_DECRYPTED_DESTINATION))
	{
		AfxMessageBox(m_currlang.GetDesc(74, temp), MB_ICONERROR);
		return;
	}
	// Ricarico la vista col nuovo database
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;				// Puntatore alla mainframe
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();		// Puntatore alla vista principale
	CNoteSeriesDoc* pDoc = pView->GetDocument();							// Puntatore al documento

	pDoc->SendInitialUpdate();
	AfxMessageBox(m_currlang.GetDesc(35, temp), MB_ICONINFORMATION);
}

// Senza il backup automatico, disabilito
// il ripristino
void DlgOptions::CheckBackup()
{
	if (m_chkauto.GetCheck())
		m_btnold.EnableWindow(1);
	else
		m_btnold.EnableWindow(0);
}


void DlgOptions::EsportaTutto()
{
	if (m_chkall.GetCheck())
	{
		m_chkcat.SetCheck(1);
		m_chkfut.SetCheck(1);
		m_chkviste.SetCheck(1);
		m_table = 0;
	}
	else
	{
		m_chkcat.SetCheck(0);
		m_chkfut.SetCheck(0);
		m_chkviste.SetCheck(0);
		m_table = 5;
	}
}

void DlgOptions::checkViste()
{
	if (m_chkviste.GetCheck())
		m_table = 1;
}


void DlgOptions::checkFuture()
{
	if (m_chkfut.GetCheck())
		m_table = 2;
}


void DlgOptions::checkCat()
{
	if (m_chkcat.GetCheck())
		m_table = 3;
}


// Esporto le tabelle del db ".mdb" in ".sql"
void DlgOptions::EsportaInSQL()
{
	if ((m_chkfut.GetCheck() && m_chkviste.GetCheck()) || (m_chkviste.GetCheck() && m_chkcat.GetCheck()) || (m_chkfut.GetCheck() && m_chkcat.GetCheck()))
		m_table = 4;
	
	if (m_chkall.GetCheck())
		m_table = 0;
	
	if (!(m_chkfut.GetCheck() || m_chkviste.GetCheck() || m_chkcat.GetCheck()))
		return;

	if (AfxMessageBox(m_currlang.GetDesc(141, temp), MB_ICONINFORMATION | MB_YESNO) == IDNO)
		return;

	switch (m_table)
	{
	// TUTTE
	case 0:
	{
		CString nomef = _T("NoteSeries.sql");
		CString* pnome = nullptr;
		pnome = &nomef;
		if (getViste(pnome) && getFuture(pnome) && getCat(pnome))
			AfxMessageBox(m_currlang.GetDesc(34, temp), MB_ICONINFORMATION);
		else
		{
			AfxMessageBox(_T("ERROR"));
			return;
		}
		break;
	}
	// VISTE
	case 1:
	{
		CString nomef = _T("viste.sql");
		CString* pnome = nullptr;
		pnome = &nomef;
		if (!getViste(pnome))
		{
			AfxMessageBox(_T("ERROR"));
			return;
		}
		AfxMessageBox(m_currlang.GetDesc(34, temp), MB_ICONINFORMATION);
		break;
	}
	// FUTURE
	case 2:
	{
		CString nomef = _T("future.sql");
		CString* pnome = nullptr;
		pnome = &nomef;
		if (!getFuture(pnome))
		{
			AfxMessageBox(_T("ERROR"));
			return;
		}
		AfxMessageBox(m_currlang.GetDesc(34, temp), MB_ICONINFORMATION);
		break;
	}
	// CATEGORIE
	case 3:
	{
		CString nomef = _T("categorie.sql");
		CString* pnome = nullptr;
		pnome = &nomef;
		if (!getCat(pnome))
		{
			AfxMessageBox(_T("ERROR"));
			return;
		}
		AfxMessageBox(m_currlang.GetDesc(34, temp), MB_ICONINFORMATION);
		break;
	}
	case 4:
	{
		CString nomef = _T("export.sql");
		CString* pnome = nullptr;
		pnome = &nomef;
		if (m_chkviste.GetCheck() && m_chkfut.GetCheck())
		{
			if (getViste(pnome) && getFuture(pnome))
				AfxMessageBox(m_currlang.GetDesc(34, temp), MB_ICONINFORMATION);
			else
			{
				AfxMessageBox(_T("ERROR"));
				return;
			}
		}
		else if (m_chkviste.GetCheck() && m_chkcat.GetCheck())
		{
			if (getViste(pnome) && getCat(pnome))
				AfxMessageBox(m_currlang.GetDesc(34, temp), MB_ICONINFORMATION);
			else
			{
				AfxMessageBox(_T("ERROR"));
				return;
			}
		}
		else
		{
			if (getFuture(pnome) && getCat(pnome))
				AfxMessageBox(m_currlang.GetDesc(34, temp), MB_ICONINFORMATION);
			else
			{
				AfxMessageBox(_T("ERROR"));
				return;
			}
		}
		break;
	}

	default:
		break;
	}
}

BOOL DlgOptions::getViste(CString* nomefile)
{
	try
	{
		CString ida, nome, cartella, inizio, fine, stato, voto, commenti, sito, bitmap, idcat = _T("");
		CDbConn dbconn;
		dbconn.dbopen();
		CRecordset righe(dbconn.serie);
		righe.Open(CRecordset::forwardOnly, L"SELECT * FROM anime");
		// Percorso dove salva i file
		if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
		{
			len = 255;
			chiave.QueryStringValue((_T("Export")), dir.GetBufferSetLength(len), &len);
			dir.ReleaseBuffer();
		}
		// Creo la directory
		CreateDirectoryEx(dir, dir + L"\\SQL", 0);
		std::ofstream outfile(dir + L"\\SQL\\" + *nomefile, std::ios_base::app);
		outfile << "CREATE TABLE IF NOT EXISTS anime (" << std::endl;
		outfile << "ida int(11) NOT NULL AUTO_INCREMENT," << std::endl;
		outfile << "nome varchar(30) NOT NULL," << std::endl;
		outfile << "cartella varchar(30) NOT NULL," << std::endl;
		outfile << "inizio varchar(30) NOT NULL," << std::endl;
		outfile << "fine varchar(30) NULL," << std::endl;
		outfile << "stato int(11) NOT NULL," << std::endl;
		outfile << "voto int(11) NOT NULL," << std::endl;
		outfile << "commenti varchar(255) NULL," << std::endl;
		outfile << "sito varchar(30) NULL," << std::endl;
		outfile << "bitmap varchar(30) NULL," << std::endl;
		outfile << "IDCAT int(11) NOT NULL," << std::endl;
		outfile << "PRIMARY KEY(`ida`)" << std::endl;
		outfile << ") ENGINE = InnoDB  DEFAULT CHARSET = utf8 AUTO_INCREMENT = 4;" << std::endl << std::endl;
		while (!righe.IsEOF())
		{
			righe.GetFieldValue(L"ida", ida);
			righe.GetFieldValue(L"nome", nome);
			righe.GetFieldValue(L"cartella", cartella);
			righe.GetFieldValue(L"inizio", inizio);
			righe.GetFieldValue(L"fine", fine);
			righe.GetFieldValue(L"stato", stato);
			righe.GetFieldValue(L"voto", voto);
			righe.GetFieldValue(L"commenti", commenti);
			righe.GetFieldValue(L"sito", sito);
			righe.GetFieldValue(L"bitmap", bitmap);
			righe.GetFieldValue(L"IDCAT", idcat);

			outfile << "INSERT INTO anime (`ida`, `nome`,`cartella`, `inizio`,`fine`, `stato`,`voto`, `commenti`,`sito`, `bitmap`,`IDCAT`) VALUES" << std::endl;
			outfile << "(" << CT2A(ida) << "," << CT2A(L"\"" + nome + L"\"") << ","
				<< CT2A(L"\"" + cartella + L"\"") << "," << CT2A(L"\"" + inizio + L"\"") << "," << CT2A(L"\"" + fine + L"\"") << ","
				<< CT2A(stato) << "," << CT2A(voto) << "," << CT2A(L"\"" + commenti + L"\"") << ","
				<< CT2A(L"\"" + sito + L"\"") << "," << CT2A(L"\"" + bitmap + L"\"") << "," << CT2A(idcat) << ");" << std::endl << std::endl;
			righe.MoveNext();
		}
		righe.Close();
		dbconn.serie->Close();
		outfile.close();
	}
	catch (CDBException* e) 
	{ 
		AfxMessageBox(_T("Database error: ") + e->m_strError); 
		return FALSE;
	}
	return TRUE;
}

BOOL DlgOptions::getFuture(CString* nomefile)
{
	try
	{
		CString idv, nome, stato, commenti, priorita, idcat = _T("");
		CDbConn dbconn;
		dbconn.dbopen();
		CRecordset righe(dbconn.serie);
		righe.Open(CRecordset::forwardOnly, L"SELECT * FROM [da vedere]");
		// Percorso dove salva i file
		if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
		{
			len = 255;
			chiave.QueryStringValue((_T("Export")), dir.GetBufferSetLength(len), &len);
			dir.ReleaseBuffer();
		}
		// Creo la directory
		CreateDirectoryEx(dir, dir + L"\\SQL", 0);
		std::ofstream outfile(dir + L"\\SQL\\" + *nomefile, std::ios_base::app);
		outfile << "CREATE TABLE IF NOT EXISTS `da vedere` (" << std::endl;
		outfile << "idv int(11) NOT NULL AUTO_INCREMENT," << std::endl;
		outfile << "nome varchar(30) NOT NULL," << std::endl;
		outfile << "stato int(11) NOT NULL," << std::endl;
		outfile << "commenti varchar(255) NULL," << std::endl;
		outfile << "priorita int(11) NOT NULL," << std::endl;
		outfile << "IDCAT int(11) NOT NULL," << std::endl;
		outfile << "PRIMARY KEY(`idv`)" << std::endl;
		outfile << ") ENGINE = InnoDB  DEFAULT CHARSET = utf8 AUTO_INCREMENT = 4;" << std::endl << std::endl;
		while (!righe.IsEOF())
		{
			righe.GetFieldValue(L"idv", idv);
			righe.GetFieldValue(L"nome", nome);
			righe.GetFieldValue(L"stato", stato);
			righe.GetFieldValue(L"commenti", commenti);
			righe.GetFieldValue(L"priorità", priorita);
			righe.GetFieldValue(L"IDCAT", idcat);
			outfile << "INSERT INTO `da vedere` (`idv`, `nome`,`stato`, `commenti`,`priorita`, `IDCAT`) VALUES" << std::endl;
			outfile << "(" << CT2A(idv) << "," << CT2A(L"\"" + nome + L"\"") << "," << CT2A(stato) << "," << CT2A(L"\"" + commenti + L"\"") << "," << CT2A(priorita) << "," << CT2A(idcat) << ");" << std::endl << std::endl;
			righe.MoveNext();
		}
		righe.Close();
		dbconn.serie->Close();
		outfile.close();
	}
	catch (CDBException* e) 
	{ 
		AfxMessageBox(_T("Database error: ") + e->m_strError); 
		return FALSE;
	}
	return TRUE;
}

BOOL DlgOptions::getCat(CString* nomefile)
{
	try
	{
		CString idcat, nomecat = _T("");
		CDbConn dbconn;
		dbconn.dbopen();
		CRecordset righe(dbconn.serie);
		righe.Open(CRecordset::forwardOnly, L"SELECT * FROM CATEGORIE");
		// Percorso dove salva i file
		if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
		{
			len = 255;
			chiave.QueryStringValue((_T("Export")), dir.GetBufferSetLength(len), &len);
			dir.ReleaseBuffer();
		}
		// Creo la directory
		CreateDirectoryEx(dir, dir + L"\\SQL", 0);
		std::ofstream outfile(dir + L"\\SQL\\" + *nomefile, std::ios_base::app);
		outfile << "CREATE TABLE IF NOT EXISTS CATEGORIE (" << std::endl;
		outfile << "IDCAT int(11) NOT NULL AUTO_INCREMENT," << std::endl;
		outfile << "NOMECAT varchar(30) NOT NULL," << std::endl;
		outfile << "PRIMARY KEY(`IDCAT`)" << std::endl;
		outfile << ") ENGINE = InnoDB  DEFAULT CHARSET = utf8 AUTO_INCREMENT = 4;" << std::endl;
		while (!righe.IsEOF())
		{
			righe.GetFieldValue(L"IDCAT", idcat);
			righe.GetFieldValue(L"NOMECAT", nomecat);
			outfile << "INSERT INTO CATEGORIE (`IDCAT`, `NOMECAT`) VALUES" << std::endl;
			outfile << "(" << CT2A(idcat) << "," << CT2A(L"'" + nomecat + L"'") << ");" << std::endl;
			righe.MoveNext();
		}
		righe.Close();
		dbconn.serie->Close();
		outfile.close();
	}
	catch (CDBException* e) 
	{ 
		AfxMessageBox(_T("Database error: ") + e->m_strError); 
		return FALSE;
	}
	return TRUE;
}