// DlgPrimoAvvio.cpp: file di implementazione
//

#include "stdafx.h"
#include "Note Series.h"
#include "Note SeriesDoc.h"
#include "Note SeriesView.h"
#include "mainfrm.h"
#include "DlgPrimoAvvio.h"
#include "afxdialogex.h"


// Finestra di dialogo DlgPrimoAvvio

IMPLEMENT_DYNAMIC(DlgPrimoAvvio, CDialogEx)

DlgPrimoAvvio::DlgPrimoAvvio(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PRIMO_AVVIO, pParent)
{
	m_len = 255;
	m_count = 0;
	m_ret = _T("");
	m_vers = _T("");
	m_msg = _T("");
	m_tmp = _T("");
}

DlgPrimoAvvio::~DlgPrimoAvvio()
{
}

void DlgPrimoAvvio::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNSALVA, m_btnSalva);
	DDX_Control(pDX, IDC_CMBLINGUA, m_cmbLingua);
	DDX_Control(pDX, IDC_TXTCAT, m_categoria);
	DDX_Control(pDX, IDC_TXTNOME, m_nome);
	DDX_Control(pDX, IDC_TXTMAIL, m_mail);
	DDX_Control(pDX, IDC_TXTSOC, m_soc);
	DDX_Control(pDX, IDC_TXTTEL, m_tel);
	DDX_Control(pDX, IDC_LOGHI, m_loghi);
	DDX_Control(pDX, IDC_EXPORT, m_export);
	DDX_Control(pDX, IDC_CARTELLA, m_cartella);
	DDX_Control(pDX, IDC_LBLLANG, m_lblLang);
	DDX_Control(pDX, IDC_LBLCAT, m_lblCat);
	DDX_Control(pDX, IDC_GPBANAG, m_gpbAnag);
	DDX_Control(pDX, IDC_GPBPATH, m_gpbPath);
	DDX_Control(pDX, IDC_LBLNOME, m_lblNome);
	DDX_Control(pDX, IDC_LBLMAIL, m_lblMail);
	DDX_Control(pDX, IDC_LBLSOC, m_lblSoc);
	DDX_Control(pDX, IDC_LBLTEL, m_lblTel);
	DDX_Control(pDX, IDC_LBLLOGHI, m_lblLoghi);
	DDX_Control(pDX, IDC_LBLEXPORT, m_lblExport);
	DDX_Control(pDX, IDC_LBLRADICE, m_lblRadice);
	DDX_Control(pDX, IDC_LBLDB, m_lblDbVers);
	DDX_Control(pDX, IDC_LBLTITLE, m_lblTitle);
	DDX_Control(pDX, IDC_LBLNOTA, m_lblNota);
	DDX_Control(pDX, IDC_LBLSEZ1, m_lblSez1);
	DDX_Control(pDX, IDC_LBLSEZ2, m_lblSez2);
	DDX_Control(pDX, IDC_LBLSEZ3, m_lblSez3);
	DDX_Control(pDX, IDC_LBLESEMPI, m_lblEsempi);
}


BEGIN_MESSAGE_MAP(DlgPrimoAvvio, CDialogEx)
	ON_BN_CLICKED(IDC_BTNSALVA, &DlgPrimoAvvio::OnClickSalva)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Gestori di messaggi di DlgPrimoAvvio


BOOL DlgPrimoAvvio::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// Colori e dimensioni caratteri
	CFont *m_Title = new CFont();
	CFont *m_Sez = new CFont();
	m_Title->CreatePointFont(150, _T("Verdana"));
	m_Sez->CreatePointFont(120, _T("Verdana"));
	m_lblTitle.SetFont(m_Title, TRUE);
	m_lblSez1.SetFont(m_Sez, TRUE);
	m_lblSez2.SetFont(m_Sez, TRUE);
	m_lblSez3.SetFont(m_Sez, TRUE);
	this->SetBackgroundColor(RGB(255, 255, 255), 1);
	
	// Aggiungo riferimenti CEdit a CArray (NON serve a un cavolo, ma va beh)
	m_edit.Add(&m_nome);
	m_edit.Add(&m_tel);
	m_edit.Add(&m_soc);
	m_edit.Add(&m_mail);

	// Leggo le chiavi di registro nella sezione "Settings"
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings")))
	{
		for (short i = 0; i < m_edit.GetSize(); i++)
		{
			chiave.QueryStringValue((txtKey[i]), m_ret.GetBufferSetLength(m_len), &m_len);
			m_edit[i]->SetWindowTextW(m_ret);
			m_len = 255;
		}
		m_ret.ReleaseBuffer();
	}

	// Leggo le chiavi di registro nella sezione "Path"
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
	{
		m_edit.RemoveAll(); // Svuoto CArray
		m_edit.Add(&m_loghi); 
		m_edit.Add(&m_export);
		m_edit.Add(&m_cartella);
		for (short j = 0; j < m_edit.GetSize(); j++)
		{
			chiave.QueryStringValue((txtKeyPath[j]), m_ret.GetBufferSetLength(m_len), &m_len);
			m_edit[j]->SetWindowTextW(m_ret);
			m_len = 255;
		}
		m_ret.ReleaseBuffer();
	}
	CMenu* barraTitolo = GetSystemMenu(FALSE);

	if (barraTitolo != NULL)
	{
		// Disabilito tasto chiusura (X)
		barraTitolo->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_DISABLED);
	}

	GetVersioneDB(); // Versione "serie.mdb" 
	GetDescUpdate(); // Traduzioni
	return TRUE;  
}

void DlgPrimoAvvio::GetDescUpdate()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;				// Puntatore alla mainframe
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();		// Puntatore alla vista principale
	CNoteSeriesDoc* pDoc = pView->GetDocument();							// Puntatore al documento
	
	// Categoria
	m_categoria.SetWindowTextW(_T("SAMPLE"));

	// Combo della lingua
	m_cmbLingua.AddString(m_currlang.GetDesc(5, m_tmp)); // italiano
	m_cmbLingua.AddString(m_currlang.GetDesc(6, m_tmp)); // inglese
	
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings")))
	{
		if (GetUserDefaultUILanguage() == 0x0410) // 0x0410 id italiano
			chiave.SetStringValue(_T("Lingua"), (L"ITALIANO"));
		else
			chiave.SetStringValue(_T("Lingua"), (L"ENGLISH"));

		m_len = 255;
		chiave.QueryStringValue((_T("Lingua")), m_ret.GetBufferSetLength(m_len), &m_len);
		m_ret.ReleaseBuffer();
	}

	if (m_ret == m_currlang.GetDesc(5, m_tmp))
		m_cmbLingua.SetCurSel(0); // ITALIANO
	else
		m_cmbLingua.SetCurSel(1); // INGLESE

	m_btnSalva.SetWindowTextW(m_currlang.GetDesc(117,m_tmp));
	m_lblLang.SetWindowTextW(m_currlang.GetDesc(121, m_tmp));
	m_lblCat.SetWindowTextW(m_currlang.GetDesc(122, m_tmp));
	m_gpbAnag.SetWindowTextW(m_currlang.GetDesc(118, m_tmp));
	m_gpbPath.SetWindowTextW(m_currlang.GetDesc(119, m_tmp));
	m_lblNome.SetWindowTextW(m_currlang.GetDesc(37, m_tmp));
	m_lblSoc.SetWindowTextW(m_currlang.GetDesc(11, m_tmp));
	m_lblLoghi.SetWindowTextW(m_currlang.GetDesc(19, m_tmp));
	m_lblExport.SetWindowTextW(m_currlang.GetDesc(30, m_tmp));
	m_lblRadice.SetWindowTextW(m_currlang.GetDesc(10, m_tmp));
	m_lblTitle.SetWindowTextW(m_currlang.GetDesc(113, m_tmp));
	m_lblNota.SetWindowTextW(m_currlang.GetDesc(120, m_tmp));
	m_lblSez1.SetWindowTextW(m_currlang.GetDesc(114, m_tmp));
	m_lblSez2.SetWindowTextW(m_currlang.GetDesc(115, m_tmp));
	m_lblSez3.SetWindowTextW(m_currlang.GetDesc(116, m_tmp));
	m_lblEsempi.SetWindowTextW(m_currlang.GetDesc(129, m_tmp));

	if(m_vers != pDoc->m_dbvers)
		m_lblDbVers.SetWindowTextW(m_currlang.GetDesc(123, m_tmp) + L" " + pDoc->m_dbvers);
	else
		m_lblDbVers.SetWindowTextW(m_currlang.GetDesc(128, m_tmp));
}

int DlgPrimoAvvio::ContaCategorie(CString* sql)
{
	CString temp = _T("");
	dbconfig.dbopen();
	try
	{
		CRecordset righe(dbconfig.serie);
		righe.Open(CRecordset::forwardOnly,L"SELECT IDCAT FROM CATEGORIE");
		while (!righe.IsEOF())
		{
		m_count = righe.GetRecordCount();
			righe.MoveNext();
		}
		righe.Close();
		dbconfig.serie->Close();
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("Database error: ") + e->m_strError);
		return -1;
	}
	return m_count;
}

void DlgPrimoAvvio::OnClickSalva()
{
	if (AfxMessageBox(m_currlang.GetDesc(126, m_tmp), MB_YESNO) == IDNO)
		return;

	CString cat, newKey, sql, inseCat = _T("");

	
	
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;				// Puntatore alla mainframe
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();		// Puntatore alla vista principale
	CNoteSeriesDoc* pDoc = pView->GetDocument();							// Puntatore al documento

	// Backup automatico
	CRegKey chiave;
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings")))
	{
		DWORD back = 0;
		CString m_ret = _T("");
		chiave.QueryDWORDValue(_T("Backup"), back);
		if (back == 1)
		{
			if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
			{
				ULONG m_len = 255;
				chiave.QueryStringValue((_T("SharedMDB")), m_ret.GetBufferSetLength(m_len), &m_len);
				m_ret.ReleaseBuffer();
			}
			// Creo copia del database "serie.mdb"
			if (!CopyFileExW(m_ret + L"\\serie.mdb", m_ret + L"\\backup_serie.mdb", NULL, NULL, FALSE, COPY_FILE_ALLOW_DECRYPTED_DESTINATION))
			{
				m_lblDbVers.SetWindowTextW(m_currlang.GetDesc(125, m_tmp));
				return;
			}
		}
	}
	// Controllo versione del db
	if (m_vers != pDoc->m_dbvers)
	{
		if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
		{
			m_len = 255;
			chiave.QueryStringValue((_T("SharedMDB")), m_ret.GetBufferSetLength(m_len), &m_len);
			m_ret.ReleaseBuffer();
		}
		// Creo copia del database "serie.mdb"
		//if (!CopyFileExW(m_ret + L"\\serie.mdb", m_ret + L"\\backup_serie_" + pDoc->m_backup + L".mdb", NULL, NULL, FALSE, COPY_FILE_ALLOW_DECRYPTED_DESTINATION))
		//{
		//	//AfxMessageBox((m_currlang.GetDesc(126, m_tmp)), MB_ICONWARNING);
		//	m_lblDbVers.SetWindowTextW(m_currlang.GetDesc(126, m_tmp));
		//	return;
		//}

		// Aggiorno il database "serie.mdb"
		if (!AggiornaDB())
		{
			m_lblDbVers.SetWindowTextW(m_currlang.GetDesc(124, m_tmp));
			return;
		}
	}
	
	// Se nel db non ci sono categorie rendo obbligatorio compilare la casella
	sql.Format(L"SELECT COUNT(IDCAT) FROM CATEGORIE");
	ContaCategorie(&sql);
	if (m_count < 1)
	{
		m_categoria.GetWindowTextW(cat);
		if (cat == _T(""))
		{
			AfxMessageBox(m_currlang.GetDesc(127, m_tmp));
			return;
		}
		inseCat.Format(L"INSERT INTO [CATEGORIE] (NOMECAT) VALUES('%s')", cat.GetString()); // Inserisco categoria
		dbconfig.SetSerie(inseCat);
	}
	if (pDoc->m_dbvers == _T("1.0.0.1"))
	{
		if (EliminaIndici())
			AfxMessageBox(_T("Pulizia indici superflui completata, clicca ok per proseguire"));
	}
	// Aggiorno le chiavi di registro
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings")))
	{
		m_nome.GetWindowText(newKey);
		chiave.SetStringValue(_T("Utente"), (newKey));

		m_cmbLingua.GetWindowText(newKey);
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
		
		m_export.GetWindowText(newKey);
		chiave.SetStringValue(_T("Export"), (newKey));
	}
	CDialogEx::OnOK();
}

CString* DlgPrimoAvvio::GetVersioneDB()
{
	dbconfig.dbopen();
	try
	{
		CRecordset righe(dbconfig.serie);
		righe.Open(CRecordset::forwardOnly, L"SELECT VERSIONE FROM [VERSIONI] WHERE IDVERS=1");
		while (!righe.IsEOF())
		{
			righe.GetFieldValue(L"VERSIONE", m_vers);
			righe.MoveNext();
		}
		righe.Close();
		dbconfig.serie->Close();
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("Database error: ") + e->m_strError);
	}
	return &m_vers;
}

BOOL DlgPrimoAvvio::AggiornaDB()
{
	// Query di aggiornamento
	if (m_vers == L"1.0.0.1")
	{
		CString creaTab = _T("CREATE TABLE CATEGORIE (IDCAT AUTOINCREMENT PRIMARY KEY, NOMECAT VARCHAR(255) NOT NULL)");	// Creo tabella [categorie]
		CString colsCat = _T("ALTER TABLE [anime] ADD COLUMN IDCAT INTEGER");												// Chiave esterna nella tabella [anime]
		CString colsFut = _T("ALTER TABLE [da vedere] ADD COLUMN IDCAT INTEGER");											// Chiave esterna nella tabella [da vedere]
		CString catVist = _T("UPDATE [anime] SET IDCAT=1");																	// Chiave esterna a 1 di default
		CString catFut = _T("UPDATE [da vedere] SET IDCAT=1");
		CString updVers = _T("UPDATE [VERSIONI] SET VERSIONE='1.0.0.2' WHERE IDVERS=1");									// Aggiorno versione del database

		// Eseguo le query di aggiornamento
		try
		{
			dbconfig.SetSerie(creaTab);
			dbconfig.SetSerie(colsCat);
			dbconfig.SetSerie(colsFut);
			dbconfig.SetSerie(catVist);
			dbconfig.SetSerie(catFut);
			dbconfig.SetSerie(updVers);
		}
		catch (CException * exception)
		{
			exception->ReportError();
			return FALSE;
		}
	}
	if (m_vers == L"1.0.0.2")
	{
		CString creaTab = _T("CREATE TABLE ETICHETTE (IDL AUTOINCREMENT PRIMARY KEY, NOME VARCHAR(255) NOT NULL)");	// Creo tabella [ETICHETTE]
		CString colsCat = _T("ALTER TABLE [anime] ADD COLUMN IDL INTEGER");						// Chiave esterna nella tabella [anime]
		CString colsFut = _T("ALTER TABLE [da vedere] ADD COLUMN IDL INTEGER");					// Chiave esterna nella tabella [da vedere]
		CString catVist = _T("UPDATE [anime] SET IDL=0");										// Chiave esterna a 0 di default
		CString catFut = _T("UPDATE [da vedere] SET IDL=0");									// Chiave esterna a 0 di default
		CString updVers = _T("UPDATE [VERSIONI] SET VERSIONE='1.0.0.3' WHERE IDVERS=1");		// Aggiorno versione del database

		// Eseguo le query di aggiornamento
		try
		{
			dbconfig.SetSerie(creaTab);
			dbconfig.SetSerie(colsCat);
			dbconfig.SetSerie(colsFut);
			dbconfig.SetSerie(catVist);
			dbconfig.SetSerie(catFut);
			dbconfig.SetSerie(updVers);
		}
		catch (CException * exception)
		{
			exception->ReportError();
			return FALSE;
		}
	}
	return TRUE;
}


// Filtro pulsanti premuti sulla tastiera
BOOL DlgPrimoAvvio::PreTranslateMessage(MSG* pMsg)
{
	// Pressione tasto
	if (pMsg->message == WM_KEYDOWN)
	{
		// Blocco pulsanti ESC e INVIO
		if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
			return TRUE;
	}
	// Blocco combinazione ALT+F4
	if (VK_F4 == pMsg->wParam)
	{
		if (::GetKeyState(VK_MENU) < 0)
			return TRUE; 
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL DlgPrimoAvvio::EliminaIndici()
{
	dbconfig.dbopen();
	try
	{
		dbconfig.serie->ExecuteSQL(L"DROP INDEX nome on [anime]");
		dbconfig.serie->ExecuteSQL(L"DROP INDEX nome on [da vedere]");
		dbconfig.serie->Close();
		return TRUE;
	}
	catch (CDBException* e)
	{
		dbconfig.serie->Close();
		AfxMessageBox(_T("Database error: ") + e->m_strError);
		return FALSE;
	}
}