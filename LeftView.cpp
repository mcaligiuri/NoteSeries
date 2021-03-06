// LeftView.cpp: implementazione della classe CLeftView
//

#include "stdafx.h"
#include "Note Series.h"

#include "Note SeriesDoc.h"
#include "LeftView.h"
#include "NoTE SeriesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CLeftView::OnTvnSelchanged)
END_MESSAGE_MAP()


// Costruzione/distruzione di CLeftView

CLeftView::CLeftView()
{
	// TODO: aggiungere qui il codice di costruzione.
	retVal = _T("");
	m_nodo = _T("");
	m_sezNodo = _T("");
	tabella = _T("");
	hRoot = NULL;
	hViste = NULL;
	hFuture = NULL;
	hAnni = NULL;
	hAnniF = NULL;
	hMesi = NULL;
	hStati = NULL;
	hStatiV = NULL;
	hPriorita = NULL;
	hAlfa = NULL;
	hAlfaFut = NULL;
	hInizio = NULL;
	hFine = NULL;
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: modificare qui la classe o gli stili Window modificando la struttura CREATESTRUCT

	return CTreeView::PreCreateWindow(cs);
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
	// Puntatore al documento
	CNoteSeriesDoc* pDoc = GetDocument();

	int titolini[9] = {67,99,26,70,68,25,69,100,101};
	for (int i = 0; i < 9; i++) {
		pDoc->m_artitoli.Add(m_currlang.GetDesc(titolini[i] - 1,retVal));
	}

	int form[14] = { 5,38,37,39,25,26,24,27,45,16,41,44,11,110 };
	 for (int j = 0; j < 14; j++) {
		 pDoc->m_form.Add(m_currlang.GetDesc(form[j] - 1, retVal));
	 }

	int date[4] = { 102,103,104,131 };
	for (short k = 0; k < 4; k++ )
		pDoc->m_date.Add(m_currlang.GetDesc(date[k] - 1, retVal));

	// Preparo le descrizioni per il dialog delle impostazioni
	short imp[26] = { 9,10,11,12,13,20,31,14,15,16,17,18,19,143,144,145,146,147,148,149,150,151,68,69,108,152 };
	for (short i = 0; i < 26; i++)
		pDoc->m_setting.Add(m_currlang.GetDesc(imp[i] -1, retVal));
	
	// Preparo le descrizioni per il dialog delle label
	int label[10] = { 175,176,177,178,179,180,181,182,183,184 };
	for (int i = 0; i < 10; i++)
		pDoc->m_label.Add(m_currlang.GetDesc(label[i] - 1, retVal));

	// Pulizia
	GetTreeCtrl().DeleteAllItems();

	// Riferimento 
	CTreeCtrl &liste = this->GetTreeCtrl();

	// Dimensioni caratteri
	CFont *m_pFont = new CFont();
	m_pFont->CreatePointFont(110, _T("Verdana"));
	liste.SetFont(m_pFont, TRUE);

	// Colore del testo e colore di sfondo (scuro con scritte bianche)
	liste.SetTextColor(RGB(255, 255, 255));
	liste.SetBkColor(RGB(54,54,54));

	// Creo la root "dell'albero"
	hRoot = liste.InsertItem(m_currlang.GetDesc(66, retVal), 0, 1);
	//hRoot = liste.InsertItem(_T("SERIE"), 0, 1);
	
	// Genero "albero" con i vari ordinamenti
	SetOrdinamenti(liste, hRoot);

	// Stile dell'"albero"
	liste.ModifyStyle(NULL, TVS_HASBUTTONS | TVS_LINESATROOT | TVS_INFOTIP);
	
	// TODO: è possibile inserire elementi in TreeView mediante accesso diretto al
	//  relativo controllo di albero tramite una chiamata a GetTreeCtrl().
}

// Diagnostica di CLeftView

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CNoteSeriesDoc* CLeftView::GetDocument() // la versione non debug è inline.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNoteSeriesDoc)));
	return (CNoteSeriesDoc*)m_pDocument;
}
#endif //_DEBUG


// Gestori di messaggi di CLeftView


// 1) Controllo cosa ho selezionato nel menù laterale
// 2) al variare delle variabili del documento aggiorno i controlli della vista
// 3) tengo traccia anche della tabella dove è presente la serie selezionata
void CLeftView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	// Riferimento all'albero
	CTreeCtrl &liste = this->GetTreeCtrl(); 
	HTREEITEM hCurSel = liste.GetNextItem(TVI_ROOT, TVGN_CARET); // Selezione corrente
	HTREEITEM hTabSel = liste.GetParentItem(hCurSel); // Elemento precedente alla selezione corrente
	
	if (hCurSel == NULL)
		return;
	
	//  Nodo dell'albero selezionato ora
	m_nodo = (LPCTSTR)liste.GetItemText(hCurSel);

	// Nodo da cui parte il ramo fino al nodo selezionato(padre)
	tabella = liste.GetItemText(hTabSel);
	
	// Puntatore al documento
	CNoteSeriesDoc * pDoc = GetDocument();

	// Controllo se ho cliccato su un titolino
	if (m_nodo == pDoc->m_artitoli.GetAt(3) || m_nodo == pDoc->m_artitoli.GetAt(4) || m_nodo == pDoc->m_artitoli.GetAt(6) ||
		m_nodo == pDoc->m_artitoli.GetAt(5) || m_nodo == pDoc->m_artitoli.GetAt(2) || m_nodo == pDoc->m_artitoli.GetAt(7) || m_nodo == pDoc->m_artitoli.GetAt(8))
		return;


	// Assegno il primo nodo alla variabile del documento
	pDoc->m_nodo = m_nodo;
	
	// Controlli sulla tabella corrente
	// Con assegnazione alla variabile del documento
	
	if (tabella == pDoc->m_artitoli.GetAt(6)) // Viste
		pDoc->m_tabella = _T("[anime]");
	
	else if (tabella == pDoc->m_artitoli.GetAt(4)) // Future
		pDoc->m_tabella = _T("[da vedere]");

	else if (tabella == pDoc->m_artitoli.GetAt(1) || tabella == pDoc->m_artitoli.GetAt(2)) // Alfabeto o stato, perchè si ripetono
	{
		tabella = liste.GetItemText(liste.GetParentItem(hTabSel));
		if(tabella == pDoc->m_artitoli.GetAt(6))
			pDoc->m_tabella = _T("[anime]");
		else
		{
			pDoc->m_tabella = _T("[da vedere]");
			pDoc->m_sezNodo = pDoc->m_artitoli.GetAt(2);
		}
	}
	else
	{
		// Assegno il  nodo padre alla variabile del documento
		// una volta assodato che non sia una tabella del db
		pDoc->m_sezNodo = liste.GetItemText(hTabSel); // anno
		pDoc->m_tabella = _T("[anime]");
		pDoc->m_data = liste.GetItemText(liste.GetParentItem(hTabSel));
	}

	pDoc->UpdateAllViews(NULL); // Aggiorno tutte le viste
	*pResult = 0;
}

// Caricamento ordinamento per ALFABETO,
// DATA DI INIZIO, DATA DI FINE, STATO E PRIORITA'
void CLeftView::SetOrdinamenti(CTreeCtrl & pList, HTREEITEM root)
{
	// Puntatore al documento
	CNoteSeriesDoc* pDoc = GetDocument();
	
	// Descrizioni
	CString temp = _T("");

	// Memorizzo gli stati
	pDoc->m_stati.Add(m_currlang.GetDesc(0, temp));
	pDoc->m_stati.Add(m_currlang.GetDesc(1, temp));
	pDoc->m_stati.Add(m_currlang.GetDesc(2, temp));
	pDoc->m_stati.Add(m_currlang.GetDesc(3, temp));
	pDoc->m_stati.Add(m_currlang.GetDesc(138, temp));

	// Memorizzo le priorità
	pDoc->m_prior.Add(m_currlang.GetDesc(46, temp));
	pDoc->m_prior.Add(m_currlang.GetDesc(47, temp));
	pDoc->m_prior.Add(m_currlang.GetDesc(48, temp));
	pDoc->m_prior.Add(m_currlang.GetDesc(49, temp));
	
	// Memorizzo i mesi
	int mese[12] = { 51,52,53,54,55,56,57,58,59,60,61,62 };	// Array con IDDESCR dei mesi
	for (short j = 0; j < 12; j++)
		pDoc->m_armesi.Add(m_currlang.GetDesc(mese[j]-1, temp));
	
	// Lettera
	CString let = _T("");
	
	// Conterrà l'anno
	CString anno = _T("");
	
	// Anno corrente in formato numerico
	long d = _wtoi(CTime::GetCurrentTime().Format("%Y"));

	// Creo le sezioni Viste e Future
	hViste = pList.InsertItem(m_currlang.GetDesc(68, temp), root);		// Viste
	// hViste = pList.InsertItem(_T("Viste"), root);
	hFuture = pList.InsertItem(m_currlang.GetDesc(67, temp), root);		// Future
	//hFuture = pList.InsertItem(_T("Future"), root);
	
	// Genero le lettere dell' alfabeto
	// e le visualizzo in entrambe le sezioni
	hAlfa = pList.InsertItem(m_currlang.GetDesc(98,retVal), hViste);		// Ordine alfabetico viste
	//hAlfa = pList.InsertItem(_T("Alfabeto"), hViste);
	//hAlfaFut = pList.InsertItem(_T("Alfabeto"), hFuture);	// Ordine alfabetico future
	hAlfaFut = pList.InsertItem(m_currlang.GetDesc(98, retVal), hFuture);
	hStatiV = pList.InsertItem(m_currlang.GetDesc(25, retVal), hViste);
	//hStatiV = pList.InsertItem(_T("Stato"), hViste);		// Ordine per stati in corso e finito
	
    // Inserisco nodo 0-9 in entrambe le sezioni
	pList.InsertItem(L"0-9", hAlfa);									
	pList.InsertItem(L"0-9", hAlfaFut);
	
	for (short i = 65; i < 91; i++)
	{
		let.Format(_T("%c"), i);
		pList.InsertItem(let, hAlfa);
		pList.InsertItem(let, hAlfaFut);
	}
	pList.Expand(root, TVE_EXPAND);
	
	//VISUALIZZAZIONE ANNO-MESE E STATO
	hViste = pList.InsertItem(m_currlang.GetDesc(69, temp), hViste);
	//hViste = pList.InsertItem(_T("Data"), hViste);
	// Genero gli anni e sotto ogni anno
	// faccio comparire i mesi per data inizio e fine
	hInizio = pList.InsertItem(m_currlang.GetDesc(99, retVal), hViste);
	hFine = pList.InsertItem(m_currlang.GetDesc(100, retVal), hViste);

	/*hInizio = pList.InsertItem(_T("Inizio"), hViste);
	hFine = pList.InsertItem(_T("Fine"), hViste);*/
	for (long i = 2014; i < d + 1; i++)
	{
		anno.Format(_T("%d"), i);
		hAnni = pList.InsertItem(anno, hInizio);
		hAnniF = pList.InsertItem(anno, hFine);
		for (short i = 0; i < pDoc->m_armesi.GetSize(); i++)
		{
			hMesi = pList.InsertItem(pDoc->m_armesi.GetAt(i), hAnni);		// Inizio
			hMesi = pList.InsertItem(pDoc->m_armesi.GetAt(i), hAnniF);		// Fine
		}
	}

	// Finito e In corso
	pList.InsertItem(pDoc->m_stati.GetAt(0), hStatiV);
	pList.InsertItem(pDoc->m_stati.GetAt(2), hStatiV);

	// In attesa, In uscita, CONCLUSO
	hStati = pList.InsertItem(m_currlang.GetDesc(25, temp), hFuture); // Stato
	pList.InsertItem(pDoc->m_stati.GetAt(1), hStati);
	pList.InsertItem(pDoc->m_stati.GetAt(3), hStati);
	pList.InsertItem(pDoc->m_stati.GetAt(4), hStati);

	// Nessuna, Alta, Media, Bassa
	 hPriorita = pList.InsertItem(m_currlang.GetDesc(24, temp), hFuture); // Priorità
	for (short j = 0; j < pDoc->m_prior.GetSize(); j++)
		pList.InsertItem(pDoc->m_prior.GetAt(j), hPriorita);
}
