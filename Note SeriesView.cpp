// Note SeriesView.cpp: implementazione della classe CNoteSeriesView
//

#include "stdafx.h"
#ifndef SHARED_HANDLERS
#include "Note Series.h"
#include "atlimage.h"
#endif

#include "Note SeriesDoc.h"
#include "Note SeriesView.h"
#include "DlgCartella.h"
#include "DlgPrimoAvvio.h"
#include "CUpgrade.h"
#include "libxl.h"
#include <WinInet.h>

#include <fstream>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CNoteSeriesView

IMPLEMENT_DYNCREATE(CNoteSeriesView, CFormView)


BEGIN_MESSAGE_MAP(CNoteSeriesView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTNINIZIA, &CNoteSeriesView::IniziaSerie)
	ON_BN_CLICKED(IDC_BTNTERM, &CNoteSeriesView::TerminaSerie)
	ON_BN_CLICKED(IDC_BTNDEL, &CNoteSeriesView::EliminaSerie)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_OUTPUT, &CNoteSeriesView::SerieSelezionata)
	ON_COMMAND(ID_FILE_EXPORT_EXCEL, &CNoteSeriesView::OnExportExcel)
	ON_COMMAND(ID_FILE_EXPORT_EXCEL2, &CNoteSeriesView::OnExportExcel)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_RDOVISTE, &CNoteSeriesView::OnClickRdoViste)
	ON_BN_CLICKED(IDC_RDOFUTURE, &CNoteSeriesView::OnClickRdoFuture)
	ON_BN_CLICKED(IDC_BTNCAT, &CNoteSeriesView::OnClickCategorie)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CNoteSeriesView::OnTcnSelchangeTab)
	ON_BN_CLICKED(IDC_BTNMODDATE, &CNoteSeriesView::OnClickModificaDate)
	ON_COMMAND(ID_SALVA_COME, &CNoteSeriesView::SalvaProgetto)
	ON_COMMAND(ID_SALVA_COME2, &CNoteSeriesView::SalvaProgetto)
	ON_COMMAND(ID_PR_OPEN, &CNoteSeriesView::OnFileOpen)
	ON_COMMAND(ID_PR_OPEN2, &CNoteSeriesView::OnFileOpen)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_OUTPUT, &CNoteSeriesView::OnClickColonna)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_OUTPUT, &CNoteSeriesView::EvidenziaPerPriorita)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCOM, &CNoteSeriesView::CambiaMenu)
	ON_BN_CLICKED(IDC_LBLCAT, &CNoteSeriesView::OnClickCategorie)
END_MESSAGE_MAP()

// Costruzione/distruzione di CNoteSeriesView

CNoteSeriesView::CNoteSeriesView()
	: CFormView(IDD_NOTESERIES_FORM)
{
	temp = _T("");		 // Usato come ritorno per la GetDesc
	relPath = _T("");	 // Percorso relativo loghi
	m_sql = _T("");		 // Query
	m_orderNome = FALSE; // Usata per l'ordinamento colonna ASC e DESC
	priorità = NESSUNA;
	CAggMod dlg0 = new CAggMod();
	DlgDate dlg2 = new DlgDate();
	DlgCategorie dlg1 = new DlgCategorie();
	m_mostraDlg = nullptr;
}

CNoteSeriesView::~CNoteSeriesView()
{
	delete dlg0;
	delete dlg2;
	delete dlg1;
}

void CNoteSeriesView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	CNoteSeriesDoc* pDoc = GetDocument();

	// Controlli combo e caselle di testo
	DDX_Control(pDX, IDC_LBLCAT, m_lblCat);
	DDX_Control(pDX, IDC_OUTPUT, m_output);
	// Bottoni, radio button
	DDX_Control(pDX, IDC_BTNINIZIA, m_btnInizia);
	DDX_Control(pDX, IDC_BTNTERM, m_btnTerm);
	DDX_Control(pDX, IDC_BTNDEL, m_btnDel);
	DDX_Control(pDX, IDC_RDOVISTE, m_rdoViste);
	DDX_Control(pDX, IDC_RDOFUTURE, m_rdoFuture);
	DDX_Control(pDX, IDC_TAB, m_TabCtrl);
	DDX_Control(pDX, IDC_TXTIDCAT, m_txtIdCat);
	DDX_Control(pDX, IDC_TABCOM, m_tabCom);
}

BOOL CNoteSeriesView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: modificare la classe o gli stili Window modificando
	//  la struttura CREATESTRUCT

	return CFormView::PreCreateWindow(cs);
}

void CNoteSeriesView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();

	// Controllo se è la prima volta che il programma viene avviato
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings")))
	{
		DWORD reg = 0;
		chiave.QueryDWORDValue(_T("Register"), reg);
		if (reg != 1)
		{
			DlgPrimoAvvio config = new DlgPrimoAvvio();
			config.DoModal();
			chiave.SetDWORDValue(L"Register", 1);
			delete config;
		}
	}
	CNoteSeriesDoc* pDoc = GetDocument(); // Documento
	
	GetDescUpdate();			// Descrizioni
	//ResizeParentToFit();
	// Dimensioni caratteri output
	CFont *m_pFont = new CFont();
	m_pFont->CreatePointFont(100, _T("Verdana"));
	m_output.SetFont(m_pFont, TRUE);
	m_TabCtrl.SetFont(m_pFont, TRUE);

	// Font per il pulsante delle categorie
	CFont *m_pLabel = new CFont();
	m_pLabel->CreatePointFont(8, _T("Verdana"));
	m_lblCat.SetFont(m_pLabel, TRUE);
	
	// Celle nella griglia dei risultati e selezione full riga
	m_output.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES | LVS_REPORT | LVS_OWNERDATA);
	// Imposto le colonne griglia dei risultati
	int titoli[9] = { 38,11,22,23,24,37,25,26,27 }; // IDDESCR nel database

	while (m_output.DeleteColumn(0)); // Cancello colonne

	// Metto nuove colonne e nascondo quelle superflue
	for (short i = 0; i <=8; i++) {
		if (i == 0)
			m_output.InsertColumn(i, m_currlang.GetDesc(titoli[i]-1, temp) + L" ▲▼", LVCFMT_CENTER, 120);
		
		else 
			m_output.InsertColumn(i, m_currlang.GetDesc(titoli[i]-1, temp), LVCFMT_CENTER | LVS_EX_HEADERDRAGDROP | HDS_BUTTONS, 120);
	}
	NascondiColonne(pDoc->m_tabella);
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	dbconfig.GetCategoria(&m_TabCtrl, tcItem);
	// Controllo la presenza di aggiornamenti
	if (ControllaAggiornamenti())
	{
		CUpgrade upd = new CUpgrade();
		upd.DoModal();
		delete upd;
	}
	// Setto icone sotto la griglia
	SetIcons();

	// Menù a tab
	m_tabCom.InsertItem(0, _T("FORM"));
	m_tabCom.InsertItem(2, _T("DATE"));
	m_tabCom.InsertItem(3, pDoc->m_label.GetAt(0).MakeUpper());

	CRect ret;
	m_tabCom.GetClientRect(&ret);

	// Dialog per modificare o aggiungere
	dlg0.Create(IDD_AGGIUNGI_MODIFICA, &m_tabCom);
	dlg0.SetWindowPos(NULL, 5, 25, ret.Width() - 10, ret.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
	m_mostraDlg = &dlg0;

	// Dialog per modificare le date
	dlg2.Create(IDD_MODIFICA_DATE, &m_tabCom);
	dlg2.SetWindowPos(NULL, 5, 25, ret.Width() - 10, ret.Height() - 30, SWP_HIDEWINDOW | SWP_NOZORDER);
}

void CNoteSeriesView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CNoteSeriesView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Diagnostica di CNoteSeriesView

#ifdef _DEBUG
void CNoteSeriesView::AssertValid() const
{
	CFormView::AssertValid();
}

void CNoteSeriesView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNoteSeriesDoc* CNoteSeriesView::GetDocument() const // la versione non debug è inline.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNoteSeriesDoc)));
	return (CNoteSeriesDoc*)m_pDocument;
}
#endif //_DEBUG


// Gestori di messaggi di CNoteSeriesView


// Descrizioni della vista
void CNoteSeriesView::GetDescUpdate()
{	
	// Popolo la combo dello stato e verifico cosa ho selezionato
	CNoteSeriesDoc* pDoc = GetDocument(); // Puntatore al documento

	// Bottoni
	m_btnInizia.SetWindowTextW(m_currlang.GetDesc(39, temp));			// Bottone inizia
    m_btnTerm.SetWindowTextW(m_currlang.GetDesc(41, temp));				// Bottone termina
    m_btnDel.SetWindowTextW(m_currlang.GetDesc(42, temp));				// Bottone elimina
	m_rdoViste.SetWindowTextW(m_currlang.GetDesc(68, temp));			// Radio, sezione viste
	m_rdoFuture.SetWindowTextW(m_currlang.GetDesc(67, temp));			// Radio, sezione future
	m_lblCat.SetWindowTextW(m_currlang.GetDesc(107, temp));				// Label categorie
}

// Imposto icone dei pulsanti
void CNoteSeriesView::SetIcons()
{
	// Font dei pulsanti
	CFont *m_pFont = new CFont();
	m_pFont->CreatePointFont(110, _T("Verdana"));

	// Pulsante elimina
	HICON del = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_DEL), IMAGE_ICON, 20, 20, LR_DEFAULTCOLOR);
	m_btnDel.SetIcon(del);
	m_btnDel.SetTooltip(m_currlang.GetDesc(171,temp));
	m_btnDel.SetTextColor(RGB(255, 0, 0));
	m_btnDel.SetFont(m_pFont);
	m_btnDel.SetMouseCursor(LoadCursor(NULL, IDC_HAND));

	// Pulsante inizia
	HICON ini = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_INIZIA), IMAGE_ICON, 20, 20, LR_DEFAULTCOLOR);
	m_btnInizia.SetIcon(ini);
	m_btnInizia.SetTooltip(m_currlang.GetDesc(172, temp));
	m_btnInizia.SetTextColor(RGB(63, 181, 122));
	m_btnInizia.SetFont(m_pFont);
	m_btnInizia.SetMouseCursor(LoadCursor(NULL, IDC_HAND));

	// Pulsante termina
	HICON fin = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_TERMINA), IMAGE_ICON, 20, 20, LR_DEFAULTCOLOR);
	m_btnTerm.SetIcon(fin);
	m_btnTerm.SetTooltip(m_currlang.GetDesc(173, temp));
	m_btnTerm.SetTextColor(RGB(0, 0, 255));
	m_btnTerm.SetFont(m_pFont);
	m_btnTerm.SetMouseCursor(LoadCursor(NULL,IDC_HAND));
}

void CNoteSeriesView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// Al variare delle variabili del documento aggiorno i controlli della vista
	
	CNoteSeriesDoc* pDoc = GetDocument(); // Puntatore al documento
	CListCtrl* pList = (CListCtrl *)GetDlgItem(IDC_OUTPUT); // Puntatore a griglia

	// Al variare del click sul menù laterale aggiorno la griglia e la form in basso
	if (pDoc->m_tabella == _T("") && pDoc->m_nodo != pDoc->m_artitoli.GetAt(0))
		return;

	// Se ho selezionato una lettera nel menù laterale estraggo le serie che iniziano per quella lettera
	if (pDoc->m_nodo.GetLength() == 1)
	{
		CString tmp = pDoc->m_nodo += '%';
		if (pDoc->m_lbl != 0)
			m_sql.Format(_T("SELECT * FROM %s WHERE(nome LIKE '%s' AND IDCAT=%d AND IDL=%d) ORDER BY nome DESC"), pDoc->m_tabella.GetString(), tmp.GetString(),pDoc->m_cat,pDoc->m_lbl);
		else
			m_sql.Format(_T("SELECT * FROM %s WHERE(nome LIKE '%s' AND IDCAT=%d) ORDER BY nome DESC"), pDoc->m_tabella.GetString(), tmp.GetString(), pDoc->m_cat);

		pDoc->m_requery = m_sql;
		dbconfig.UpdateSerieSel(m_sql, pList);
	}
	else if (pDoc->m_nodo == pDoc->m_artitoli.GetAt(0)) // Label "SERIE" nell'albero selezionata
	{
		CString idcat,CurrTab = _T("");
		short nsel = m_TabCtrl.GetCurSel();
		TCITEM tcItem;
		// Ricavo il testo del tab selezionato
		tcItem.mask = TCIF_TEXT;
		tcItem.cchTextMax = 256;
		tcItem.pszText = CurrTab.GetBuffer(tcItem.cchTextMax);

		if (!(m_TabCtrl.GetItem(nsel, &tcItem)))
			return;

		CurrTab.ReleaseBuffer();
		dbconfig.GetIDCategoria(&CurrTab, &m_txtIdCat);
		m_txtIdCat.GetWindowTextW(idcat);
		pDoc->m_cat = _wtoi(idcat);
		pDoc->m_tabella = _T("[anime]");
		if (pDoc->m_lbl == 0)
			m_sql.Format(_T("SELECT nome,cartella,inizio,fine,stato,commenti,voto,sito, IDL FROM %s WHERE IDCAT=%d ORDER BY nome DESC"), pDoc->m_tabella.GetString(), pDoc->m_cat);
		else
			m_sql.Format(_T("SELECT nome,cartella,inizio,fine,stato,commenti,voto,sito, IDL FROM %s WHERE IDCAT=%d AND IDL=%d ORDER BY nome DESC"), pDoc->m_tabella.GetString(), pDoc->m_cat, pDoc->m_lbl);
		pDoc->m_requery = m_sql;
		NascondiColonne(pDoc->m_tabella);
		dbconfig.UpdateSerieSel(m_sql, pList);
		return;
	}
	// Se ho selezionato 0-9 nell'albero
	// significa che voglio vedere le serie che iniziano per numero
	else if (pDoc->m_nodo == _T("0-9"))
	{
		CString interv = _T("[0-9]"); // Intervallo
		interv = interv += '%';
		if (pDoc->m_lbl == 0)
			m_sql.Format(_T("SELECT * FROM %s WHERE(nome LIKE '%s' AND IDCAT=%d)"), pDoc->m_tabella.GetString(), interv.GetString(), pDoc->m_cat);
		else
			m_sql.Format(_T("SELECT * FROM %s WHERE(nome LIKE '%s' AND IDCAT=%d AND IDL=%d)"), pDoc->m_tabella.GetString(), interv.GetString(), pDoc->m_cat,pDoc->m_lbl);

		pDoc->m_requery = m_sql;
		dbconfig.UpdateSerieSel(m_sql, pList);
	}
	else if (pDoc->m_nodo == pDoc->m_artitoli.GetAt(1)) // Ordine Alfabetico
	{
		m_sql.Format(_T("SELECT * FROM %s WHERE IDCAT=%d ORDER BY nome DESC"), pDoc->m_tabella.GetString(), pDoc->m_cat);
		pDoc->m_requery = m_sql;
		dbconfig.UpdateSerieSel(m_sql, pList);
	}
	else if (pDoc->m_nodo == pDoc->m_stati.GetAt(0) || pDoc->m_nodo == pDoc->m_stati.GetAt(2))
	{
		short n = 0; // posizione combo

		// stato "Finito"
		if (pDoc->m_nodo == pDoc->m_stati.GetAt(0)) n = 0;

		// stato "In corso"
		else  n = 2;

		if (pDoc->m_lbl == 0)
			m_sql.Format(_T("SELECT * FROM %s WHERE(stato = %d AND IDCAT=%d) ORDER BY nome DESC"), pDoc->m_tabella.GetString(), n, pDoc->m_cat);
		else
			m_sql.Format(_T("SELECT * FROM %s WHERE(stato = %d AND IDCAT=%d AND IDL=%d) ORDER BY nome DESC"), pDoc->m_tabella.GetString(), n, pDoc->m_cat,pDoc->m_lbl);

		pDoc->m_requery = m_sql;
		dbconfig.UpdateSerieSel(m_sql, pList);
	}
	// sottodivisioni della sezione "Future"
	else if(pDoc->m_sezNodo == pDoc->m_artitoli.GetAt(2) || pDoc->m_sezNodo == pDoc->m_artitoli.GetAt(5))
	{
		short n = 0; // Valore corrispondente allo stato o alla priorità
		if (pDoc->m_nodo == pDoc->m_stati.GetAt(1) || pDoc->m_nodo == pDoc->m_stati.GetAt(4) || pDoc->m_nodo == pDoc->m_stati.GetAt(3))
		{
			pDoc->m_tabella = _T("[Da vedere]");
			
			// stato "in attesa"
			if(pDoc->m_nodo == pDoc->m_stati.GetAt(1)) n = 1;

			// stato "concluso"
			else if (pDoc->m_nodo == pDoc->m_stati.GetAt(4)) n = 4;
			
			// stato "in uscita"
			else  n = 3;
			
			if (pDoc->m_lbl == 0)
				m_sql.Format(_T("SELECT * FROM %s WHERE(stato = %d AND IDCAT=%d) ORDER BY nome DESC"), pDoc->m_tabella.GetString(), n, pDoc->m_cat);
			else
				m_sql.Format(_T("SELECT * FROM %s WHERE(stato = %d AND IDCAT=%d AND IDL=%d) ORDER BY nome DESC"), pDoc->m_tabella.GetString(), n, pDoc->m_cat, pDoc->m_lbl);

			pDoc->m_requery = m_sql;
		}
		else
		{
			pDoc->m_tabella = _T("[Da vedere]");

			// Priorità "Nessuna"
			if (pDoc->m_nodo == pDoc->m_prior.GetAt(0)) n = 0;

			// Priorità "Alta"
			else if (pDoc->m_nodo == pDoc->m_prior.GetAt(1)) n = 1;

			// Priorità "Media"
			else if (pDoc->m_nodo == pDoc->m_prior.GetAt(2)) n = 2;

			// Priorità "Bassa"
			else  n = 3;
			if (pDoc->m_lbl == 0)
				m_sql.Format(_T("SELECT * FROM %s WHERE(priorità = %d AND IDCAT=%d) ORDER BY nome DESC"), pDoc->m_tabella.GetString(), n, pDoc->m_cat);
			else
				m_sql.Format(_T("SELECT * FROM %s WHERE(priorità = %d AND IDCAT=%d AND IDL=%d) ORDER BY nome DESC"), pDoc->m_tabella.GetString(), n, pDoc->m_cat, pDoc->m_lbl);
			
			pDoc->m_requery = m_sql;
		}
		pDoc->m_requery = m_sql;
		dbconfig.UpdateSerieSel(m_sql, pList);
	}
	// Se non sono in nessuno dei casi precedenti allora ho selezionato un mese nell'albero
	else
	{
		CString anno = _T("");
		CString mese = _T("");

		// Scorro array dei mesi
		for (short i = 0; i < pDoc->m_armesi.GetSize(); i++)
		{
			// Controllo se la variabile del documento è uguale a uno
			// degli elementi dell array dei mesi
			if (pDoc->m_nodo == pDoc->m_armesi.GetAt(i))
			{
				// Mese in formato numerico
				if(i < 9)
					mese.Format(_T("0%d"), i+1);
				else
					mese.Format(_T("%d"), i + 1);
				// Assegno anno memorizzato nel documento
				anno = pDoc->m_sezNodo; 

				// Reimposto tabella corrente
				//pDoc->m_tabella = _T("[anime]"); 

				// Seleziono dal db le serie di quell'anno e in quel mese pulendo prima la griglia
				// Controllo se l'utente vuole un ordinamento per data di inizio o fine
				if (pDoc->m_data == pDoc->m_artitoli.GetAt(7))
				{
					if (pDoc->m_lbl == 0)
						m_sql.Format(_T("SELECT * FROM %s WHERE(inizio LIKE '%s' AND inizio LIKE '%s' AND IDCAT=%d) ORDER BY nome ASC"), pDoc->m_tabella.GetString(), '%' + anno, '%' + mese + '%', pDoc->m_cat);
					else
						m_sql.Format(_T("SELECT * FROM %s WHERE(inizio LIKE '%s' AND inizio LIKE '%s' AND IDCAT=%d AND IDL=%d) ORDER BY nome ASC"), pDoc->m_tabella.GetString(), '%' + anno, '%' + mese + '%', pDoc->m_cat, pDoc->m_lbl);
				}					
				else
				{
					if (pDoc->m_lbl == 0)
						m_sql.Format(_T("SELECT * FROM %s WHERE(inizio LIKE '%s' AND inizio LIKE '%s' AND fine<>'-' AND IDCAT=%d) ORDER BY nome ASC"), pDoc->m_tabella.GetString(), '%' + anno, '%' + mese + '%', pDoc->m_cat);
					else
						m_sql.Format(_T("SELECT * FROM %s WHERE(inizio LIKE '%s' AND inizio LIKE '%s' AND fine<>'-' AND IDCAT=%d AND IDL=%d) ORDER BY nome ASC"), pDoc->m_tabella.GetString(), '%' + anno, '%' + mese + '%', pDoc->m_cat, pDoc->m_lbl);
				}				
				pDoc->m_requery = m_sql;
				dbconfig.UpdateSerieSel(m_sql,pList);
			}
		}
	}
	UpdateData(TRUE);
	// Se la tabella del db "serie.mdb" è questa, allora abilito/disabilto questi controlli
	if (pDoc->m_tabella == _T("[anime]"))
	{
		m_btnInizia.EnableWindow(0);
		m_btnTerm.EnableWindow(1);
		m_btnDel.EnableWindow(1);
		m_rdoViste.SetCheck(TRUE);
		m_rdoFuture.SetCheck(FALSE);
		NascondiColonne(pDoc->m_tabella);
	}
	else // Se la tabella è l'altra allora uso questa configurazione
	{
		m_btnInizia.EnableWindow(1);
		m_btnTerm.EnableWindow(0);
		m_btnDel.EnableWindow(1);
		m_rdoViste.SetCheck(FALSE);
		m_rdoFuture.SetCheck(TRUE);
		NascondiColonne(pDoc->m_tabella);
	}
	
	// Se non è checcata disabilito l'opzione per la cartella
	if (!(dlg0.m_dlgChkCartella.GetCheck()))
			dlg0.m_dlgcartella.EnableWindow(0);
	else
		dlg0.m_dlgcartella.EnableWindow(1);
}

// Riduco a 0 la lunghezza delle colonne non necessarie
// a seconda della tabella del database
void CNoteSeriesView::NascondiColonne(CString tabella)
{
	if (tabella == _T("[anime]"))
	{
		short viste[9] = { 280,120,120,120,120,120,0,120,120 };
		
		m_rdoViste.SetCheck(TRUE);
		m_rdoFuture.SetCheck(FALSE);
		
		for (short i = 0; i < 9; i++)
			m_output.SetColumnWidth(i, viste[i]);
	}
	else
	{
		short futur[9] = { 280,0,0,0,0,0,120,120,120 };
		
		m_rdoViste.SetCheck(FALSE);
		m_rdoFuture.SetCheck(TRUE);

		for (short i = 0; i < 9; i++)
			m_output.SetColumnWidth(i, futur[i]);
	}
}

CNoteSeriesView * CNoteSeriesView::GetView()
{
	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);

	CView * pView = pFrame->GetActiveView();

	if (!pView)
		return NULL;

	// Fail if view is of wrong kind
	// (this could occur with splitter windows, or additional
	// views on a single document

	if (!pView->IsKindOf(RUNTIME_CLASS(CNoteSeriesView)))
		return NULL;

	return (CNoteSeriesView *)pView;
}

void CNoteSeriesView::IniziaSerie()
{
	CNoteSeriesDoc* pDoc = GetDocument();

	// Il nome ci deve essere per forza
	dlg0.m_frmnome.GetWindowText(pDoc->m_nome);
	if (pDoc->m_nome == _T("") || m_output.GetSelectedCount() == 0)
		return;

	else if (!(AfxMessageBox(m_currlang.GetDesc(65, temp), MB_YESNO) == IDYES))
		return;

	short sel = 2;	// La posizione della combo stato
	short voto = 0; // La posizione della combo voto

	// Aggiorno variabili documento col testo dei controlli
	dlg0.m_dlgcom.GetWindowText(pDoc->m_com);
	pDoc->m_sito = _T("-");
	dlg0.m_dlgsimbolo.GetWindowText(pDoc->m_bmp);

	if (pDoc->m_bmp != _T(""))
		GetLogo();

	// Controllo apici singoli per SQL
	if (pDoc->m_nome.Find(L"'") != -1 || pDoc->m_com.Find(L"'") != -1)
	{
		pDoc->m_nome.Replace(L"'", L"''");
		pDoc->m_com.Replace(L"'", L"''");
	}

	// OMONIMOOOOO!!!! (CIT BAFFO)
	int count = dbconfig.ContaOmonimi(pDoc->m_nome, L"[anime]", pDoc->m_cat);
	if (count > 0)
	{
		CString val = _T("");
		val.Format(_T(" (%d)"), count + 1);
		pDoc->m_nome = pDoc->m_nome + val;
	}
	pDoc->m_inizio = CTime::GetCurrentTime().Format("%d/%m/%Y");
	pDoc->m_fine = _T("-");
	// Costruisco ed eseguo query di inserimento
	m_sql.Format
	(L"INSERT INTO [anime] (nome, inizio, fine, stato, commenti, sito, bitmap, voto, IDCAT ) VALUES('%s','%s','%s', %d,'%s','%s','%s',%d,%d)",
		pDoc->m_nome.GetString(), pDoc->m_inizio.GetString(), pDoc->m_fine.GetString(), sel, pDoc->m_com.GetString(), pDoc->m_sito.GetString(), pDoc->m_bmp.GetString(), voto, pDoc->m_cat);

	if (!(dbconfig.SetSerie(m_sql)))
		return;

	// Costruisco ed eseguo query di cancellazione
	m_sql.Format(L"DELETE FROM %s WHERE idv=%d", pDoc->m_tabella.GetString(), _wtoi(pDoc->m_idv));

	if (!(dbconfig.SetSerie(m_sql)))
		return;

	// Requery
	pDoc->m_tabella = _T("[anime]");
	m_sql.Format(L"SELECT * FROM %s WHERE IDCAT=%d ORDER BY nome DESC", pDoc->m_tabella.GetString(), pDoc->m_cat);

	OnBnClickedBtnreset();
	CListCtrl *pList = &m_output;
	dbconfig.UpdateSerieSel(m_sql, pList);
	NascondiColonne(pDoc->m_tabella);

	// Abilito controlli
	//m_btnSalva.EnableWindow(1);
	m_btnDel.EnableWindow(1);
	m_btnTerm.EnableWindow(1);
	dlg0.m_dlgChkCartella.EnableWindow(1);
	dlg0.m_dlgcartella.EnableWindow(1);
	dlg0.m_dlgsito.EnableWindow(1);
	dlg0.m_dlgsimbolo.EnableWindow(1);
	dlg0.m_dlgvoto.EnableWindow(1);
	dlg0.m_dlgpriorita.EnableWindow(0);
	m_rdoFuture.SetCheck(FALSE);
	m_rdoViste.SetCheck(TRUE);
}

// Caricamento logo
void CNoteSeriesView::GetLogo()
{
	CNoteSeriesDoc* pDoc = GetDocument(); // Puntatore al documento

	CString path = _T(""); // Contiene il percorso assoluto del logo
	dlg0.m_dlgsimbolo.GetWindowText(pDoc->m_bmp);

	CString nomeFoto = pDoc->m_bmp.Mid(pDoc->m_bmp.ReverseFind('\\') + 1);

	// Leggo chiave di registro per i loghi (dove vengono salvati)
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
	{
		len = 255;
		chiave.QueryStringValue(_T("Loghi"), relPath.GetBufferSetLength(len), &len);
	}

	// Costruisco il percorso del logo: chiave + nomelogo
	path.Format(_T("%s\\%s"), relPath.GetString(), nomeFoto.GetString());

	// Controllo che nel percorso non sia presente un logo con lo stesso nome
	CFileFind file;
	if (file.FindFile(path))
	{
		AfxMessageBox(m_currlang.GetDesc(62,temp));
		return;
	}

	// Copio il logo scelto dall'utente nel percorso costruito
	if (!(CopyFile(pDoc->m_bmp, path, 0)))
	{
		AfxMessageBox(m_currlang.GetDesc(63,temp));
		return;
	}

	// Assegno il percorso costruito alla variabile del documento
	pDoc->m_bmp = path;
}

// Termino una serie aggiungendo la data di fine
void CNoteSeriesView::TerminaSerie()
{
	CNoteSeriesDoc* pDoc = GetDocument(); // Puntatore al documento

	// Aggiorno variabile del documento
	dlg0.m_frmnome.GetWindowText(pDoc->m_nome);			// nome
	if (pDoc->m_nome == _T("") || dlg0.m_dlgstato.GetCurSel() == 0 || m_output.GetSelectedCount() == 0)
	{
		dlg0.m_dlgLabel.SetWindowTextW(m_currlang.GetDesc(154,temp));
		return;
	}
	CString msg,des = _T("");
	//des = m_currlang.GetDesc(154, temp);
	msg.Format(_T("%s %s"), m_currlang.GetDesc(153, temp).GetString(), pDoc->m_nome.GetString());
	if (!(AfxMessageBox(msg, MB_YESNO) == IDYES))
		return;

	CListCtrl *pList = NULL; // Puntatore a grigla
	pList = &m_output;

	// Aggiorno la data di fine
	pDoc->m_fine = CTime::GetCurrentTime().Format("%d/%m/%Y");

	// Combo stato
	short stato = dlg0.m_dlgstato.SetCurSel(0);

	// Controllo apici singoli per SQL
	if (pDoc->m_nome.Find(L"'") != -1 || pDoc->m_com.Find(L"'") != -1)
	{
		pDoc->m_nome.Replace(L"'", L"''");
		pDoc->m_com.Replace(L"'", L"''");
	}

	// Costruisco la query e la eseguo
	m_sql.Format(L"UPDATE [anime] SET fine='%s',stato=%d WHERE nome='%s'",pDoc->m_fine.GetString(), stato, pDoc->m_nome.GetString());
	
	if (!(dbconfig.SetSerie(m_sql)))
		return;
	
	// Mostro il risultato con requery
	dbconfig.UpdateSerieSel(pDoc->m_requery, pList);
}

// Elimino una serie
void CNoteSeriesView::EliminaSerie()
{
	CNoteSeriesDoc* pDoc = GetDocument(); // Puntatore al documento

	// Aggiorno variabile del documento
	dlg0.m_frmnome.GetWindowText(pDoc->m_nome); // nome
	if (pDoc->m_nome == _T("") || m_output.GetSelectedCount() == 0)
		return;

	if (!(AfxMessageBox(m_currlang.GetDesc(65,temp), MB_YESNO) == IDYES))
		return;

	// Controllo apici singoli per SQL
	if (pDoc->m_nome.Find(L"'") != -1 || pDoc->m_com.Find(L"'") != -1)
	{
		pDoc->m_nome.Replace(L"'", L"''");
		pDoc->m_com.Replace(L"'", L"''");
	}
	
	m_sql.Format(L"DELETE FROM %s WHERE nome ='%s' AND IDCAT=%d",pDoc->m_tabella.GetString(), pDoc->m_nome.GetString(), pDoc->m_cat);
	
	if ((!dbconfig.SetSerie(m_sql)))
		return;

	// Elimino eventuali loghi
	DeleteFile(pDoc->m_bmp);
	OnBnClickedBtnreset(); // Reset controlli
	
	// Mostro il risultato con requery
	CListCtrl *pList = NULL; // Puntatore a griglia
	/*CString sql = _T("");
	sql.Format(L"SELECT * FROM %s WHERE IDCAT=%d", pDoc->m_tabella, pDoc->m_cat);*/
	pList = &m_output;
	dbconfig.UpdateSerieSel(pDoc->m_requery, pList);
	pDoc->UpdateAllViews(this);
}


// Imposto colore sfondo label bianco
HBRUSH CNoteSeriesView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	CBrush brush;

	// Sfondo 
	pDC->SetBkColor(RGB(255, 255, 255));
	if ((HBRUSH)brush == NULL)
		brush.CreateSolidBrush(RGB(255, 255, 255));
	
	// Restituisce oggetto CBrush
	return (HBRUSH)brush;
}

// Copro lo sfondo della form con un rect colore bianco
BOOL CNoteSeriesView::OnEraseBkgnd(CDC* pDC)
{
	CRect r;
	GetClientRect(&r);
	CBrush brush;

	if ((HBRUSH)brush == NULL)
		brush.CreateSolidBrush(RGB(255, 255, 255));
	pDC->FillRect(&r, &brush);

	return TRUE;
}

// Reset controlli
void CNoteSeriesView::OnBnClickedBtnreset()
{
	CNoteSeriesDoc* pDoc = GetDocument(); // Documento

	// Pulizia griglia e caselle di testo
	//m_output.DeleteAllItems();
	dlg0.m_frmnome.SetWindowTextW(_T(""));
	dlg0.m_dlgcom.SetWindowTextW(_T(""));
	dlg0.m_dlgsimbolo.SetWindowTextW(_T(""));
	dlg0.m_dlgsito.SetWindowTextW(_T(""));
	dlg0.m_dlgcartella.SetWindowTextW(_T(""));
	
	// Combo in posizione 0
	dlg0.m_dlgstato.SetCurSel(0);
	dlg0.m_dlgvoto.SetCurSel(0);

	// Nessun logo mostrato
	/*CStatic *pctrl = (CStatic *)GetDlgItem(IDC_ICONSERIES);
	pctrl->SetBitmap(NULL);
	pctrl->Invalidate();*/
	
	// Label informativa
	dlg0.m_dlgLabel.SetWindowTextW(m_currlang.GetDesc(4, temp));
	dlg0.m_dlgLabel.EnableWindow(1);
	
	// Pulsanti
	m_btnInizia.EnableWindow(0);
	m_btnTerm.EnableWindow(0);
	m_btnDel.EnableWindow(0);

	if (pDoc->m_tabella == _T("[anime]") && m_rdoViste.GetCheck())
		dlg0.m_dlgstato.SetCurSel(0);
	else
		dlg0.m_dlgstato.SetCurSel(1);
}

void CNoteSeriesView::SerieSelezionata(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CNoteSeriesDoc* pDoc = GetDocument(); // Documento

	// colonna di riferimento
	// uso quella del nome
	short nCol = 0;

	// posizione del primo elemento selezionato
	POSITION pos = m_output.GetFirstSelectedItemPosition();

	// numero elemento selezionato
	short nelem = m_output.GetNextSelectedItem(pos);

	// conterrà l'lelemento selezionato (nome)
	CString txt = _T("");

	// Assegno il testo dell'elemento selezionato
	txt = m_output.GetItemText(nelem, nCol);
	
	// Aggiorno casella di testo e varibile del documento
	//m_frmnome.SetWindowTextW(txt);
	pDoc->m_nome = txt;
	
	if (pDoc->m_nome == _T(""))
		return;
	
	// Controllo apici singoli per SQL
	if (pDoc->m_nome.Find(L"'") != -1 || pDoc->m_com.Find(L"'") != -1)
	{
		pDoc->m_nome.Replace(L"'", L"''");
		pDoc->m_com.Replace(L"'", L"''");
	}

	// Aggiorno le altre variabili del documento
	if (pDoc->m_tabella == _T("[anime]")) {
		m_sql.Format(_T("SELECT ida, nome,inizio,fine,sito,commenti,stato,voto,bitmap,cartella FROM %s WHERE nome='%s'"), pDoc->m_tabella.GetString(), pDoc->m_nome.GetString());
		dbconfig.GetSerieCorrente(m_sql);
		m_rdoFuture.SetCheck(FALSE);
		m_rdoViste.SetCheck(TRUE);
		m_btnInizia.EnableWindow(0);
		m_btnTerm.EnableWindow(1);
		m_btnDel.EnableWindow(1);
	}
	// Se la tabella è l'altra allora uso questa configurazione
	else {
		m_sql.Format(_T("SELECT idv,nome,commenti,priorità,stato FROM %s WHERE nome='%s'"), pDoc->m_tabella.GetString(), pDoc->m_nome.GetString());
		dbconfig.GetSerieCorrente(m_sql);
		m_rdoFuture.SetCheck(TRUE);
		m_rdoViste.SetCheck(FALSE);
		m_btnInizia.EnableWindow(1);
		m_btnTerm.EnableWindow(0);
		m_btnDel.EnableWindow(1);
	}
	
	// Estraggo percorso relativo loghi
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
	{
		len = 255;
		chiave.QueryStringValue(_T("Loghi"), relPath.GetBufferSetLength(len), &len);
		relPath.ReleaseBuffer();
	}
	
	if (pDoc->m_nome != _T(""))
	{
		int nsel = m_tabCom.GetCurSel();
		CRect ret;
		m_tabCom.GetClientRect(&ret);
		switch (nsel)
		{
		case 0:
			dlg0.DestroyWindow();
			dlg0.Create(IDD_AGGIUNGI_MODIFICA, &m_tabCom);
			dlg0.SetWindowPos(NULL, 5, 25, ret.Width() - 10, ret.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
			break;
		case 1:
			dlg2.DestroyWindow();
			dlg2.Create(IDD_MODIFICA_DATE, &m_tabCom);
			dlg2.SetWindowPos(NULL, 5, 25, ret.Width() - 10, ret.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
			break;
		}
	}
	dlg0.SerieSelezionata(pNMHDR, pResult);
	*pResult = 0;
}

// Esportazione dati in Excel della serie selezionata in griglia
void CNoteSeriesView::OnExportExcel()
{
	// Se in griglia non è selezionato nulla mi fermo
	if (m_output.GetSelectedCount() == 0)
		return;
	
	using namespace libxl;
	CString temp = _T("");		// Per le descrizioni
	CString cella = _T("");		// Cella letta
	CString dir = _T("");		// Cartella dove salvo il file
	CString retVal = _T("");	// Contiene il percorso del file da salvare

	CNoteSeriesDoc * pDoc = GetDocument(); // Puntatore al documento

	int r = 0;		// Riga corrente
	int c = 0;		// Colonna corrente
	short nrow = 6; // Numero di righe
	short ncol = 8; // Numero di colonne
	long n = 0;		// IDDESCR /d o valore /v
	

	// Mi assicuro che ci sia un elemento valido selezionato
	// Basta controllare la variabile dedicata alla tabella, se ha qualcosa
	// e quindi non è vuota allora l'elemento è valido
	if (!(pDoc->m_tabella == _T("[anime]") || pDoc->m_tabella == _T("[da vedere]")))
		return;

	Book* book = xlCreateXMLBook(); // Oggetto Book xlCreateBook
	if (!(book))
		return;

	// Percorso dove salva i file esportati in Excel
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
	{
		len = 255;
		chiave.QueryStringValue((_T("Template")), temp.GetBufferSetLength(len), &len);
		temp.ReleaseBuffer();

		len = 255;
		chiave.QueryStringValue((_T("Export")), dir.GetBufferSetLength(len), &len);
		dir.ReleaseBuffer();
	}
	if (!(book->load(temp)))
	{
		AfxMessageBox(m_currlang.GetDesc(28, temp));
		return;
	}

	Sheet* sheet = book->getSheet(0); // Scheda
	if (!(sheet))
		return;

	sheet->setName(pDoc->m_nome); // Nome scheda

	// Scandisco righe e colonne
	for (r = 0; r < nrow; r++)
	{
		for (c = 0; c < ncol; c++)
		{
			cella = sheet->readStr(r, c); // Leggo la cella

			// Se trovo un "/d" estraggo il suo IDDESCR
			if (cella.Find(L"/d") != -1)
			{
				n = _wtoi(cella.Right(2));
				sheet->writeStr(r, c, m_currlang.GetDesc(n, temp)); // Scrivo la descrizione nella cella
			}
			// Se trovo un "/v" estraggo il suo valore e uso uno switch per
			// decidere quale variabile scrivere nella cella letta
			else if (cella.Find(L"/v") != -1)
			{
				n = _wtoi(cella.Right(1));
				switch (n)
				{
				case 1:
				{
					sheet->writeStr(r, c, pDoc->m_nome);
					break;
				}
				case 2:
				{
					if (pDoc->m_tabella == _T("[anime]"))
					{
						sheet->writeStr(r, c, pDoc->m_inizio);
					}
					else
						sheet->writeStr(r, c, _T("-"));
					break;
				}
				case 3:
				{
					if (pDoc->m_tabella == _T("[anime]"))
					{
						sheet->writeStr(r, c, pDoc->m_fine);
					}
					else
						sheet->writeStr(r, c, _T("-"));
					break;
				}
				case 4:
				{
					dlg0.m_dlgvoto.GetWindowText(pDoc->m_voto);
					sheet->writeStr(r, c, pDoc->m_voto);
					break;
				}
				case 5:
				{
					if (pDoc->m_tabella == _T("[da vedere]"))
						dlg0.m_dlgpriorita.GetWindowText(pDoc->m_priorita);
					else
						sheet->writeStr(r, c, _T("-"));
					break;
				}
				case 6:
				{
					dlg0.m_dlgstato.GetWindowText(pDoc->m_stato);
					sheet->writeStr(r, c, pDoc->m_stato);
					break;
				}
				case 7:
				{
					sheet->writeStr(r, c, pDoc->m_com);
					break;
				}
				}
			}
		}
	}
	/* Salvo il nuovo file Excel e provo ad aprirlo,
	se non ci riesco chiedo se si vuole aprire la cartella di salvataggio*/
	
	// Creo la directory in caso non esista
	CreateDirectoryEx(dir, dir + L"\\Excel", 0);

	dir = dir += L"\\Excel";
	retVal.Format(_T("%s\\%s.xlsx"), dir.GetString(), pDoc->m_nome.GetString());

	// Controllo se il file è già esistente
	CFileFind file;
	if (!(file.FindFile(retVal)))
	{
		book->save(retVal);
		if (!(ShellExecute(NULL, L"open", retVal, NULL, NULL, SW_SHOWMAXIMIZED)))
		{
			if (!(AfxMessageBox(m_currlang.GetDesc(29, temp), MB_YESNO) == IDYES))
				return;

			ShellExecute(NULL, L"open", dir + L"\\Excel", NULL, NULL, SW_SHOWMAXIMIZED);
		}
	}
	else
	{
		// Se il file c'è già chiedo se si vuole sovrascrivere
		if (!(AfxMessageBox(m_currlang.GetDesc(31, temp), MB_YESNO) == IDYES))
			return;

		book->save(retVal);
		ShellExecute(NULL, L"open", retVal, NULL, NULL, SW_SHOWMAXIMIZED);
	}
	book->release();
}

// Passo alla modalità viste
void CNoteSeriesView::OnClickRdoViste()
{
	CNoteSeriesDoc * pDoc = GetDocument(); // Puntatore al documento
	CString sql = _T("");
	
	OnBnClickedBtnreset();
	pDoc->m_tabella = _T("[anime]");
	dlg0.m_dlgChkCartella.EnableWindow(1);
	dlg0.m_dlgcartella.EnableWindow(1);
	dlg0.m_dlgvoto.EnableWindow(1);
	dlg0.m_dlgsimbolo.EnableWindow(1);
	dlg0.m_dlgpriorita.EnableWindow(0);
	dlg0.m_dlgstato.SetCurSel(0);
	m_btnInizia.EnableWindow(0);
	m_btnTerm.EnableWindow(1);
	dlg0.m_btnSalva.EnableWindow(1);
	m_btnDel.EnableWindow(1);
	dlg0.m_dlgsito.EnableWindow(1);
	
	sql.Format(L"SELECT * FROM [anime] WHERE IDCAT=%d ORDER BY nome DESC", pDoc->m_cat);
	pDoc->m_requery.Format(L"SELECT * FROM [anime] WHERE IDCAT=%d ORDER BY nome DESC", pDoc->m_cat);
	dbconfig.UpdateSerieSel(sql, &m_output);
	NascondiColonne(pDoc->m_tabella);
}

// Passo alla modalità future
void CNoteSeriesView::OnClickRdoFuture()
{
	CNoteSeriesDoc * pDoc = GetDocument(); // Puntatore al documento
	CString sql = _T("");
	
	OnBnClickedBtnreset();
	pDoc->m_tabella = _T("[da vedere]");
	dlg0.m_dlgcartella.SetWindowTextW(L"");
	dlg0.m_dlgChkCartella.EnableWindow(0);
	dlg0.m_dlgcartella.EnableWindow(0);
	dlg0.m_dlgvoto.EnableWindow(0);
	dlg0.m_dlgsimbolo.EnableWindow(0);
	dlg0.m_dlgpriorita.EnableWindow(1);
	dlg0.m_dlgstato.SetCurSel(1);
	m_btnInizia.EnableWindow(1);
	m_btnTerm.EnableWindow(0);
	dlg0.m_btnSalva.EnableWindow(1);
	m_btnDel.EnableWindow(1);
	dlg0.m_btnApri.EnableWindow(0);
	dlg0.m_dlgsito.EnableWindow(0);
	
	sql.Format(L"SELECT * FROM [da vedere] WHERE IDCAT=%d ORDER BY nome DESC", pDoc->m_cat);
	pDoc->m_requery.Format(L"SELECT * FROM [da vedere] WHERE IDCAT=%d ORDER BY nome DESC", pDoc->m_cat);
	
	dbconfig.UpdateSerieSel(sql, &m_output);
	NascondiColonne(pDoc->m_tabella);
}

// Inoltro combinazioni di tasti Windows <-> programma
BOOL CNoteSeriesView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && IsDialogMessage(pMsg))
		return TRUE;

	return CFormView::PreTranslateMessage(pMsg);
}


void CNoteSeriesView::OnClickCategorie()
{
	DlgCategorie modifica;
	if (modifica.DoModal() == IDCANCEL)
		return;
	
	CNoteSeriesDoc * pDoc = GetDocument(); // Puntatore al documento
	CString sql,CurrTab,tmp = _T("");
	
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	dbconfig.GetCategoria(&m_TabCtrl, tcItem);
	
	// Seleziono primo tab
	m_TabCtrl.SetCurSel(0);
	
	// Elimino carattere superfluo da ordinamento per lettera
	if (pDoc->m_nodo.Find(L"%") != -1)
		pDoc->m_nodo.Delete(pDoc->m_nodo.GetLength() - 1);

	// Ricavo il testo del tab selezionato
	tcItem.mask = TCIF_TEXT;
	tcItem.cchTextMax = 256;
	tcItem.pszText = CurrTab.GetBuffer(tcItem.cchTextMax);

	if (!(m_TabCtrl.GetItem(0, &tcItem)))
		return;

	CurrTab.ReleaseBuffer();
	dbconfig.GetIDCategoria(&CurrTab, &m_txtIdCat);
	m_txtIdCat.GetWindowTextW(tmp);
	pDoc->m_cat = _wtoi(tmp);
	sql.Format(L"SELECT * FROM %s WHERE IDCAT=%d ORDER BY nome", pDoc->m_tabella.GetString(), pDoc->m_cat);
	dbconfig.UpdateSerieSel(sql, &m_output);
}

// Tengo traccia del tab corrente
void CNoteSeriesView::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	CNoteSeriesDoc * pDoc = GetDocument(); // Puntatore al documento
	if (pDoc->m_requery == _T(""))
		return;

	// Tab corrente e variabile di appoggio
	CString CurrTab, tmp, sql = _T("");
	
	// Elemento CTabCtrl
	TCITEM tcItem;
	
	// Puntatore a questa vista
	CNoteSeriesView* pview = NULL;
	
	// Puntatore a griglia
	CListCtrl *pList = NULL;
	pList = &m_output;
	
	// Tab selezionato
	short nsel = m_TabCtrl.GetCurSel();
	
	// Elimino carattere superfluo da ordinamento per lettera
	/*pDoc->m_nodo == m_currlang.GetDesc(99, temp);
	if(pDoc->m_nodo.Find(L"%") !=-1)
		pDoc->m_nodo.Delete(pDoc->m_nodo.GetLength() - 1);*/
	
	// Ricavo il testo del tab selezionato
	tcItem.mask = TCIF_TEXT;
	tcItem.cchTextMax = 256;
	tcItem.pszText = CurrTab.GetBuffer(tcItem.cchTextMax);

	if (!(m_TabCtrl.GetItem(nsel, &tcItem)))
		return;

	CurrTab.ReleaseBuffer();
	dbconfig.GetIDCategoria(&CurrTab,&m_txtIdCat);
	m_txtIdCat.GetWindowText(tmp);
	pDoc->m_cat = _wtoi(tmp);
	
	int pos = 0;
	CString newCat = _T("");
	newCat.Format(_T("IDCAT=%d"), pDoc->m_cat);
	pos = pDoc->m_requery.Find(L"IDCAT=");
	CString oldCat = pDoc->m_requery.Mid(pos, 7);
	pDoc->m_requery.Replace(oldCat, newCat);
	
	dbconfig.UpdateSerieSel(pDoc->m_requery, pList);
	dlg0.OnBnClickedBtnreset();

	*pResult = 0;
}


void CNoteSeriesView::OnClickModificaDate()
{
	CNoteSeriesDoc * pDoc = GetDocument(); // Puntatore al documento
	if (pDoc->m_nome == _T(""))
		return;
	try
	{
		DlgDate date;
		if (date.DoModal() == IDCANCEL)
			return;

		// Mostro il risultato con requery
		CListCtrl *pList = NULL; // Puntatore a griglia
		pList = &m_output;
		dbconfig.UpdateSerieSel(pDoc->m_requery, pList);
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("Database error: ") + e->m_strError);
	}
}

void CNoteSeriesDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// codice di archiviazione
		ar << m_nome << m_sito << m_cartella << m_nvoto << m_nstato << m_com << m_npriorita;
	}
	else
	{
		// codice di caricamento
		ar >> m_nome >> m_sito >> m_cartella >> m_nvoto >> m_nstato >> m_com >> m_npriorita;
	}
}

void CNoteSeriesView::SalvaProgetto()
{
	CNoteSeriesDoc * pDoc = GetDocument(); // Puntatore al documento
	if (pDoc->m_nome == _T(""))
		return;

	CFileDialog dlg(FALSE,L".ns");
	dlg.m_ofn.lpstrFilter = L"NoteSeries Project (*.ns)\0*.ns\0All Files (*.*)\0*.*\0\0";
	dlg.m_ofn.lpstrTitle = L"Salva file ns come";
	if (dlg.DoModal() != IDOK)
		return;
	
	CString filename = _T("");
	CFile file;
	filename = dlg.GetPathName(); // Percorso del file salvato

	UpdateData(TRUE);
	if (!(file.Open(filename, CFile::modeCreate | CFile::modeWrite)))
		return;

	CArchive ar(&file, CArchive::store);
	dlg0.m_frmnome.GetWindowTextW(pDoc->m_nome);
	dlg0.m_dlgsito.GetWindowTextW(pDoc->m_sito);
	dlg0.m_dlgcartella.GetWindowTextW(pDoc->m_cartella);
	dlg0.m_dlgcom.GetWindowTextW(pDoc->m_com);

	pDoc->m_nvoto = dlg0.m_dlgvoto.GetCurSel();
	pDoc->m_nstato = dlg0.m_dlgstato.GetCurSel();
	pDoc->m_npriorita = dlg0.m_dlgpriorita.GetCurSel();

	pDoc->Serialize(ar);
	ar.Close();
}


void CNoteSeriesView::OnFileOpen()
{
	CFileDialog dlg(TRUE);
	CString filename = _T("");
	dlg.m_ofn.lpstrFilter = L"NoteSeries Project (*.ns)\0*.ns\0All Files (*.*)\0*.*\0\0";
	dlg.m_ofn.lpstrTitle = L"Apri file Note Series";
	if (dlg.DoModal() != IDOK)
		return;

	filename = dlg.GetPathName(); // Percorso del file da aprire
	if (!(filename.Find(L".ns") != -1))
	{
		AfxMessageBox(_T("File not valid"));
		return;
	}
	CNoteSeriesDoc * pDoc = GetDocument(); // Puntatore al documento
	CFile file;
	
	UpdateData(TRUE);
	
	if(!(file.Open(filename, CFile::modeRead)))
		return;
	
	CArchive ar(&file, CArchive::load);
	pDoc->Serialize(ar);

	dlg0.m_frmnome.SetWindowTextW(pDoc->m_nome);
	dlg0.m_dlgsito.SetWindowTextW(pDoc->m_sito);
	dlg0.m_dlgcartella.SetWindowTextW(pDoc->m_cartella);
	dlg0.m_dlgcom.SetWindowTextW(pDoc->m_com);
	dlg0.m_dlgvoto.SetCurSel(pDoc->m_nvoto);
	dlg0.m_dlgstato.SetCurSel(pDoc->m_nstato);
	dlg0.m_dlgpriorita.SetCurSel(pDoc->m_npriorita);
	ar.Close();
	file.Close();

	UpdateData(FALSE);
}


void CNoteSeriesView::OnClickColonna(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CNoteSeriesDoc * pDoc = GetDocument(); // Puntatore al documento
	CListCtrl *pTab = NULL;
	pTab = &m_output;
	switch (pNMLV->iSubItem)
	{
	case 0: // colonna nome
		if (m_orderNome == TRUE) // ordinamento ascendente
		{
			long riga = 0;
			long count = pTab->GetItemCount()-1;
			pTab->SetRedraw(FALSE);
			pTab->DeleteAllItems();
			for(long i = 0; i <= count; i++)
			{
				riga = pTab->InsertItem(i, _T("12313"));
				pTab->SetItemText(riga, 0, pDoc->m_griglia[i].nome);
				pTab->SetItemText(riga, 1, pDoc->m_griglia[i].cartella);
				pTab->SetItemText(riga, 2, pDoc->m_griglia[i].inizio);
				pTab->SetItemText(riga, 3, pDoc->m_griglia[i].fine);
				pTab->SetItemText(riga, 4, pDoc->m_griglia[i].voto);
				pTab->SetItemText(riga, 5, pDoc->m_griglia[i].sito);
				pTab->SetItemText(riga, 6, pDoc->m_griglia[i].priorità);
				pTab->SetItemText(riga, 7, pDoc->m_griglia[i].stato);
				pTab->SetItemText(riga, 8, pDoc->m_griglia[i].commento);
			}
			pTab->SetRedraw(TRUE);
			pTab->Invalidate();
			pTab->UpdateWindow();
			m_orderNome = FALSE;
		}
		else
		{
			// ordinamento decrescente
			long riga = 0;
			long count = pTab->GetItemCount() - 1;
			pTab->SetRedraw(FALSE);
			pTab->DeleteAllItems();
			for (long i = count; i >= 0; i--)
			{
				riga = pTab->InsertItem(i, _T("12313"));
				pTab->SetItemText(riga, 0, pDoc->m_griglia[i].nome);
				pTab->SetItemText(riga, 1, pDoc->m_griglia[i].cartella);
				pTab->SetItemText(riga, 2, pDoc->m_griglia[i].inizio);
				pTab->SetItemText(riga, 3, pDoc->m_griglia[i].fine);
				pTab->SetItemText(riga, 4, pDoc->m_griglia[i].voto);
				pTab->SetItemText(riga, 5, pDoc->m_griglia[i].sito);
				pTab->SetItemText(riga, 6, pDoc->m_griglia[i].priorità);
				pTab->SetItemText(riga, 7, pDoc->m_griglia[i].stato);
				pTab->SetItemText(riga, 8, pDoc->m_griglia[i].commento);
			}

			pTab->SetRedraw(TRUE);
			pTab->Invalidate();
			pTab->UpdateWindow();
			m_orderNome = TRUE;
		}
		break;
	default:
		break;
	}

	*pResult = 0;
}

BOOL CNoteSeriesView::ControllaAggiornamenti()
{
	using namespace std;
	CNoteSeriesDoc * pDoc = GetDocument(); // Puntatore al documento
	// Pulisco la cache
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
		versione.close();
		return FALSE;
	}
	versione.close();
	return TRUE;
}

// Nella sezione future evidenzio righe per priorità
// Rosso: Alta
// Giallo: Media
// Verde: Bassa
// Nero: Nessuna
void CNoteSeriesView::EvidenziaPerPriorita(NMHDR *pNMHDR, LRESULT *pResult)
{
	CNoteSeriesDoc * pDoc = GetDocument(); // Puntatore al documento
	
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	
	CListCtrl* pList = &m_output;
	CString prior = _T("");	
	COLORREF coloreTestor = RGB(255, 255, 255);
	COLORREF coloreSfondo = RGB(105, 105, 105);
	
	*pResult = CDRF_DODEFAULT;
	switch (pLVCD->nmcd.dwDrawStage) {
	case CDDS_PREPAINT:
		//activate painting
		*pResult = CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
		break;
	case CDDS_ITEMPREPAINT:
		if (m_rdoFuture.GetCheck() && pDoc->m_tabella != _T("[anime]")){	// Scorro le righe della griglia
			for (short i = 0; i < pDoc->m_gridCount; i++) {
				prior = pList->GetItemText(i, 6); // Estraggo il testo della priorità
				
				if (prior == pDoc->m_prior.GetAt(0))
					priorità = NESSUNA;

				else if (prior == pDoc->m_prior.GetAt(1))
					priorità = ALTA;

				else if (prior == pDoc->m_prior.GetAt(2))
					priorità = MEDIA;

				else if (prior == pDoc->m_prior.GetAt(3))
					priorità = BASSA;

				else
					priorità = VUOTA;

				if ((pLVCD->nmcd.dwItemSpec) == i) {
					switch (priorità) {
					case ALTA: // Verde
						coloreTestor = RGB(255, 255, 255);			
						coloreSfondo = RGB(63, 181, 122);
						break;
					case MEDIA:	// Giallo
						coloreTestor = RGB(255, 255, 255);
						coloreSfondo = RGB(243, 159, 024);
						break;
					case BASSA: // Rosso
						coloreTestor = RGB(255, 255, 255);
						coloreSfondo = RGB(181, 64, 64);
						break;
					case NESSUNA:
						coloreTestor = RGB(255, 255, 255);			
						coloreSfondo = RGB(105, 105, 105);
						break;

					default:
						coloreTestor = RGB(255, 255, 255);
						coloreSfondo = RGB(105, 105, 105);
						break;
					}
				}
			}
			pLVCD->clrText = coloreTestor;
			pLVCD->clrTextBk = coloreSfondo;
			pResult = CDRF_DODEFAULT;
		}
		if (m_rdoViste.GetCheck() && pDoc->m_tabella == _T("[anime]")){
			// righe pari testo nero sfondo grigino
			if ((pLVCD->nmcd.dwItemSpec % 2) == 0) {
				coloreTestor = RGB(0, 0, 0);
				coloreSfondo = RGB(231, 239, 239);
			}
			pLVCD->clrText = coloreTestor;
			pLVCD->clrTextBk = coloreSfondo;
			pResult = CDRF_DODEFAULT;
		}
		break;
	}
}

// Previene il ridimensionamento delle colonne nascoste
BOOL CNoteSeriesView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CNoteSeriesDoc * pDoc = GetDocument(); // Puntatore al documento
	HD_NOTIFY *pHDN = (HD_NOTIFY*)lParam;

	if ((pHDN->hdr.code == HDN_BEGINTRACKW || pHDN->hdr.code == HDN_BEGINTRACKA) && pHDN->iItem == 6 && pDoc->m_tabella == _T("[anime]"))
	{
		*pResult = TRUE; // disabilito il track
		return TRUE;
	}
	else if ((pHDN->hdr.code == HDN_BEGINTRACKW || pHDN->hdr.code == HDN_BEGINTRACKA) && (pHDN->iItem == 1 || pHDN->iItem == 2 || pHDN->iItem == 3 || pHDN->iItem == 4 || pHDN->iItem == 5) && pDoc->m_tabella != _T("[anime]"))
	{
		*pResult = TRUE; // disabilito il track
		return TRUE;
	}

	return CFormView::OnNotify(wParam, lParam, pResult);
}

// Cambio scheda nel menù basso
void CNoteSeriesView::CambiaMenu(NMHDR *pNMHDR, LRESULT *pResult)
{
	CNoteSeriesDoc * pDoc = GetDocument(); // Puntatore al documento
	CRect ret;
	m_tabCom.GetClientRect(&ret);
	int nsel = m_tabCom.GetCurSel();
	
	if (m_mostraDlg != NULL)
	{
		m_mostraDlg->ShowWindow(SW_HIDE);
		m_mostraDlg = NULL;
	}
	if (pDoc->m_nome == _T("") && nsel != 2)
	{
		m_tabCom.SetCurSel(0);
		nsel = m_tabCom.GetCurSel();
	}
	switch (nsel) {
	case 0:
		dlg0.DestroyWindow();
		dlg0.Create(IDD_AGGIUNGI_MODIFICA, &m_tabCom);
		dlg0.SetWindowPos(NULL, 5, 25, ret.Width() - 10, ret.Height() - 30, SWP_HIDEWINDOW | SWP_NOZORDER);
		dlg0.ShowWindow(SW_SHOW);
		m_mostraDlg = &dlg0;
		break;
	case 1:
		dlg2.DestroyWindow();
		dlg2.Create(IDD_MODIFICA_DATE, &m_tabCom);
		dlg2.SetWindowPos(NULL, 5, 25, ret.Width() - 10, ret.Height() - 30, SWP_HIDEWINDOW | SWP_NOZORDER);
		dlg2.ShowWindow(SW_SHOW);
		m_mostraDlg = &dlg2;
		break;
	case 2:
		dlg1.DestroyWindow();
		dlg1.Create(IDD_GRUPPI, &m_tabCom);
		dlg1.SetWindowPos(NULL, 5, 25, ret.Width() - 10, ret.Height() - 30, SWP_HIDEWINDOW | SWP_NOZORDER);
		dlg1.ShowWindow(SW_SHOW);
		m_mostraDlg = &dlg1;
		break;
	}
	*pResult = 0;
}