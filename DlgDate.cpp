// DlgDate.cpp: file di implementazione
//

#include "stdafx.h"
#include "Note Series.h"
#include "Note SeriesDoc.h"
#include "Note SeriesView.h"
#include "mainfrm.h"
#include "DlgDate.h"
#include "afxdialogex.h"


// Finestra di dialogo DlgDate

IMPLEMENT_DYNAMIC(DlgDate, CDialogEx)

DlgDate::DlgDate(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MODIFICA_DATE, pParent)
{
	m_mesi = _T("");
	m_anni = _T("");
	m_giorni = _T("");
	m_explode = _T("");
	m_data = _T("");
	m_dataF = _T("");
	m_pos = 0;
	pDoc = nullptr;
}

DlgDate::~DlgDate()
{
}

void DlgDate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMBGIORNO, m_cmbGiorno);
	DDX_Control(pDX, IDC_CMBMESE, m_cmbMese);
	DDX_Control(pDX, IDC_CMBANNO, m_cmbAnno);
	DDX_Control(pDX, IDC_CMBGIORNOF, m_cmbGiornoF);
	DDX_Control(pDX, IDC_CMBMESEF, m_cmbMeseF);
	DDX_Control(pDX, IDC_CMBANNOF, m_cmbAnnoF);
	DDX_Control(pDX, IDC_BTNSALVA, m_btnSalva);
	DDX_Control(pDX, IDC_LBLTITLE, m_lblTitle);
	DDX_Control(pDX, IDC_LBLINIZIO, m_lblInizio);
	DDX_Control(pDX, IDC_LBLFINE, m_lblFine);
	DDX_Control(pDX, IDC_LBLERROR, m_lblError);
}


BEGIN_MESSAGE_MAP(DlgDate, CDialogEx)
	ON_BN_CLICKED(IDC_BTNSALVA, &DlgDate::OnClickSalva)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_CMBMESE, &DlgDate::OnCbnSelchangeCmbmese)
	ON_CBN_SELCHANGE(IDC_CMBMESEF, &DlgDate::OnCbnSelchangeCmbmesef)
	ON_CBN_SELCHANGE(IDC_CMBANNO, &DlgDate::OnCbnSelchangeCmbanno)
	ON_CBN_SELCHANGE(IDC_CMBANNOF, &DlgDate::OnCbnSelchangeCmbannof)
END_MESSAGE_MAP()


// Gestori di messaggi di DlgDate


BOOL DlgDate::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	pDoc = GetDoc();
	// Popolo le combo dei giorni
	for (short i = 1; i < 32; i++)
	{
		if(i < 10) 
			m_giorni.Format(_T("0%d"), i);
		else
			m_giorni.Format(_T("%d"), i);

		m_cmbGiorno.AddString(m_giorni);
		m_cmbGiornoF.AddString(m_giorni);
	}

	// Popolo le combo dei mesi
	for (short m = 1; m < 13; m++)
	{
		if (m < 10)
			m_mesi.Format(_T("0%d"), m);
		else
			m_mesi.Format(_T("%d"), m);

		m_cmbMese.AddString(m_mesi);
		m_cmbMeseF.AddString(m_mesi);
	}

	// Popolo le combo degli anni
	long d = _wtoi(CTime::GetCurrentTime().Format("%Y"));	// Anno corrente in formato numerico
	for (short a = 2014; a < d + 1; a++)
	{
		m_anni.Format(_T("%d"),a);
		m_cmbAnno.AddString(m_anni);
		m_cmbAnnoF.AddString(m_anni);
	}

	if (pDoc->m_fine == _T("-"))
	{
		m_cmbGiornoF.EnableWindow(FALSE);
		m_cmbMeseF.EnableWindow(FALSE);
		m_cmbAnnoF.EnableWindow(FALSE);
	}
	else
	{
		m_cmbGiornoF.EnableWindow(TRUE);
		m_cmbMeseF.EnableWindow(TRUE);
		m_cmbAnnoF.EnableWindow(TRUE);
		CheckDataFine();
	}
	this->SetBackgroundColor(RGB(255, 255, 255)); // Sfondo bianco
	m_lblError.ShowWindow(SW_HIDE);				  // Nascondo label errore
	
	// Caratte del titolo
	CFont *m_SizeTitle = new CFont();
	m_SizeTitle->CreatePointFont(110, _T("Verdana"));
	m_lblTitle.SetFont(m_SizeTitle, TRUE);
	
	CheckDataInizio();
	
	// Traduzioni
	GetDescUpdate();
	
	return TRUE;
}

void DlgDate::GetDescUpdate()
{
	CString temp = _T("");
	m_lblTitle.SetWindowTextW(pDoc->m_date.GetAt(0) + L" " + pDoc->m_nome);
	m_lblInizio.SetWindowTextW(pDoc->m_date.GetAt(1));
	m_lblFine.SetWindowTextW(pDoc->m_date.GetAt(2));
	m_btnSalva.SetWindowTextW(pDoc->m_form.GetAt(9));
	m_lblError.SetWindowTextW(pDoc->m_date.GetAt(3));
}

// Ricavo puntatore al documento
CNoteSeriesDoc* DlgDate::GetDoc()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;				// Puntatore alla mainframe
	CNoteSeriesView* pView = (CNoteSeriesView*)pMain->GetActiveView();		// Puntatore alla vista principale
	CNoteSeriesDoc* pDoc = pView->GetDocument();							// Puntatore al documento
	return pDoc;
}
void DlgDate::CheckDataInizio()
{
	CString ris = _T("");
	m_pos = 0;
	int i = 0;
	
	// Esplodo data di inizio 
	m_explode = pDoc->m_inizio.Tokenize(_T("/"), m_pos);
	while (m_explode != _T(""))
	{
		ris.Format(_T("%s"), m_explode.GetString());
		i++;
		switch (i)
		{
		case 1:
			m_cmbGiorno.SelectString(0, ris);
			m_explode = pDoc->m_inizio.Tokenize(_T("/"), m_pos);
			break;
		case 2:
			m_cmbMese.SelectString(0, ris);
			m_explode = pDoc->m_inizio.Tokenize(_T("/"), m_pos);
			break;
		case 3:
			m_cmbAnno.SelectString(0, ris);
			m_explode = pDoc->m_inizio.Tokenize(_T("/"), m_pos);
			break;
		}
	}
}

void DlgDate::CheckDataFine()
{
	CString ris = _T("");
	int i = 0;
	m_pos = 0;
	
	// Esplodo data di fine
	m_explode = pDoc->m_fine.Tokenize(_T("/"), m_pos);
	while (m_explode != _T(""))
	{
		ris.Format(_T("%s"), m_explode.GetString());
		i++;
		switch (i)
		{
		case 1:
			m_cmbGiornoF.SelectString(0, ris);
			m_explode = pDoc->m_fine.Tokenize(_T("/"), m_pos);
			break;
		case 2:
			m_cmbMeseF.SelectString(0, ris);
			m_explode = pDoc->m_fine.Tokenize(_T("/"), m_pos);
			break;
		case 3:
			m_cmbAnnoF.SelectString(0, ris);
			m_explode = pDoc->m_fine.Tokenize(_T("/"), m_pos);
			break;
		}
	}
}

void DlgDate::OnClickSalva()
{
	CString msg, ndata, ndataF = _T("");
	CString giorno, mese, anno = _T("");
	CString temp, sql = _T("");

	// Costruisco nuova data di inizio
	m_cmbGiorno.GetWindowText(giorno);
	m_cmbMese.GetWindowText(mese);
	m_cmbAnno.GetWindowText(anno);
	m_data = giorno + L"/" + mese + L"/" + anno;
	if (pDoc->m_fine == L"-")
	{
		msg.Format(m_currlang.GetDesc(105,temp), pDoc->m_nome + L"'?\n", pDoc->m_inizio, m_data);
		if (!(AfxMessageBox(msg, MB_YESNO | MB_ICONINFORMATION) == IDYES))
			return;

		sql.Format(_T("UPDATE [anime] SET inizio='%s' WHERE nome='%s'"),m_data.GetString(), pDoc->m_nome.GetString());
	}
	else
	{
		CTime t1(_wtoi(anno), _wtoi(mese), _wtoi(giorno), 0, 0, 0); // Timestamp data di inizio
		
		// Costruisco la data di fine
		m_cmbGiornoF.GetWindowText(giorno);
		m_cmbMeseF.GetWindowText(mese);
		m_cmbAnnoF.GetWindowText(anno);
		m_dataF = giorno + L"/" + mese + L"/" + anno;
		
		CTime t2(_wtoi(anno), _wtoi(mese), _wtoi(giorno), 0, 0, 0); // Timestamp data di fine
		
		if (t2 < t1 /*|| t2 == t1*/)
		{
			m_lblError.ShowWindow(SW_SHOW);
			return;
		}
		
		m_lblError.ShowWindow(SW_HIDE);
		msg.Format(m_currlang.GetDesc(104,temp),pDoc->m_nome+L"'?\n", pDoc->m_inizio, m_data+L"\n", pDoc->m_fine,m_dataF+L"\n");
		
		if (!(AfxMessageBox(msg, MB_YESNO | MB_ICONINFORMATION) == IDYES))
			return;

		sql.Format(_T("UPDATE[anime] SET inizio = '%s', fine = '%s' WHERE nome = '%s'"),m_data.GetString(), m_dataF.GetString(), pDoc->m_nome.GetString());
	}
	dbconfig.SetSerie(sql);
	pDoc->UpdateAllViews(NULL);
}

// Scritta rossa per l'errore
HBRUSH DlgDate::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC && pWnd->GetSafeHwnd() == GetDlgItem(IDC_LBLERROR)->GetSafeHwnd())
	{
		pDC->SetTextColor(RGB(255, 0, 0));
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}

void DlgDate::OnCbnSelchangeCmbmese()
{
	short sel = m_cmbGiorno.GetCurSel();
	CString anno = _T("");
	int giorno = 0;
	switch (m_cmbMese.GetCurSel())
	{
	case 1: // Febbraio
		m_cmbAnno.GetWindowTextW(anno);
		m_cmbGiorno.ResetContent();
		if (GetAnnoBisestile(anno))
			giorno = 30;
		else
			giorno = 29;
		// Ripopolo le combo dei giorni
		for (short i = 1; i < giorno; i++)
		{
			if (i < 10)
				m_giorni.Format(_T("0%d"), i);
			else
				m_giorni.Format(_T("%d"), i);

			m_cmbGiorno.AddString(m_giorni);
		}
		m_cmbGiorno.SetCurSel(sel-1);
		break;
	// Aprile,giugno,settembre,novembre hanno 30 giorni
	case 3:
	case 5:
	case 8:
	case 10:
		m_cmbGiorno.ResetContent();
		// Ripopolo le combo dei giorni
		for (short i = 1; i < 31; i++)
		{
			if (i < 10)
				m_giorni.Format(_T("0%d"), i);
			else
				m_giorni.Format(_T("%d"), i);

			m_cmbGiorno.AddString(m_giorni);
		}
		break;
	default:
	{
		m_cmbGiorno.ResetContent();
		// Ripopolo le combo dei giorni
		for (short i = 1; i < 32; i++)
		{
			if (i < 10)
				m_giorni.Format(_T("0%d"), i);
			else
				m_giorni.Format(_T("%d"), i);

			m_cmbGiorno.AddString(m_giorni);
		}
	}
	}
	m_cmbGiorno.SetCurSel(sel);
}


void DlgDate::OnCbnSelchangeCmbmesef()
{
	short sel = m_cmbGiornoF.GetCurSel();
	CString anno = _T("");
	int giorno = 0;

	switch (m_cmbMeseF.GetCurSel())
	{
	case 1: // Febbraio
		m_cmbAnnoF.GetWindowTextW(anno);
		m_cmbGiornoF.ResetContent();
		if (GetAnnoBisestile(anno))
			giorno = 30;
		else
			giorno = 29;
		// Ripopolo le combo dei giorni
		for (short i = 1; i < giorno; i++)
		{
			if (i < 10)
				m_giorni.Format(_T("0%d"), i);
			else
				m_giorni.Format(_T("%d"), i);

			m_cmbGiornoF.AddString(m_giorni);
		}
		m_cmbGiornoF.SetCurSel(sel - 1);
		break;

		// Aprile,giugno,settembre,novembre hanno 30 giorni
	case 3:
	case 5:
	case 8:
	case 10:
		m_cmbGiornoF.ResetContent();
		// Ripopolo le combo dei giorni
		for (short i = 1; i < 31; i++)
		{
			if (i < 10)
				m_giorni.Format(_T("0%d"), i);
			else
				m_giorni.Format(_T("%d"), i);

			m_cmbGiornoF.AddString(m_giorni);
		}
		break;
	default:
		m_cmbGiornoF.ResetContent();
		// Ripopolo le combo dei giorni
		for (short i = 1; i < 32; i++)
		{
			if (i < 10)
				m_giorni.Format(_T("0%d"), i);
			else
				m_giorni.Format(_T("%d"), i);

			m_cmbGiornoF.AddString(m_giorni);
		}
	}
	m_cmbGiornoF.SetCurSel(sel);
}

// Controllo se un anno è bisestile
BOOL DlgDate::GetAnnoBisestile(CString anno)
{
	int cifre = _wtoi(anno.Right(2));
	if (cifre % 4 == 0)
		return TRUE;
	
	return FALSE;
}

void DlgDate::OnCbnSelchangeCmbanno()
{
	// Se l'anno è bisestile e sono su febbraio
	// metto 29 giorno
	short sel = m_cmbGiorno.GetCurSel();
	short giorni = 0;
	CString anno = _T("");
	m_cmbAnno.GetWindowTextW(anno);

	if (GetAnnoBisestile(anno) && m_cmbMese.GetCurSel() == 1)
		giorni = 30;
	else if ((!GetAnnoBisestile(anno)) && m_cmbMese.GetCurSel() == 1)
		giorni = 29;
	else
		giorni = 32;

	//m_cmbGiornoF.SetCurSel(sel - 1);
	m_cmbGiorno.ResetContent();
	// Ripopolo le combo dei giorni
	for (short i = 1; i < giorni; i++)
	{
		if (i < 10)
			m_giorni.Format(_T("0%d"), i);
		else
			m_giorni.Format(_T("%d"), i);

		m_cmbGiorno.AddString(m_giorni);
	}
	if (!m_cmbGiorno.SetCurSel(sel))
		m_cmbGiorno.SetCurSel(sel+1);
}


void DlgDate::OnCbnSelchangeCmbannof()
{
	short sel = m_cmbGiornoF.GetCurSel();
	short giorni = 0;
	CString annoF = _T("");
	m_cmbAnnoF.GetWindowTextW(annoF);
	if (GetAnnoBisestile(annoF) && m_cmbMeseF.GetCurSel() == 1)
		giorni = 30;

	else if ((!GetAnnoBisestile(annoF)) && m_cmbMeseF.GetCurSel() == 1)
		giorni = 29;

	else
		giorni = 32;
	
	m_cmbGiornoF.ResetContent();
	// Ripopolo le combo dei giorni
	for (short i = 1; i < giorni; i++)
	{
		if (i < 10)
			m_giorni.Format(_T("0%d"), i);
		else
			m_giorni.Format(_T("%d"), i);

		m_cmbGiornoF.AddString(m_giorni);
	}
	if (!m_cmbGiornoF.SetCurSel(sel))
		m_cmbGiornoF.SetCurSel(1);
}

// Ridefinisco la OnOk, lasciando vuoto in modo che non chiuda
// le finestre di dialogo alla pressione del tasto Invio
void DlgDate::OnOK() { return; }

// Ridefinisco la OnCancel, lasciando vuoto in modo che non chiuda
// le finestre di dialogo alla pressione del tasto Esc
void DlgDate::OnCancel() { return; }