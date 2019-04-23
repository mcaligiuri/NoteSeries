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
}

void DlgPdf::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LBLTITLE, m_lblTitle);
	DDX_Control(pDX, IDC_BTNCREA, m_btnCrea);
	DDX_Control(pDX, IDC_BTNRES, m_btnCanc);
	DDX_Control(pDX, IDC_LBLNOME, m_lblNome);
	DDX_Control(pDX, IDC_TXTNOME, m_txtNome);
}


BEGIN_MESSAGE_MAP(DlgPdf, CDialogEx)
	ON_BN_CLICKED(IDC_BTNRES, &DlgPdf::Annulla)
	ON_BN_CLICKED(IDC_BTNCREA, &DlgPdf::SetPdf)
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

	CreateDirectoryEx(retPath, retPath + L"\\PDF", 0);
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

}

void DlgPdf::Annulla() { OnCancel();}


void DlgPdf::SetPdf()
{
	CString npdf = _T("");
	m_txtNome.GetWindowTextW(npdf);
	if (npdf == L"") // Nome scelto dall'utente
		return;
	npdf = retPath + L"\\PDF" + "\\" + npdf;
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
			AfxMessageBox(_T("error"));
			CoUninitialize();
			return;
		}
	}
	else
	{
		if (!client->SendGridFuture((_bstr_t)m_nome, (_bstr_t)m_stato, (_bstr_t)m_priorità, (_bstr_t)m_commento, (_bstr_t)retLang, (_bstr_t)npdf))
		{
			AfxMessageBox(_T("error"));
			CoUninitialize();
			return;
		}
	}
	// Apro file pdf creato
	CString path = _T("");
	path.Format(_T("%s.pdf"),npdf);
	ShellExecute(NULL, L"open",path , NULL, NULL, SW_SHOWMAXIMIZED);
	// Svuoto variabili dopo averle utilizzate
	m_nome = _T("");
	m_cartella = _T("");
	m_inizio = _T(""); 
	m_fine = _T(""); 
	m_stato = _T("");  
	m_priorità = _T(""); 
	m_sito = _T(""); 
	m_voto = _T(""); 
	m_commento = _T("");
	AfxMessageBox(_T("Operazione completata"));
	CoUninitialize();
}
