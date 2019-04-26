// DlgPdf.cpp: file di implementazione
//

#include "stdafx.h"
#include <MainFrm.h>
#include "Note SeriesView.h"
#include "Note Series.h"
#include "DlgPdf.h"
#include "afxdialogex.h"

// Finestra di dialogo DlgPdf

IMPLEMENT_DYNAMIC(DlgPdf, CDialogEx)

DlgPdf::DlgPdf(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PDF, pParent)
{
	pDoc = nullptr;
	m_pBtnFont = new CFont();
	m_SizeTitle = new CFont();
	m_temp = _T("");
	m_nome = _T("");
	m_cartella = _T("");
	m_inizio = _T("");
	m_fine = _T("");
	m_stato = _T("");
	m_priorità = _T("");
	m_sito = _T("");
	m_voto = _T("");
	m_commento = _T("");
	len = 255;
	retLang = _T("");
	retPath = _T("");
}

DlgPdf::~DlgPdf()
{
	delete m_pBtnFont;
	delete m_SizeTitle;
}

void DlgPdf::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LBLTITLE, m_lblTitle);
	DDX_Control(pDX, IDC_BTNCREA, m_btnCrea);
	DDX_Control(pDX, IDC_BTNRES, m_btnCanc);
	DDX_Control(pDX, IDC_LBLNOME, m_lblNome);
	DDX_Control(pDX, IDC_TXTNOME, m_txtNome);
	DDX_Control(pDX, IDC_BTNAPRIPDF, m_btnApriPdf);
	DDX_Control(pDX, IDC_BTNAPRIDIR, m_btnApriDir);
	DDX_Control(pDX, IDC_LSTPDF, m_lstPdf);
	DDX_Control(pDX, IDC_BTNDELPDF, m_btnElimina);
	DDX_Control(pDX, IDC_TXTREN, m_txtRen);
	DDX_Control(pDX, IDC_BUTRIN, m_btnRin);
}


BEGIN_MESSAGE_MAP(DlgPdf, CDialogEx)
	ON_BN_CLICKED(IDC_BTNRES, &DlgPdf::Annulla)
	ON_BN_CLICKED(IDC_BTNCREA, &DlgPdf::SetPdf)
	ON_BN_CLICKED(IDC_BTNAPRIDIR, &DlgPdf::ApriCartellaPdf)
	ON_BN_CLICKED(IDC_BTNAPRIPDF, &DlgPdf::ApriPdf)
	ON_LBN_SELCHANGE(IDC_LSTPDF, &DlgPdf::GetChange)
	ON_BN_CLICKED(IDC_BTNDELPDF, &DlgPdf::EliminaPdf)
	ON_BN_CLICKED(IDC_BUTRIN, &DlgPdf::RinominaPdf)
	ON_LBN_DBLCLK(IDC_LSTPDF, &DlgPdf::OnDoubleClick)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// Gestori di messaggi di DlgPdf


BOOL DlgPdf::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	pDoc = GetDoc();

	// Controllo la lingua corrente e il path per i pdf
	
	if (chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings")))
		return FALSE;

	chiave.QueryStringValue(_T("Lingua"), retLang.GetBufferSetLength(len), &len);
	retLang.ReleaseBuffer();

	if (chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
		return FALSE;

	len = 255;
	chiave.QueryStringValue(_T("Export"), retPath.GetBufferSetLength(len), &len);
	retPath.ReleaseBuffer();

	// Creo cartella per i pdf se non esiste
	CreateDirectoryEx(retPath, retPath + L"\\PDF", 0);

	// Lista pdf
	GetList();
	this->SetBackgroundColor(RGB(210, 46, 46)); // Sfondo bianco
	m_SizeTitle->CreatePointFont(130, _T("Verdana"));
	m_lblTitle.SetFont(m_SizeTitle);
	m_pBtnFont->CreatePointFont(110, _T("Verdana"));
	GetDescUpdate();
	SetButton();
	return TRUE;  
}

// Recupero puntatore al documento
CNoteSeriesDoc* DlgPdf::GetDoc()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;			// Puntatore alla mainframe
	CNoteSeriesView* pView = (CNoteSeriesView*)pMain->GetActiveView();	// Puntatore alla vista principale
	CNoteSeriesDoc* pDoc = pView->GetDocument();						// Puntatore al documento
	return pDoc;
}

// Descrizioni in lingua
void DlgPdf::GetDescUpdate()
{
	// Label
	m_lblNome.SetWindowTextW(m_currlang.GetDesc(38, m_temp));
	m_lblTitle.SetWindowTextW(m_currlang.GetDesc(185, m_temp));
	// Bottoni
	m_btnCrea.SetWindowTextW(m_currlang.GetDesc(186, m_temp));
	m_btnApriPdf.SetWindowTextW(m_currlang.GetDesc(187, m_temp));
	m_btnApriDir.SetWindowTextW(m_currlang.GetDesc(97, m_temp));
	m_btnCanc.SetWindowTextW(m_currlang.GetDesc(14, m_temp));
	m_btnElimina.SetWindowTextW(m_currlang.GetDesc(43, m_temp));
	m_btnRin.SetWindowTextW(m_currlang.GetDesc(86, m_temp));
}

// Aggiorno listbox coi pdf
void DlgPdf::GetList()
{
	m_lstPdf.ResetContent(); // Pulizia ListBox
	WIN32_FIND_DATA data;
	HANDLE cartella = FindFirstFile(retPath + L"\\PDF\\*", &data);
	CString nome = _T("");

	if (cartella == INVALID_HANDLE_VALUE)
		return;

	// Scandisco il contenuto della cartella e lo mostro nella ListBox
	while (FindNextFile(cartella, &data))
	{
		nome.Format(_T("%s"), data.cFileName);
		if (nome != _T(".") && nome != _T("..") && nome != _T("desktop.ini"))
			m_lstPdf.InsertString(NULL, nome);
	}
	FindClose(cartella);

	m_lstPdf.SetCurSel(0);
	GetChange();
}

// Modifico dimensioni dei pulsanti
void DlgPdf::SetButton()
{
	CMFCButton* btn[6] = { &m_btnCrea ,&m_btnApriPdf,&m_btnApriDir,&m_btnCanc,&m_btnElimina,&m_btnRin };
	for (int i = 0; i < 6; i++)
	{
		btn[i]->SetFont(m_pBtnFont);
		btn[i]->SetMouseCursor(LoadCursor(NULL, IDC_HAND));
	}
	btn[2]->SetTooltip(retPath + L"\\PDF");
}

// Genero PDF
void DlgPdf::SetPdf()
{
	CString npdf = _T("");
	m_txtNome.GetWindowTextW(npdf);
	if (npdf == L"") // Nome scelto dall'utente
		return;
	
	// Inizializzo interfaccia COM su thread corrente
	if (CoInitialize(NULL) == S_OK)
		return;

	IMyClientPtr client;
	client.CreateInstance(__uuidof(MyClient));
	
	// Salvo griglia corrente su variabili di questa classe
	for (int i = 0; i < pDoc->m_gridCount; i++)
	{
		m_nome = m_nome + pDoc->m_griglia[i].nome + L"*--*";
		m_cartella = m_cartella + pDoc->m_griglia[i].cartella + L"*--*";
		m_inizio = m_inizio + pDoc->m_griglia[i].inizio + L"*--*";
		m_fine = m_fine + pDoc->m_griglia[i].fine + L"*--*";
		m_stato = m_stato + pDoc->m_griglia[i].stato + L"*--*";
		m_priorità = m_priorità + pDoc->m_griglia[i].priorità + L"*--*";
		m_sito = m_sito + pDoc->m_griglia[i].sito + L"*--*";
		m_voto = m_voto + pDoc->m_griglia[i].voto + L"*--*";
		m_commento = m_commento + pDoc->m_griglia[i].commento + L"*--*";
	}
	// Invio dati alla dll del WebClient
	if (pDoc->m_tabella == _T("[anime]"))
	{
		if (!client->SendGridViste((_bstr_t)m_nome, (_bstr_t)m_cartella, (_bstr_t)m_inizio, (_bstr_t)m_fine, (_bstr_t)m_stato, 
			(_bstr_t)m_sito, (_bstr_t)m_voto, (_bstr_t)m_commento, (_bstr_t)retLang, (_bstr_t)npdf))
		{
			AfxMessageBox(m_currlang.GetDesc(188,m_temp));
			SvuotaVariabili();
			CoUninitialize();
			return;
		}
	}
	else
	{
		if (!client->SendGridFuture((_bstr_t)m_nome, (_bstr_t)m_stato, (_bstr_t)m_priorità, (_bstr_t)m_commento, (_bstr_t)retLang, (_bstr_t)npdf))
		{
			AfxMessageBox(m_currlang.GetDesc(188, m_temp));
			SvuotaVariabili();
			CoUninitialize();
			return;
		}
	}
	CString path,url = _T("");
	url.Format(_T("https://michelecaligiuri.altervista.org/noteseries/generaReportPDF/%s.pdf"),npdf.GetString());
	// Scarico file remoto e lo apro
	path.Format(_T("%s\\PDF\\%s.pdf"), retPath.GetString(),npdf.GetString());
	URLDownloadToFile(0, url, path, 0, 0);
	// Elimino file remoto
	if (!client->DeleteMyFile((_bstr_t)npdf + L".pdf"))
	{
		AfxMessageBox(m_currlang.GetDesc(188, m_temp));
		CoUninitialize();
		SvuotaVariabili();
		return;
	}
	ShellExecute(NULL, L"open", path, NULL, NULL, SW_SHOWMAXIMIZED);
	
	// Svuoto variabili dopo averle utilizzate
	SvuotaVariabili();
	
	AfxMessageBox(m_currlang.GetDesc(35,m_temp),MB_OK | MB_ICONINFORMATION);
	CoUninitialize();
	GetList();
}

void DlgPdf::SvuotaVariabili()
{
	m_nome = _T("");
	m_cartella = _T("");
	m_inizio = _T("");
	m_fine = _T("");
	m_stato = _T("");
	m_priorità = _T("");
	m_sito = _T("");
	m_voto = _T("");
	m_commento = _T("");
}



// Apro pdf selezionato in lista
void DlgPdf::ApriPdf()
{
	CString pdf = _T("");
	m_txtRen.GetWindowTextW(pdf);
	ShellExecute(NULL, L"open", retPath + L"\\PDF\\" + pdf, NULL, NULL, SW_SHOWMAXIMIZED);
}

// Rinomino pdf
void DlgPdf::RinominaPdf()
{
	CString newName,oldName = _T("");
	m_lstPdf.GetText(m_lstPdf.GetCurSel(), oldName);
	m_txtRen.GetWindowTextW(newName);

	// Per evitare di danneggiare il file
	if (newName.Find(L".pdf") == -1)
		newName = newName + L".pdf";

	if (newName == oldName)
		return;
	
	if (_wrename(retPath + L"\\PDF\\" + oldName, retPath + L"\\PDF\\" + newName) != 0)
	{
		CString error = _T("");
		error.Format((m_currlang.GetDesc(92, m_temp)), errno);
		AfxMessageBox(error);
		return;
	}
	GetList();
}

// Elimino pdf selezionato
void DlgPdf::EliminaPdf()
{
	CString pdf = _T("");
	m_txtRen.GetWindowTextW(pdf);
	if (AfxMessageBox(m_currlang.GetDesc(89, m_temp), MB_YESNO) == IDNO)
		return;

	if (!DeleteFile(retPath + L"\\PDF\\" + pdf))
	{
		AfxMessageBox((m_currlang.GetDesc(90, m_temp)));
		return;
	}
	GetList();
}


// Cambio selezione nella listbox
void DlgPdf::GetChange()
{
	int nsel = m_lstPdf.GetCurSel();
	CString buf = _T("");
	m_lstPdf.GetText(nsel, buf);
	m_txtRen.SetWindowTextW(buf);
}

// Apro cartella dei pdf
void DlgPdf::ApriCartellaPdf() { ShellExecute(NULL, L"open", retPath + L"\\PDF", NULL, NULL, SW_SHOWMAXIMIZED); }

// Chiudo la finestra
void DlgPdf::Annulla() { OnCancel(); }

// Doppio click su un pdf in lista, chiamo la funzione per aprirlo
void DlgPdf::OnDoubleClick() { ApriPdf(); }


HBRUSH DlgPdf::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		pDC->SetTextColor(RGB(255, 255, 255));
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	default:
		return hbr;
	}
}
