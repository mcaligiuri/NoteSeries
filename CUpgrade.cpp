// CUpgrade.cpp: file di implementazione
//

#include "stdafx.h"
#include "Note Series.h"
#include "CUpgrade.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "Note SeriesDoc.h"
#include "Note SeriesView.h"
#include <WinInet.h>

#include <fstream>
#include <string>

// Finestra di dialogo CUpgrade

IMPLEMENT_DYNAMIC(CUpgrade, CDialogEx)

CUpgrade::CUpgrade(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UPGRADE, pParent)
{
	temp = _T("");
	msg = _T("");
}

CUpgrade::~CUpgrade()
{
}

void CUpgrade::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LBLTITOLO, m_lblTtitolo);
	DDX_Control(pDX, IDC_LBLVERSIONE, m_lblVersione);
	DDX_Control(pDX, IDC_LBLNUOVAVERSIONE, m_lblNversione);
	DDX_Control(pDX, IDC_BTNINSTALLA, m_btnInstalla);
	DDX_Control(pDX, IDC_BTNANNULLA, m_btnAnnulla);
}


BEGIN_MESSAGE_MAP(CUpgrade, CDialogEx)
	ON_BN_CLICKED(IDC_BTNANNULLA, &CUpgrade::Annulla)
	ON_BN_CLICKED(IDC_BTNINSTALLA, &CUpgrade::Installa)
	//ON_WM_TIMER()
END_MESSAGE_MAP()


// Gestori di messaggi di CUpgrade


BOOL CUpgrade::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	using namespace std;
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();
	CNoteSeriesDoc* pDoc = pView->GetDocument();
	
	// Colori e dimensioni caratteri
	CFont *m_Title = new CFont();
	m_Title->CreatePointFont(120, _T("Verdana"));
	m_lblTtitolo.SetFont(m_Title, TRUE);
	this->SetBackgroundColor(RGB(255, 255, 255), 1);

	// Scarico file remoto con la versione
	DeleteUrlCacheEntry(_T("https://michelecaligiuri.altervista.org/noteseries/versione.txt"));
	URLDownloadToFile(0, _T("https://michelecaligiuri.altervista.org/noteseries/versione.txt"), _T("versione.txt"), 0, 0);
	
	// Controllo versione del file remoto con quella del programmma
	fstream versione("versione.txt");
	if (!versione)
		return FALSE;

	string riga;
	getline(versione, riga);
	
	// Se le due versioni sono uguali non apro il dialog
	if (riga.c_str() == pDoc->m_versione)
	{
		AfxMessageBox(m_currlang.GetDesc(82, msg));
		versione.close();
		EndDialog(0);
		return FALSE;
	}
	// Altrimenti scarico l'aggiornamento e termino di aprire il dialog
	DeleteUrlCacheEntry(_T("https://michelecaligiuri.altervista.org/noteseries/NSUPDATE.exe"));
	URLDownloadToFile(0, _T("https://michelecaligiuri.altervista.org/noteseries/NSUPDATE.exe"), _T("NSUPDATE.EXE"), 0, 0);

	// Aggiorno traduzioni del dialog
	GetDescUpdate();
	
	return TRUE;
}

void CUpgrade::GetDescUpdate()
{
	using namespace std;
	ifstream versione("versione.txt");
	if (!versione)
		return;
	
	// Estraggo riga dal file di testo
	string riga;
	getline(versione, riga);

	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd; // Puntatore mnframe
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView(); // Puntatore vista principale
	CNoteSeriesDoc* pDoc = pView->GetDocument(); // Puntatore documento

	m_lblTtitolo.SetWindowTextW(m_currlang.GetDesc(76,temp));
	m_lblVersione.SetWindowTextW(m_currlang.GetDesc(77,temp) + pDoc->m_versione);
	m_lblNversione.SetWindowTextW(m_currlang.GetDesc(78,temp) + riga.c_str());
	m_btnInstalla.SetWindowTextW(m_currlang.GetDesc(79,temp));
	m_btnAnnulla.SetWindowTextW(m_currlang.GetDesc(80,temp));
	
	versione.close();
}

// Installo l'aggiornamento
void CUpgrade::Installa()
{
	CRegKey chiave;
	// Controllo se è la prima volta che il programma viene avviato
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings")))
	{
		DWORD reg = 0;
		DWORD bac = 0;
		
		// Primo avvio
		chiave.QueryDWORDValue(_T("Register"), reg);
		if (reg != 0)
			chiave.SetDWORDValue(L"Register", 0);
		
		// Backup
		chiave.QueryDWORDValue(_T("Backup"), bac);
		if (bac != 1)
			chiave.SetDWORDValue(L"Backup", 1);
	}
	// Faccio partire l'installazione dell'aggiornamento chiudendo il programma
	ShellExecute(NULL, L"open", L"NSUPDATE.exe", NULL, NULL, SW_NORMAL);
	PostQuitMessage(0);
}

// Chiudo il dialog senza fare nulla
void CUpgrade::Annulla()
{
	EndDialog(0);
}