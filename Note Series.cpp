// Note Series.cpp: definisce i comportamenti delle classi per l'applicazione.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Note Series.h"
#include "MainFrm.h"
#include "DlgOptions.h"

#include "CUpgrade.h"
#include "Note SeriesDoc.h"
#include "Note SeriesView.h"
#include "LeftView.h"

#include "DlgVersioni.h"
#include "DlgPrimoAvvio.h"
#include "DlgRiconoscimenti.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CNoteSeriesApp

BEGIN_MESSAGE_MAP(CNoteSeriesApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CNoteSeriesApp::OnAppAbout)
	ON_COMMAND(ID_FILE_OPTIONS, &CNoteSeriesApp::ShowOptions)
	ON_COMMAND(ID_OPTIONS2, &CNoteSeriesApp::ShowOptions)
	// Comandi di documenti basati su file standard
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_STAMPA, &CNoteSeriesApp::OnPrintDoc)
	ON_COMMAND(ID_STAMPA2, &CNoteSeriesApp::OnPrintDoc)
END_MESSAGE_MAP()


// Costruzione di CNoteSeriesApp

CNoteSeriesApp::CNoteSeriesApp()
{
	// supporto Gestione riavvio
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// Se l'applicazione è compilata mediante il supporto CLR (/clr):
	//     1) Questa ipostazione è necessaria per il corretto funzionamento del supporto di Gestione riavvio.
	//     2) Nel progetto è necessario aggiungere un riferimento a System.Windows.Forms per eseguire la compilazione.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: sostituire la stringa dell'ID applicazione seguente con una stringa di ID univoca; il formato consigliato
	// per la stringa è NomeSocietà.NomeProdotto.Sottoprodotto.Versione
	SetAppID(_T("MCSOFTWARE.NoteSeries.1.1.2"));

	// TODO: aggiungere qui il codice di costruzione.
	// Inserire l'inizializzazione significativa in InitInstance.
}
// Unico oggetto CNoteSeriesApp

CNoteSeriesApp theApp;

// Inizializzazione di CNoteSeriesApp

BOOL CNoteSeriesApp::InitInstance()
{
	// InitCommonControlsEx() è necessario in Windows XP se nel manifesto
	// di un'applicazione è specificato l'utilizzo di ComCtl32.dll versione 6 o successiva per abilitare
	// gli stili visuali.  In caso contrario, non sarà possibile creare finestre.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Effettuare questa impostazione in modo da includere tutte le classi di controlli comuni da utilizzare
	// nell'applicazione.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinAppEx::InitInstance();

	// Inizializzare le librerie OLE.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	EnableTaskbarInteraction(FALSE);
	// Chiavi di registro
	CRegKey chiave;
	PWSTR path = _T(""); // Usata per conservare percorsi assoluti
	CString pathExp = _T(""); // Percroso assoluto cartella documenti + cartella salvataggio file Excel
	SetRegistryKey(_T("MC SOFTWARE"));
	LoadStdProfileSettings(4);  // Caricare le opzioni del file INI standard (inclusa MRU).
	// Chiavi di registro predefinite
	if (chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
	{
		// Percorsi
		if (ERROR_SUCCESS == chiave.Create(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
		{
			// Posizione del file template per export in Excel
			chiave.SetStringValue(_T("Template"), (L"Export\\template.xlsx"));

			// Cartella download, predefinita per il salvataggio delle serie
			SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &path);
			chiave.SetStringValue(_T("Cartella"), (path));
			// Cartella archivi
			chiave.SetStringValue(_T("SharedMDB"), (L"Archivi\\"));
			//Cartella dei loghi
			chiave.SetStringValue(_T("Loghi"), (L"BMP\\"));
			
			//Cartella dei file esportati
			SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);
			pathExp.Format(_T("%s\\Export Note Series"),path);
			
			CreateDirectoryEx(path, pathExp,0);
			chiave.SetStringValue(_T("Export"), (pathExp));
			CreateDirectoryEx(pathExp, pathExp+L"\\"+L"Serie", 0);
			CreateDirectoryEx(pathExp, pathExp+L"\\"+L"Excel", 0);
			CreateDirectoryEx(pathExp, pathExp+L"\\"+L"SQL", 0);
			CreateDirectoryEx(pathExp, pathExp + L"\\" + L"PDF", 0);
		}
		// Opzioni
		if (ERROR_SUCCESS == chiave.Create(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings")))
		{
			chiave.SetDWORDValue(_T("Register"), (0));
			chiave.SetDWORDValue(_T("Backup"), (1));
			
			if (GetUserDefaultUILanguage() == 0x0410) // 0x0410 id italiano
				chiave.SetStringValue(_T("Lingua"), (L"ITALIANO"));
			else
				chiave.SetStringValue(_T("Lingua"), (L"ENGLISH"));

			chiave.SetStringValue(_T("Utente"), (L"User"));
			chiave.SetStringValue(_T("Società"), (L"-"));
			chiave.SetStringValue(_T("Mail"), (L"mail@mail.com"));
			chiave.SetStringValue(_T("Tel"), (L"999-999-999-9"));
		}
	}
	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Registrare i modelli di documenti dell'applicazione.  I modelli di documenti
	//  funzionano da connessione tra documenti, finestre cornice e viste.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CNoteSeriesDoc),
		RUNTIME_CLASS(CMainFrame),       // finestra cornice SDI principale
		RUNTIME_CLASS(CLeftView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	

	// Analizzare la riga di comando per i comandi shell standard, DDE, apri file
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Invia i comandi specificati nella riga di comando.  Restituisce FALSE se
	// l'applicazione è stata avviata con l'opzione /RegServer, /Register, /Unregserver o /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// L'unica e sola finestra è stata inizializzata, quindi è possibile visualizzarla e aggiornarla.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED/*SW_SHOW*/);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CNoteSeriesApp::ExitInstance()
{
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// Gestori di messaggi di CNoteSeriesApp


// finestra di dialogo CAboutDlg utilizzata per visualizzare le informazioni sull'applicazione.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // supporto DDX/DDV

// Implementazione
protected:
	DECLARE_MESSAGE_MAP()
public:
	// Data di rilascio
	CStatic m_lblRel;
	CStatic m_lblTitle;
	virtual BOOL OnInitDialog();
	CStatic m_desc;
	afx_msg void OnBnClickedOk3();
	afx_msg void OnBnClickedSito();
	CButton m_btnVersioni;
	CButton m_btnSito;
	CButton m_btnUpdate;
	afx_msg void OnBnClickedBtnupdate();
	// Versione programma
	CStatic m_lblVers;
	// Versione del database
	CStatic m_lblVersDb;
	// Autore
	CStatic m_lblAut;
	// Copyright
	CStatic m_lblCopy;
	// Tutti i diritti sono riservati
	CStatic m_lblDiritti;
	CButton m_btnRiconoscimenti;
	afx_msg void MostraRiconoscimenti();
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LBLREL, m_lblRel);
	DDX_Control(pDX, IDC_TITLE, m_lblTitle);
	DDX_Control(pDX, IDC_DESC, m_desc);
	DDX_Control(pDX, IDOK3, m_btnVersioni);
	DDX_Control(pDX, IDC_SITO, m_btnSito);
	DDX_Control(pDX, IDC_BTNUPDATE, m_btnUpdate);
	DDX_Control(pDX, IDC_LBLVERSIONE, m_lblVers);
	DDX_Control(pDX, IDC_LBLVERSDB, m_lblVersDb);
	DDX_Control(pDX, IDC_LBLCOPYAUTHOR, m_lblAut);
	DDX_Control(pDX, IDC_LBLCOPY, m_lblCopy);
	DDX_Control(pDX, IDC_LBLDIRITTI, m_lblDiritti);
	DDX_Control(pDX, IDC_RICONOSCIMENTI, m_btnRiconoscimenti);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK3, &CAboutDlg::OnBnClickedOk3)
	ON_BN_CLICKED(IDC_SITO, &CAboutDlg::OnBnClickedSito)
	ON_BN_CLICKED(IDC_BTNUPDATE, &CAboutDlg::OnBnClickedBtnupdate)
	ON_BN_CLICKED(IDC_RICONOSCIMENTI, &CAboutDlg::MostraRiconoscimenti)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	this->SetBackgroundColor(RGB(255, 255, 255));
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();
	CNoteSeriesDoc* pDoc = pView->GetDocument();
	// Dimensioni caratteri
	CFont *m_pFont = new CFont();
	CString temp = _T("");
	m_pFont->CreatePointFont(150, _T("Verdana"));
	m_lblTitle.SetFont(m_pFont, TRUE);

	m_desc.SetWindowTextW(m_currlang.GetDesc(32,temp));											// Descrizione programma
	m_lblRel.SetWindowTextW(m_currlang.GetDesc(33,temp));										// Data di release
	m_btnSito.SetWindowTextW(m_currlang.GetDesc(70, temp));										// Pagina web
	m_btnVersioni.SetWindowTextW(m_currlang.GetDesc(71, temp));									// Versioni
	m_lblVers.SetWindowTextW(m_currlang.GetDesc(133, temp) + L" " + pDoc->m_versione);		    // Versione programma
	m_lblVersDb.SetWindowTextW(m_currlang.GetDesc(134, temp) + L" " + pDoc->m_dbvers);			// Versione database
	m_lblAut.SetWindowTextW(m_currlang.GetDesc(131, temp));										// Autore
	m_lblCopy.SetWindowTextW(m_currlang.GetDesc(132, temp));									// Copyright
	m_lblDiritti.SetWindowTextW(m_currlang.GetDesc(135, temp));									// Diritti
	m_btnUpdate.SetWindowTextW(m_currlang.GetDesc(81, temp));									// Cerca aggiornamenti
	m_btnRiconoscimenti.SetWindowTextW(m_currlang.GetDesc(137, temp));							// Riconoscimenti
	return TRUE;  
}

// comando dell'applicazione per eseguire la finestra di dialogo
void CNoteSeriesApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
	delete aboutDlg;
}

void CNoteSeriesApp::ShowOptions()
{
	DlgOptions optionsDlg = new DlgOptions();
	optionsDlg.DoModal();
	delete optionsDlg;
}

// Metodi di caricamento/salvataggio personalizzazione di CNoteSeriesApp

void CNoteSeriesApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	//GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CNoteSeriesApp::LoadCustomState()
{
}

void CNoteSeriesApp::SaveCustomState()
{
}

// Gestori di messaggi di CNoteSeriesApp

// Apro mini griglia versioni
void CAboutDlg::OnBnClickedOk3()
{
	DlgVersioni versioni;
	versioni.DoModal();
}


void CAboutDlg::OnBnClickedSito()
{
	// Apro la mia pagina web
	ShellExecute(NULL, L"open", L"https://michelecaligiuri.altervista.org/noteseries", NULL, NULL, SW_SHOWMAXIMIZED);
}


// Controllo aggiornamenti programma
void CAboutDlg::OnBnClickedBtnupdate()
{
	CUpgrade upd = new CUpgrade();
	upd.DoModal();
	delete upd;
	CDialogEx::OnOK();
}


void CAboutDlg::MostraRiconoscimenti()
{
	DlgRiconoscimenti ric = new DlgRiconoscimenti();
	if(!(ric.DoModal()))
		return;
	delete ric;
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	CBrush brush;

	// Sfondo 
	pDC->SetBkColor(RGB(255, 255, 255));
	if ((HBRUSH)brush == NULL)
		brush.CreateSolidBrush(RGB(255, 255, 255));

	// Restituisce oggetto CBrush
	return (HBRUSH)brush;
}

// Stampa
void CNoteSeriesApp::OnPrintDoc()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();
	CNoteSeriesDoc* pDoc = pView->GetDocument();

	// Se nella tabella c'è solo un record si riferisce
	// alla riga vuota per far spazio dall'intestazione
	// di colonna, ma non c'è nessun dato da stampare
	if (pDoc->m_gridCount == 1)
		return;

	CPrintDialog printDialog(FALSE);

	if (!printDialog.GetDefaults())
	{
		AfxMessageBox(_T("You have not default printer!"));
		return;
	}

	CDC* pDC = new CDC;
	pDC->Attach(printDialog.m_pd.hDC);
	printDialog.m_pd.Flags &= ~PD_RETURNDEFAULT;

	if (printDialog.DoModal() == IDOK)
	{
		// Carta formato A4 Portrait
		DEVMODE* pDevMode = printDialog.GetDevMode();
		pDevMode->dmFields = DM_ORIENTATION | DM_PAPERSIZE | DM_PRINTQUALITY;
		pDevMode->dmOrientation = DMORIENT_PORTRAIT;
		pDevMode->dmPaperSize = DMPAPER_A4;
		::GlobalUnlock(printDialog.m_pd.hDevMode);

		// Nuovo documento
		pDC->StartDocW(_T("NoteSeries Export"));
		CRect ret(2000, 400, 50000, 50000);
		CRect tit(1900, 100, 50000, 50000);
		CString pag = _T("");
		short count = 0;
		pDC->StartPage();

		// Titolo
		pDC->DrawText(L"NOTE SERIES EXPORT", tit, DT_WORDBREAK | DT_LEFT);

		// Scorro l'array di struct della griglia
		for (int i = 1; i < pDoc->m_gridCount; i++) // Parto da 1 a causa della riga vuota
		{
			pag.Format(_T("%d"), i);
			// Stampo 4 elementi per pagina
			if (count == 4)
			{
				pDC->EndPage();
				pDC->StartPage();
				m_grid = _T("");
				count = 0;
			}
			m_grid +=
				L"nome:  " + pDoc->m_griglia[i].nome + L"\n" +
				L"cartella: " + pDoc->m_griglia[i].cartella + L"\n" +
				L"inizio: " + pDoc->m_griglia[i].inizio + L"\n" +
				L"fine: " + pDoc->m_griglia[i].fine + L"\n" +
				L"voto: " + pDoc->m_griglia[i].voto + L"\n" +
				L"sito: " + pDoc->m_griglia[i].sito + L"\n" +
				L"priorità: " + pDoc->m_griglia[i].priorità + L"\n" +
				L"stato: " + pDoc->m_griglia[i].stato + L"\n" +
				L"commento: " + pDoc->m_griglia[i].commento + L"\n_______________________\n" + pag + L"\n\n\n";

			pDC->DrawText(m_grid, ret, DT_WORDBREAK | DT_LEFT);
			count++;
		}
		pDC->EndPage();
		pDC->EndDoc();
		pDC->DeleteDC();
	}
}

