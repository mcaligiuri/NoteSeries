// DlgCartella.cpp: file di implementazione
//

#include "stdafx.h"
#include "Note Series.h"
#include "DlgCartella.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "Note SeriesDoc.h"
#include "Note SeriesView.h"

// Finestra di dialogo DlgCartella

IMPLEMENT_DYNAMIC(DlgCartella, CDialogEx)

DlgCartella::DlgCartella(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FOLDER, pParent)
{
	len = 255;
	m_sel = 0;
	retUser = _T("");
	m_nome = _T("");
	m_path = _T("");
	m_relPath = _T("");
}

DlgCartella::~DlgCartella()
{
}

void DlgCartella::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCREEN, m_screen);
	DDX_Control(pDX, IDC_BTNDEL, m_btnDel);
	DDX_Control(pDX, IDC_BTNAPRI, m_btnApri);
	DDX_Control(pDX, IDC_TITLE, m_lblTitle);
	DDX_Control(pDX, IDC_TXTNAME, m_txtName);
	DDX_Control(pDX, IDC_BTNREN, m_btnRen);
	DDX_Control(pDX, IDOK, m_btnClose);
	DDX_Control(pDX, IDC_TITLE, m_lblTitle);
	DDX_Control(pDX, IDC_LBLSIZE, m_lblSize);
	DDX_Control(pDX, IDC_LBLNOME, m_lblNome);
	DDX_Control(pDX, IDC_LBLCREATE, m_lblCreate);
	DDX_Control(pDX, IDC_LBLMOD, m_lblMod);
	DDX_Control(pDX, IDC_TXTSIZE, m_txtSize);
	DDX_Control(pDX, IDC_TXTMOD, m_txtMod);
	DDX_Control(pDX, IDC_TXTCREA, m_txtCrea);
	DDX_Control(pDX, IDC_GPR, m_gprDati);
}


BEGIN_MESSAGE_MAP(DlgCartella, CDialogEx)
	ON_BN_CLICKED(IDC_BTNDEL, &DlgCartella::EliminaElemento)
	ON_BN_CLICKED(IDC_BTNAPRI, &DlgCartella::ApriFile)
	ON_LBN_SELCHANGE(IDC_SCREEN, &DlgCartella::OnLbnSelchangeScreen)
	ON_LBN_DBLCLK(IDC_SCREEN, &DlgCartella::DoppioClickElem)
	ON_BN_CLICKED(IDC_BTNREN, &DlgCartella::RinominaFile)
END_MESSAGE_MAP()


// Gestori di messaggi di DlgCartella


BOOL DlgCartella::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Traduzioni finestra di dialogo
	GetDescUpdate();
	// Controlli disabilitati perchè
	// non è selezionato nulla
	m_btnApri.EnableWindow(0);
	m_btnDel.EnableWindow(0);
	m_txtName.EnableWindow(0);
	m_btnRen.EnableWindow(0);
	
	// Dimensioni caratteri ListBox
	CFont *m_pFont = new CFont();
	m_pFont->CreatePointFont(110, _T("Verdana"));
	m_screen.SetFont(m_pFont, TRUE);
	m_lblTitle.SetFont(m_pFont, TRUE);

	CaricaContenuto(); // Scandisco cartella
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ECCEZIONE: le pagine delle proprietà OCX devono restituire FALSE
}

void DlgCartella::GetDescUpdate()
{
	CString retVal = _T("");
	m_btnDel.SetWindowTextW(m_currlang.GetDesc(42,retVal));
	m_btnApri.SetWindowTextW(m_currlang.GetDesc(84,retVal));
	m_btnRen.SetWindowTextW(m_currlang.GetDesc(85,retVal));
	m_btnClose.SetWindowTextW(m_currlang.GetDesc(86, retVal));
	m_lblTitle.SetWindowTextW(m_currlang.GetDesc(87, retVal));
	m_gprDati.SetWindowTextW(m_currlang.GetDesc(92, retVal));
	m_lblCreate.SetWindowTextW(m_currlang.GetDesc(93, retVal));
	m_lblMod.SetWindowTextW(m_currlang.GetDesc(94, retVal));
	m_lblSize.SetWindowTextW(m_currlang.GetDesc(95, retVal));
	m_lblNome.SetWindowTextW(m_currlang.GetDesc(37, retVal));
}
// Carica il contenuto della cartella
void DlgCartella::CaricaContenuto()
{
	m_screen.ResetContent(); // Pulizia ListBox
	WIN32_FIND_DATA data;
	// Puntatore mainFrame
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	// Puntatore vista principale
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();
	// Puntatore documento
	CNoteSeriesDoc* pDoc = pView->GetDocument();

	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
	{
		chiave.QueryStringValue((_T("Cartella")), retUser.GetBufferSetLength(len), &len);
		m_path.Format(_T("%s\\%s\\*"), retUser, pDoc->m_cartella); // Percorso cartella

		HANDLE cartella = FindFirstFile(m_path, &data);

		if (cartella != INVALID_HANDLE_VALUE)
		{
			// Scandisco il contenuto della cartella e lo mostro nella ListBox
			while (FindNextFile(cartella, &data))
			{
				data.nFileSizeLow;
				m_nome.Format(_T("%s"), data.cFileName);
				if (m_nome != _T(".") && m_nome != _T("..") && m_nome != _T("desktop.ini"))
					m_screen.InsertString(NULL, m_nome);
			}
			FindClose(cartella);
		}
		retUser.ReleaseBuffer();
	}
}

void DlgCartella::GetPercorso()
{
	// Puntatore mainFrame
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	// Puntatore vista principale
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();
	// Puntatore documento
	CNoteSeriesDoc* pDoc = pView->GetDocument();
	// Nome elemento selezionato
	CString elem = _T("");
	
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
	{
		chiave.QueryStringValue((_T("Cartella")), retUser.GetBufferSetLength(len), &len);
		
		// Selezione corrente nella ListBox
		m_sel = m_screen.GetCurSel();
		// Prendo il testo della selezione corrente
		m_screen.GetText(m_sel, elem);
		// Ricavo il percorso dell'elemento selezionato
		m_path.Format(_T("%s\\%s\\%s"), retUser, pDoc->m_cartella, elem);
		// Ricavo il percorso precedente all' elemento selezionato
		m_relPath.Format(_T("%s\\%s"), retUser, pDoc->m_cartella);
		// Inserisco percorsi nel CArray
		m_pathArray.Add(m_path);
		m_pathArray.Add(m_relPath);
		retUser.ReleaseBuffer();
	}
}

// Elimino elemento da cartella associata a una serie
void DlgCartella::EliminaElemento()
{
	CString temp = _T("");
	// Chiedo conferma dell'eliminazione
	if (!(AfxMessageBox((m_currlang.GetDesc(88,temp)),MB_YESNO) == IDYES))
		return;
	CString path = _T("");
	GetPercorso();
	path.Append(m_pathArray.GetAt(0));
	if (!DeleteFile(path))
	{
		AfxMessageBox((m_currlang.GetDesc(89, temp)));
		return;
	}
	m_pathArray.RemoveAll();
	m_txtName.SetWindowTextW(L"");
	
	m_txtName.EnableWindow(0);
	m_btnRen.EnableWindow(0);
	m_btnDel.EnableWindow(0);
	m_btnApri.EnableWindow(0);
	m_txtCrea.SetWindowTextW(_T(""));
	m_txtMod.SetWindowTextW(_T(""));
	m_txtSize.SetWindowTextW(_T(""));
	CaricaContenuto();
}

// Tento di aprire il file selezionato nella ListBox
void DlgCartella::ApriFile()
{
	CString path,temp = _T("");
	GetPercorso();
	path.Append(m_pathArray.GetAt(0));
	if (!ShellExecute(NULL, L"open", path, NULL, NULL, SW_SHOWMAXIMIZED))
	{
		AfxMessageBox((m_currlang.GetDesc(90, temp)));
		return;
	}
	m_pathArray.RemoveAll();
}

// Al cambio selezione nella ListBox abilito il pulsanti
void DlgCartella::OnLbnSelchangeScreen()
{
	CString temp, dim,date,mod = _T("");
	CFileStatus attributi;
	float dimByte = 0;
	float dimMb = 0;
	m_sel = m_screen.GetCurSel();
	if ( m_sel == -1)
		return;
	
	// Se m_sel != 1 abilito i controlli
	m_btnApri.EnableWindow(1);
	m_btnDel.EnableWindow(1);
	m_txtName.EnableWindow(1);
	m_btnRen.EnableWindow(1);

	// Prendo il testo della selezione
	// e lo associo alla casella di testo
	m_screen.GetText(m_sel, temp);
	m_txtName.SetWindowTextW(temp);
	
	
	GetPercorso();
	if (CFile::GetStatus(m_pathArray[0], attributi))
	{
		dimByte = (float)attributi.m_size;
		dimMb = dimByte / (1024 * 1024);
		dim.Format(_T("%.1f MB (%.0f byte)"), dimMb, dimByte); // dimensioni
		date = attributi.m_ctime.Format("%x");	// data di creazione
		mod = attributi.m_mtime.Format("%x");	// data ultima modifica

		// Imposto contenuti caselle di testo
		m_txtSize.SetWindowText(dim);
		m_txtMod.SetWindowText(mod);
		m_txtCrea.SetWindowText(date);
		m_pathArray.RemoveAll();
	}
}

// Apro file al doppio click su selezione
void DlgCartella::DoppioClickElem()
{
	if (m_screen.GetCurSel() == -1)
		return;

	CString path,temp = _T("");
	GetPercorso();
	path.Append(m_pathArray.GetAt(0));
	if (!ShellExecute(NULL, L"open", path, NULL, NULL, SW_SHOWMAXIMIZED))
	{
		AfxMessageBox((m_currlang.GetDesc(90, temp)));
		return;
	}
	m_pathArray.RemoveAll();
}

void DlgCartella::RinominaFile()
{
	CString pathAss, pathRel, nome, error,temp = _T("");
	
	GetPercorso();
	// Percorso file
	pathAss.Append(m_pathArray.GetAt(0));
	// Percorso precedente al file
	pathRel.Append(m_pathArray.GetAt(1));
	// Nome nuovo per il file
	m_txtName.GetWindowText(nome);

	// Rinomino concatenando percorso precedente al nuovo nome
	if (_wrename(pathAss, pathRel + L"\\" + nome) != 0)
	{
		error.Format((m_currlang.GetDesc(91,temp)), errno);
		AfxMessageBox(error);
		return;
	}
	m_pathArray.RemoveAll();
	m_txtName.EnableWindow(0);
	m_btnRen.EnableWindow(0);
	m_btnDel.EnableWindow(0);
	m_btnApri.EnableWindow(0);
	m_txtCrea.SetWindowTextW(_T(""));
	m_txtMod.SetWindowTextW(_T(""));
	m_txtSize.SetWindowTextW(_T(""));
	m_txtName.SetWindowTextW(_T(""));
	CaricaContenuto();
}
