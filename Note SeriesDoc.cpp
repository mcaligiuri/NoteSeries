// Note SeriesDoc.cpp: implementazione della classe CNoteSeriesDoc
//

#include "stdafx.h"
// SHARED_HANDLERS può essere definito in un'anteprima di implementazione del progetto ATL, nei gestori di anteprime
// e della ricerca e consente di condividere il codice dei documenti con il progetto in questione.
#ifndef SHARED_HANDLERS
#include "Note Series.h"
#include "MainFrm.h"
#endif

#include "Note SeriesDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CNoteSeriesDoc

IMPLEMENT_DYNCREATE(CNoteSeriesDoc, CDocument)

BEGIN_MESSAGE_MAP(CNoteSeriesDoc, CDocument)
END_MESSAGE_MAP()


// Costruzione/distruzione di CNoteSeriesDoc

CNoteSeriesDoc::CNoteSeriesDoc()
{
	m_inizio = CTime::GetCurrentTime().Format("%d/%m/%Y");	// Data di inizio serie o data backup database
	m_backup = CTime::GetCurrentTime().Format("%d-%m-%Y");	// Data di backup database
	m_idv = _T("");											// ID serie future
	m_ida = _T("");											// ID serie viste
	m_nome = _T("");										// Nome della serie
	m_sito = _T("-");										// Sito web della serie
	m_cartella = _T("");									// Percorso cartella che vien creata su richiesta dell'utente
	m_fine = _T("-");										// Data di fine serie
	m_voto = _T("");										// Voto della serie
	m_nvoto = 0;											// Posizione combo voto
	m_stato = _T("");										// Stato : Finito, In corso ecc..
	m_nstato = 0;											// Posizione combo stato
	m_npriorita = 0;										// Posizione combo priorità
	m_com = _T("");											// Commento
	m_bmp = _T("");											// Percorso di eventuali loghi
	m_priorita = _T(""); 
	m_tabella = _T("");										// Contiene la tabella corrente del db "serie.mdb"
	m_nodo = _T("");
	m_sezNodo = _T("");
	m_versione = _T("1.1.2");								// Versione programma
	m_dbvers = _T("1.0.0.3");								// Versione database "serie.mdb"
	m_data = _T("");
	m_requery = _T("");										// Query di aggiornamento
	m_orderby = 0;
	m_cat = 1;												// Variabile per categoria
	m_gridCount = 0;										// Numero elementi griglia dei risultati
	m_griglia = new Griglia[m_gridCount];
	m_griglia->idl = 0;
	m_idl.Add(0);
	m_lbl = 0;
}

CNoteSeriesDoc::~CNoteSeriesDoc()
{
	delete[] m_griglia;
}

BOOL CNoteSeriesDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: aggiungere qui il codice di reinizializzazione
	// (nei documenti SDI verrà riutilizzato questo documento).

	return TRUE;
}

#ifdef SHARED_HANDLERS

// Supporto per anteprime
void CNoteSeriesDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modificare il codice per creare i dati del documento
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Supporto per gestori di ricerca
void CNoteSeriesDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Consente di impostare i contenuti di ricerca dai dati del documento.
	// Le parti di contenuto devono essere separata da ";"

	// Ad esempio:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CNoteSeriesDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Diagnostica di CNoteSeriesDoc

#ifdef _DEBUG
void CNoteSeriesDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNoteSeriesDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Comandi di CNoteSeriesDoc


void CNoteSeriesDoc::SetTitle(LPCTSTR lpszTitle)
{
	CDocument::SetTitle(L"Note Series");

	CDocument::SetTitle(lpszTitle);
}
