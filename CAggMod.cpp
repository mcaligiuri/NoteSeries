// CAggMod.cpp: file di implementazione
//

#include "stdafx.h"
#include "Note Series.h"
#include "CAggMod.h"
#include "afxdialogex.h"
#include "Note SeriesView.h"
#include "MainFrm.h"
#include "DlgCartella.h"

// Finestra di dialogo CAggMod

IMPLEMENT_DYNAMIC(CAggMod, CDialogEx)

CAggMod::CAggMod(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AGGIUNGI_MODIFICA, pParent)
{
	temp = _T("");
	len = 255;
	width = 60;
	height = 60;
}

CAggMod::~CAggMod()
{
}

void CAggMod::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;				// Puntatore alla mainframe
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();		// Puntatore alla vista principale
	CNoteSeriesDoc* pDoc = pView->GetDocument();							// Puntatore al documento

	DDX_Control(pDX, IDC_COMBOSTATO, m_dlgstato);
	DDX_Control(pDX, IDC_PRIORITA, m_dlgpriorita);
	DDX_Control(pDX, IDC_COMBOVOTO, m_dlgvoto);
	DDX_Control(pDX, IDC_EDITNOME, m_frmnome);
	DDX_Control(pDX, IDC_EDITCOMMENTO, m_dlgcom);
	DDX_Control(pDX, IDC_EDITCARTELLA, m_dlgcartella);
	DDX_Control(pDX, IDC_CHECKCARTELLA, m_dlgChkCartella);
	DDX_Control(pDX, IDC_EDITSITO, m_dlgsito);
	DDX_Control(pDX, IDC_EDITBMP, m_dlgsimbolo);

	// Testo
	DDX_Text(pDX, IDC_EDITNOME, pDoc->m_nome);
	DDX_Text(pDX, IDC_EDITCARTELLA, pDoc->m_cartella);
	DDX_Text(pDX, IDC_COMBOSTATO, pDoc->m_stato);
	DDX_Text(pDX, IDC_COMBOVOTO, pDoc->m_voto);
	DDX_Text(pDX, IDC_EDITCOMMENTO, pDoc->m_com);
	DDX_Text(pDX, IDC_EDITSITO, pDoc->m_sito);
	DDX_Text(pDX, IDC_EDITBMP, pDoc->m_bmp);

	//DDX_Control(pDX, IDC_OUTPUT, m_output);

	// Label
	DDX_Control(pDX, IDC_LABEL, m_dlgLabel);
	DDX_Control(pDX, IDC_LBLNAME, m_lblNome);
	DDX_Control(pDX, IDC_LBLSITO, m_lblSito);
	DDX_Control(pDX, IDC_LBLSIMBOLO, m_lblSimbolo);
	DDX_Control(pDX, IDC_LBLPRIORITA, m_lblPriorita);
	DDX_Control(pDX, IDC_LBLSTATO, m_lblStato);
	DDX_Control(pDX, IDC_LBLVOTO, m_lblVoto);
	DDX_Control(pDX, IDC_LBLCOM, m_lblCom);
	// Bottoni
	DDX_Control(pDX, IDC_BTNSAVE, m_btnSalva);
	DDX_Control(pDX, IDC_BTNAGG, m_btnAggiungi);
	DDX_Control(pDX, IDC_BTNRESET, m_btnReset);
	DDX_Control(pDX, IDC_BTNAPRICARTELLA, m_btnApri);
	DDX_Control(pDX, IDC_BTNCOPIAURL, m_btnCopiaURL);
}


BEGIN_MESSAGE_MAP(CAggMod, CDialogEx)

	ON_BN_CLICKED(IDC_BTNAPRICARTELLA, &CAggMod::OnBnClickedBtnapricartella)
	ON_BN_CLICKED(IDC_BTNRESET,&CAggMod::OnBnClickedBtnreset)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_OUTPUT, &CAggMod::SerieSelezionata)
	ON_BN_CLICKED(IDC_BTNAGG, &CAggMod::AggiungiSerie)
	ON_BN_CLICKED(IDC_BTNSAVE, &CAggMod::ModificaSerie)
	ON_BN_CLICKED(IDC_BTNAPRICARTELLA, &CAggMod::OnBnClickedBtnapricartella)
	ON_BN_CLICKED(IDC_CHECKCARTELLA,&CAggMod::OnBnClickedCheckcartella)
	ON_CBN_SELCHANGE(IDC_COMBOSTATO, &CAggMod::OnCbnSelchangeCombostato)
	ON_BN_CLICKED(IDC_BTNCOPIAURL,&CAggMod::OnClickCopiaURL)
END_MESSAGE_MAP()

BOOL CAggMod::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;				// Puntatore alla mainframe
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();		// Puntatore alla vista principale
	CNoteSeriesDoc* pDoc = pView->GetDocument();							// Puntatore al documento
	// Popolo la combo dei voti
	CString voto = _T("-");
	short flag = 0;
	m_dlgvoto.ResetContent();
	m_dlgvoto.AddString(voto);
	for (double j = 0; j < 10.5; j += 0.5)
	{
		flag++;
		if (flag % 2 == 0) 
			voto.Format(L"%.1f", j);
		else 
			voto.Format(L"%.0f", j);

		m_dlgvoto.AddString(voto);
	}
	m_dlgvoto.SetCurSel(0);

	// Configurazione controlli attivi
	m_btnSalva.EnableWindow(0);
	m_dlgstato.SetCurSel(0);
	m_dlgpriorita.SetCurSel(0);
	m_dlgpriorita.EnableWindow(0);
	m_btnApri.EnableWindow(0);
	m_dlgChkCartella.SetCheck(FALSE);
	m_dlgcartella.EnableWindow(FALSE);
	// Imposto icone bottoni
	SetIcons();
	this->SetBackgroundColor(RGB(255, 255, 255), 1);
	GetDescUpdate();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // ECCEZIONE: le pagine delle proprietà OCX devono restituire FALSE
}

void CAggMod::GetDescUpdate()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;				// Puntatore alla mainframe
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();		// Puntatore alla vista principale
	CNoteSeriesDoc* pDoc = pView->GetDocument();							// Puntatore al documento

	// Combo dello stato
	m_dlgstato.ResetContent();
	m_dlgstato.AddString(pDoc->m_stati.GetAt(0));
	m_dlgstato.AddString(pDoc->m_stati.GetAt(1));
	m_dlgstato.AddString(pDoc->m_stati.GetAt(2));
	m_dlgstato.AddString(pDoc->m_stati.GetAt(3));
	m_dlgstato.AddString(pDoc->m_stati.GetAt(4));
	m_dlgstato.SetCurSel(0);

	// Popolo la combo delle priorità
	m_dlgpriorita.ResetContent();
	m_dlgpriorita.AddString(pDoc->m_prior.GetAt(0));
	m_dlgpriorita.AddString(pDoc->m_prior.GetAt(1));
	m_dlgpriorita.AddString(pDoc->m_prior.GetAt(2));
	m_dlgpriorita.AddString(pDoc->m_prior.GetAt(3));
	m_dlgpriorita.SetCurSel(0);

	// Label
	m_dlgLabel.SetWindowTextW(pDoc->m_form.GetAt(0));
	m_lblNome.SetWindowTextW(pDoc->m_form.GetAt(1));
	m_lblSito.SetWindowTextW(pDoc->m_form.GetAt(2));
	m_lblSimbolo.SetWindowTextW(pDoc->m_form.GetAt(3));
	m_lblPriorita.SetWindowTextW(pDoc->m_form.GetAt(4));
	m_lblStato.SetWindowTextW(pDoc->m_form.GetAt(5));
	m_lblVoto.SetWindowTextW(pDoc->m_form.GetAt(6));
	m_lblCom.SetWindowTextW(pDoc->m_form.GetAt(7));

	// Bottoni
	m_btnSalva.SetWindowTextW(pDoc->m_form.GetAt(9));			// Bottone salva modifiche
	m_btnAggiungi.SetWindowTextW(pDoc->m_form.GetAt(10));		// Bottone aggiungi
	m_btnReset.SetWindowTextW(pDoc->m_form.GetAt(11));			// Bottone reset
	m_dlgChkCartella.SetWindowTextW(pDoc->m_form.GetAt(12));	// Checkbox cartella
	m_btnApri.SetWindowTextW(pDoc->m_form.GetAt(12));			// Bottone che apre la cartella
	m_btnCopiaURL.SetWindowTextW(pDoc->m_form.GetAt(13));		// Bottone che copia l'URL
}

// Imposto icone dei pulsanti
void CAggMod::SetIcons()
{
	HICON add = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_ADD), IMAGE_ICON, 20, 20, LR_DEFAULTCOLOR);
	m_btnAggiungi.SetIcon(add);
	HICON res = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_RES), IMAGE_ICON, 20, 20, LR_DEFAULTCOLOR);
	m_btnReset.SetIcon(res);
	HICON mod = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_SAVE), IMAGE_ICON, 20, 20, LR_DEFAULTCOLOR);
	m_btnSalva.SetIcon(mod);
	HICON dir = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_FOLDER), IMAGE_ICON, 20, 20, LR_DEFAULTCOLOR);
	m_btnApri.SetIcon(dir);
}

void CAggMod::OnBnClickedBtnapricartella()
{
	DlgCartella dir = new DlgCartella();
	dir.DoModal();
	delete dir;
}

void CAggMod::SerieSelezionata(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;				// Puntatore alla mainframe
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();		// Puntatore alla vista principale
	CNoteSeriesDoc* pDoc = pView->GetDocument();							// Puntatore al documento

	// Puntatore a picture control
	CStatic *pctrl = NULL;
	pctrl = (CStatic *)GetDlgItem(IDC_ICONSERIES);

	// Serve a spostare e/o ridimensionare il picture control, comprensivo di foto
	CWnd* cornice = pctrl;

	// Serve per il caricamento di una foto
	CImage foto;

	// Serve per settare la foto sul controllo
	CBitmap bmp;

	// Aggiorno i controlli con i dati della serie selezionata
	m_frmnome.SetWindowTextW(pDoc->m_nome);
	m_dlgcom.SetWindowTextW(pDoc->m_com);
	m_dlgsimbolo.SetWindowTextW(pDoc->m_bmp);
	m_dlgsito.SetWindowTextW(pDoc->m_sito);
	m_dlgcartella.SetWindowTextW(pDoc->m_cartella);
	short m_sel = _wtoi(pDoc->m_stato);
	m_dlgstato.SetCurSel(m_sel);
	
	short m_voto = _wtoi(pDoc->m_voto);
	m_dlgvoto.SetCurSel(m_voto);
	
	m_btnSalva.EnableWindow(1);

	if (pDoc->m_cartella != L"" && pDoc->m_cartella != L"-")
		m_btnApri.EnableWindow(1);

	// Evito glitch grafico
	m_dlgsimbolo.OnChangeLayout();

	/*Se la variabile del documento, non contiene una stringa vuota o il percorso relativo della cartella dei loghi
vuol dire che in allegato alla serie c'è un logo, quindi lo mostro*/
	if (pDoc->m_bmp != _T("") && pDoc->m_bmp != relPath)
	{
		foto.Load(pDoc->m_bmp);
		bmp.Attach(foto.Detach());
		pctrl->SetBitmap(bmp);

		// Ridimensiono il controllo CWnd con foto già settata senza spostarla.
		// Così l'utente non deve preoccuparsi delle dimensioni del logo
		cornice->SetWindowPos(NULL, 0, 0, width, height, SWP_NOREPOSITION | SWP_NOMOVE);
	}
	else
	{
		pctrl->SetBitmap(NULL);
		pctrl->Invalidate();
	}

	// A seconda della tabella del db, attivo o disattivo i controlli
	if (pDoc->m_tabella == _T("[anime]"))
	{
		m_btnSalva.EnableWindow(1);
		m_dlgChkCartella.EnableWindow(1);
		m_dlgcartella.EnableWindow(1);
		m_dlgsito.EnableWindow(1);
		m_dlgsimbolo.EnableWindow(1);
		m_dlgvoto.EnableWindow(1);
		m_dlgpriorita.EnableWindow(0);
	}
	else
	{
		m_btnSalva.EnableWindow(1);
		m_dlgChkCartella.EnableWindow(0);
		m_dlgcartella.EnableWindow(0);
		m_btnApri.EnableWindow(0);
		m_dlgsito.EnableWindow(0);
		m_dlgsimbolo.EnableWindow(0);
		m_dlgvoto.EnableWindow(0);
		
		m_dlgpriorita.EnableWindow(1);
		m_dlgpriorita.SetCurSel(_wtoi(pDoc->m_priorita));
	}
	*pResult = 0;
}

// Checkbox della cartella
void CAggMod::OnBnClickedCheckcartella()
{
	CString nome, buf = _T("");
	char specialChar[10] = "\"\\/:*|<>";
	if (!(m_dlgChkCartella.GetCheck()))
	{
		//m_dlgcartella.SetWindowTextW(_T(""));
		m_dlgcartella.EnableWindow(0);
		m_btnApri.EnableWindow(0);
	}
	else
	{
		m_dlgcartella.EnableWindow(1);
		m_frmnome.GetWindowTextW(nome);

		// Caratteri speciali sostituiti con uno spazio
		for (short i = 0; i < 10; i++)
		{
			buf.Format(_T("%c"), specialChar[i]);
			if (nome.Find(buf) != -1 && nome.Find(buf) != '\0')
			{
				nome.Replace(buf, _T(" "));
				m_dlgLabel.EnableWindow(1);
				m_dlgLabel.SetWindowTextW(m_currlang.GetDesc(113, temp) + L"\n \\ / : * | <>\"");
			}
		}
		// Controllo spazi ad inizio e fine
		int idf = nome.Find(L" ");
		if (idf == 0)
			nome.Delete(idf, 1);

		int idc = nome.Find(L" ");
		if (idc == nome.GetLength() - 1)
			nome.Delete(idc, nome.GetLength() - 1);

		m_dlgcartella.SetWindowTextW(nome);
	}
}
void CAggMod::GetCartella()
{
	// Bisogna aver checcato l'opzione per la cartella
	if (!(m_dlgChkCartella.GetCheck()))
		return;

	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;				// Puntatore alla mainframe
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();		// Puntatore alla vista principale
	CNoteSeriesDoc* pDoc = pView->GetDocument();
	char specialChar[10] = "\"\\/:*|<>";
	CString buf = _T("");
	if (chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
		return;
	
	CString path = _T(""); // Percorso assoluto
	len = 255;
	chiave.QueryStringValue(_T("Cartella"), relPath.GetBufferSetLength(len), &len);

	// Caratteri speciali sostituiti con uno spazio
	for (short i = 0; i < 10; i++)
	{
		buf.Format(_T("%c"), specialChar[i]);
		pDoc->m_cartella.Replace(buf, _T(" "));
	}
	// Controllo spazi a inizio e fine
	int idf = pDoc->m_cartella.Find(L" ");
	if (idf == 0)
		pDoc->m_cartella.Delete(idf, 1);

	int idc = pDoc->m_cartella.Find(L" ");
	if (idc == pDoc->m_cartella.GetLength() - 1)
		pDoc->m_nome.Delete(idc, pDoc->m_cartella.GetLength() - 1);

	//Costruisco il percorso
	path.Format(_T("%s\\%s"), relPath, pDoc->m_cartella);

	// Creo la directory
	if (!(CreateDirectoryEx(relPath, path, 0)))
	{
		AfxMessageBox(m_currlang.GetDesc(76, temp)); // directory già esitente
		return;
	}
}

// Aggiungo nuova serie
void CAggMod::AggiungiSerie()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;				// Puntatore alla mainframe
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();		// Puntatore alla vista principale
	CNoteSeriesDoc* pDoc = pView->GetDocument();

	// Assegno contenuto controlli a variabili del documento
	// le variabili del documento verranno usate nella query sql
	m_frmnome.GetWindowText(pDoc->m_nome);
	if (pDoc->m_nome == _T(""))
		return;

	// Elimino spazi all'inizio e alla fine della stringa del nome
	int idf = pDoc->m_nome.Find(L" ");
	if (idf == 0)
		pDoc->m_nome.Delete(idf, 1);

	int idc = pDoc->m_nome.Find(L" ");
	if (idc == pDoc->m_nome.GetLength()-1)
		pDoc->m_nome.Delete(idc, pDoc->m_nome.GetLength()-1);

	if (m_dlgstato.GetCurSel() == 0 || m_dlgstato.GetCurSel() == 2)
	{
		// Costruisco la data di inizio e fine
		pDoc->m_inizio = CTime::GetCurrentTime().Format("%d/%m/%Y");

		switch (m_dlgstato.GetCurSel())
		{
		case 0:
			pDoc->m_fine = CTime::GetCurrentTime().Format("%d/%m/%Y");
			break;
		case 2:
			pDoc->m_fine = _T("-");
			m_dlgvoto.SetCurSel(0);
			break;
		}

		m_dlgcom.GetWindowText(pDoc->m_com);
		m_dlgsimbolo.GetWindowText(pDoc->m_bmp);
		if (m_dlgChkCartella.GetCheck())
		{
			m_dlgcartella.GetWindowText(pDoc->m_cartella);
			if (pDoc->m_cartella != _T(""))
				GetCartella();
		}
		else
			pDoc->m_cartella = _T("");
		
		m_dlgsito.GetWindowText(pDoc->m_sito);
		// Controllo apici singoli nel nome e nel commento per sql
		if (pDoc->m_nome.Find(L"'") != -1 || pDoc->m_com.Find(L"'") != -1)
		{
			pDoc->m_nome.Replace(L"'", L"''");
			pDoc->m_com.Replace(L"'", L"''");
		}

		// Gestisco omonimi
		int count = dbconfig.ContaOmonimi(pDoc->m_nome, pDoc->m_tabella, pDoc->m_cat);
		if (count > 0)
		{
			CString val = _T("");
			val.Format(_T(" (%d)"), count + 1);
			pDoc->m_nome = pDoc->m_nome + val;
		}

		if (pDoc->m_bmp != _T(""))
			pView->GetLogo();

		// Posizione delle combobox
		short m_sel = m_dlgstato.GetCurSel();
		short m_votosel = m_dlgvoto.GetCurSel();

		// Tabella del database
		pDoc->m_tabella = _T("[anime]");

		// Costruisco la query
		m_sql.Format
		(L"INSERT INTO %s(nome, cartella, inizio, fine, stato, voto, commenti, sito, bitmap, IDCAT) VALUES('%s', '%s', '%s', '%s', '%d', '%d', '%s', '%s', '%s', '%d')",
			pDoc->m_tabella, pDoc->m_nome, pDoc->m_cartella, pDoc->m_inizio, pDoc->m_fine, m_sel, m_votosel, pDoc->m_com, pDoc->m_sito, pDoc->m_bmp, pDoc->m_cat);
	}
	else
	{
		// Assegno contenuto controlli a variabili del documento
		// le variabili del documento verranno usate nella query m_sql
		// in questo caso ometto delle informazioni, come il logo
		m_frmnome.GetWindowText(pDoc->m_nome);
		m_dlgcom.GetWindowText(pDoc->m_com);

		// Controllo apici singoli nel nome e nel commento per sql
		if (pDoc->m_nome.Find(L"'") != -1 || pDoc->m_com.Find(L"'") != -1)
		{
			pDoc->m_nome.Replace(L"'", L"''");
			pDoc->m_com.Replace(L"'", L"''");
		}

		// Posizione delle combo
		short stato = m_dlgstato.GetCurSel();
		short prior = m_dlgpriorita.GetCurSel();

		// Tabella del database
		pDoc->m_tabella = _T("[da vedere]");

		// Gestisco omonimi
		int count = dbconfig.ContaOmonimi(pDoc->m_nome, pDoc->m_tabella, pDoc->m_cat);
		if (count > 0)
		{
			CString val = _T("");
			val.Format(_T(" (%d)"), count + 1);
			pDoc->m_nome = pDoc->m_nome + val;
		}

		m_sql.Format
		(L"INSERT INTO %s (nome, stato, commenti, priorità, IDCAT) VALUES('%s', '%d', '%s', '%d', '%d')",
			pDoc->m_tabella, pDoc->m_nome, stato, pDoc->m_com, prior, pDoc->m_cat);
	}

	// Eseguo la query e aggiorno le viste
	if (!(dbconfig.SetSerie(m_sql)))
		return;

	// Mostro il risultato con requery
	//m_sql.Format(L"SELECT * FROM %s WHERE IDCAT=%d ORDER BY nome", pDoc->m_tabella, pDoc->m_cat);
	CListCtrl* pList = &pView->m_output;
	dbconfig.UpdateSerieSel(pDoc->m_requery, pList);


	//pDoc->UpdateAllViews(NULL);

	pView->OnBnClickedBtnreset();
}

// Salvo modifiche a una serie
void CAggMod::ModificaSerie()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;				// Puntatore alla mainframe
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();		// Puntatore alla vista principale
	CNoteSeriesDoc* pDoc = pView->GetDocument();							// Puntatore al documento

	// CATEGORIA
	CString CurrTab, tmp = _T("");			// Tab corrente e variabile di appoggio

	// Elemento CTabCtrl
	TCITEM tcItem;

	// Tab selezionato
	short nsel = pView->m_TabCtrl.GetCurSel();

	// Ricavo il testo del tab selezionato
	tcItem.mask = TCIF_TEXT;
	tcItem.cchTextMax = 256;
	tcItem.pszText = CurrTab.GetBuffer(tcItem.cchTextMax);

	if (!(pView->m_TabCtrl.GetItem(nsel, &tcItem)))
		return;

	CurrTab.ReleaseBuffer();
	dbconfig.GetIDCategoria(&CurrTab, &pView->m_txtIdCat);
	pView->m_txtIdCat.GetWindowText(tmp);
	pDoc->m_cat = _wtoi(tmp);

	// Il nome ci deve essere e una riga deve essere selezionata
	m_frmnome.GetWindowText(pDoc->m_nome);
	if (pDoc->m_nome == _T("") || pView->m_output.GetSelectedCount() == 0)
		return;

	// Controllo spazi a inizio e fine del nome
	int idf = pDoc->m_nome.Find(L" ");
	if (idf == 0)
		pDoc->m_nome.Delete(idf, 1);

	int idc = pDoc->m_nome.Find(L" ");
	if (idc == pDoc->m_nome.GetLength() - 1)
		pDoc->m_nome.Delete(idc, pDoc->m_nome.GetLength() - 1);

	if (!(AfxMessageBox(m_currlang.GetDesc(8, temp), MB_YESNO) == IDYES))
		return;

	if (pDoc->m_tabella == _T("[anime]"))
	{
		CListCtrl *pList = NULL; // Puntatore a grigla
		pList = &pView->m_output;

		m_dlgcom.GetWindowText(pDoc->m_com);			// commento
		m_dlgsito.GetWindowText(pDoc->m_sito);			// sito
		if (m_dlgChkCartella.GetCheck())				// cartella
			m_dlgcartella.GetWindowText(pDoc->m_cartella);

		m_dlgsimbolo.GetWindowText(pDoc->m_bmp);		// simbolo

		// Controllo apici singoli per SQL
		if (pDoc->m_nome.Find(L"'") != -1 || pDoc->m_com.Find(L"'") != -1)
		{
			pDoc->m_nome.Replace(L"'", L"''");
			pDoc->m_com.Replace(L"'", L"''");
		}

		short m_sel = m_dlgstato.GetCurSel();
		switch (m_sel)
		{
		case 0:
			if (pDoc->m_fine == _T(""))
				pDoc->m_fine = CTime::GetCurrentTime().Format("%d/%m/%Y");
			break;
		case 2:
			pDoc->m_fine = _T("-");
			m_dlgvoto.SetCurSel(0);
			break;
		}

		// combo del voto
		short m_selv = m_dlgvoto.GetCurSel();

		// id del record
		short m_ida = _wtoi(pDoc->m_ida);

		// Provo a caricare il logo
		if (pDoc->m_bmp != _T(""))
			pView->GetLogo();

		// Costruisco la query
		m_sql.Format(L"UPDATE [anime] SET nome='%s', cartella ='%s', inizio ='%s', fine ='%s', stato =%d, voto =%d, commenti ='%s', sito ='%s', bitmap ='%s' WHERE ida=%d",
			pDoc->m_nome, pDoc->m_cartella, pDoc->m_inizio, pDoc->m_fine, m_sel, m_selv, pDoc->m_com, pDoc->m_sito, pDoc->m_bmp, m_ida);

		// Provo a creare la cartella
		if (pDoc->m_cartella != _T("") && m_dlgChkCartella.GetCheck())
			GetCartella();

		// Eseguo la query e aggiorno le viste
		if (!(dbconfig.SetSerie(m_sql)))
			return;

		//Mostro il risultato con requery
		/*CString requery = _T("");
		requery.Format(_T("SELECT * FROM[anime] WHERE IDCAT = %d ORDER BY nome"), pDoc->m_cat);*/
		dbconfig.UpdateSerieSel(pDoc->m_requery, pList);
		//pDoc->UpdateAllViews(NULL);
		OnBnClickedBtnreset();
	}
	else
	{
		CListCtrl *pList = NULL; // Puntatore a grigla
		pList = &pView->m_output;

		m_dlgcom.GetWindowText(pDoc->m_com);			// commento

				// Controllo apici singoli per SQL
		if (pDoc->m_nome.Find(L"'") != -1 || pDoc->m_com.Find(L"'") != -1)
		{
			pDoc->m_nome.Replace(L"'", L"''");
			pDoc->m_com.Replace(L"'", L"''");
		}

		// Posizione combo stato
		short m_sel = m_dlgstato.GetCurSel();
		short m_pri = m_dlgpriorita.GetCurSel();

		// id del record
		short m_ida = _wtoi(pDoc->m_idv);

		// Costruisco la query
		m_sql.Format(L"UPDATE [da vedere] SET nome='%s', stato =%d, priorità=%d, commenti ='%s' WHERE idv=%d AND IDCAT=%d", 
					pDoc->m_nome, m_sel, m_pri, pDoc->m_com, m_ida, pDoc->m_cat);

		// Eseguo la query e aggiorno le viste
		if (!(dbconfig.SetSerie(m_sql)))
			return;

		//Mostro il risultato con requery
		/*CString requery = _T("");
		requery.Format(_T("SELECT * FROM[da vedere] WHERE IDCAT = %d ORDER BY nome"), pDoc->m_cat);*/
		dbconfig.UpdateSerieSel(pDoc->m_requery, pList);
		//pDoc->UpdateAllViews(NULL);
		OnBnClickedBtnreset();
	}
}

// Al cambio selezione nella combo "stato" decido quali controlli attivare
void CAggMod::OnCbnSelchangeCombostato()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;				// Puntatore alla mainframe
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();		// Puntatore alla vista principale
	CNoteSeriesDoc* pDoc = pView->GetDocument();							// Puntatore al documento
	CButton* rdov = (CButton *)pView->GetDlgItem(IDC_RDOVISTE);
	CButton* rdof = (CButton *)pView->GetDlgItem(IDC_RDOFUTURE);
	
	if (rdov->GetCheck())
	{
		switch (m_dlgstato.GetCurSel())
		{
		case 1:
			m_dlgstato.SetCurSel(0);
			break;
		case 3:
			m_dlgstato.SetCurSel(2);
			break;
		case 4:
			m_dlgstato.SetCurSel(0);
			break;
		}
	}
	else
	{
		switch (m_dlgstato.GetCurSel())
		{
		case 0:
			m_dlgstato.SetCurSel(1);
			break;
		case 2:
			m_dlgstato.SetCurSel(3);
			break;
		}
	}
	switch (m_dlgstato.GetCurSel())
	{
	// in attesa e in uscita
	case 1:
	case 3:
	case 4:
	{
		m_dlgpriorita.EnableWindow(1);
		m_dlgvoto.EnableWindow(0);
		m_frmnome.EnableWindow(1);
		m_dlgcom.EnableWindow(1);
		m_dlgcartella.EnableWindow(0);
		m_dlgChkCartella.EnableWindow(0);
		m_dlgsito.EnableWindow(0);
		m_dlgsimbolo.EnableWindow(0);
		rdof->SetCheck(TRUE);
		rdov->SetCheck(FALSE);
		pDoc->m_tabella = ("[da vedere]");
		pView->NascondiColonne(pDoc->m_tabella);
		break;
	}
	// finito e in corso
	case 0:
	case 2:
	{
		m_dlgpriorita.EnableWindow(0);
		m_dlgvoto.EnableWindow(1);
		m_frmnome.EnableWindow(1);
		m_dlgcom.EnableWindow(1);
		m_dlgcartella.EnableWindow(1);
		m_dlgChkCartella.EnableWindow(1);
		m_dlgsito.EnableWindow(1);
		rdof->SetCheck(FALSE);
		rdov->SetCheck(TRUE);
		pDoc->m_tabella = ("[anime]");
		pView->NascondiColonne(pDoc->m_tabella);
		break;
	}
	default: 0;
	}
	// Se non ho finito non ha senso dare un voto
	if (!(m_dlgstato.GetCurSel() == 0))
		m_dlgvoto.EnableWindow(0);
	else
		m_dlgvoto.EnableWindow(1);
}

// Reset controlli
void CAggMod::OnBnClickedBtnreset()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;				// Puntatore alla mainframe
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();		// Puntatore alla vista principale
	CNoteSeriesDoc* pDoc = pView->GetDocument();							// Puntatore al documento

	// Pulizia griglia e caselle di testo

	m_frmnome.SetWindowTextW(_T(""));
	m_dlgcom.SetWindowTextW(_T(""));
	m_dlgsimbolo.SetWindowTextW(_T(""));
	m_dlgsito.SetWindowTextW(_T(""));
	m_dlgcartella.SetWindowTextW(_T(""));

	// Combo in posizione 0
	m_dlgstato.SetCurSel(0);
	m_dlgvoto.SetCurSel(0);

	// Nessun logo mostrato
	CStatic *pctrl = (CStatic *)GetDlgItem(IDC_ICONSERIES);
	pctrl->SetBitmap(NULL);
	pctrl->Invalidate();

	// Label informativa
	m_dlgLabel.SetWindowTextW(m_currlang.GetDesc(5, temp));
	m_dlgLabel.EnableWindow(1);

	// Pulsanti
	m_btnAggiungi.EnableWindow(1);
	pView->m_btnInizia.EnableWindow(0);
	pView->m_btnTerm.EnableWindow(0);
	m_btnSalva.EnableWindow(0);
	pView->m_btnDel.EnableWindow(0);
	m_btnApri.EnableWindow(0);

	if (pDoc->m_tabella == _T("[anime]"))
		m_dlgstato.SetCurSel(0);
	else
		m_dlgstato.SetCurSel(1);
}

// Copia nella cliboard il contenuto della casella sito
void CAggMod::OnClickCopiaURL()
{
	CString url = _T("");
	m_dlgsito.GetWindowTextW(url);

	if (url == _T(""))
		return;

	if (!OpenClipboard())
		return;

	if (!EmptyClipboard())
		return;

	size_t size = sizeof(TCHAR)*(1 + url.GetLength());
	HGLOBAL hResult = GlobalAlloc(GMEM_MOVEABLE, size);
	LPTSTR copia = (LPTSTR)GlobalLock(hResult);

	memcpy(copia, url.GetBuffer(), size);
	GlobalUnlock(hResult);

#ifndef _UNICODE
	if (::SetClipboardData(CF_TEXT, hResult) == NULL)
#else
	if (::SetClipboardData(CF_UNICODETEXT, hResult) == NULL)
#endif
	{
		GlobalFree(hResult);
		CloseClipboard();
		return;
	}
	CloseClipboard();
}

// Ridefinisco la OnOk, lasciando vuoto in modo che non chiuda
// le finestre di dialogo alla pressione del tasto Invio
void CAggMod::OnOK() { return; }

// Ridefinisco la OnCancel, lasciando vuoto in modo che non chiuda
// le finestre di dialogo alla pressione del tasto Esc
void CAggMod::OnCancel() { return; }