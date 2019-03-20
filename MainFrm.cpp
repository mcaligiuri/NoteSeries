// MainFrm.cpp: implementazione della classe CMainFrame
//

#include "stdafx.h"
#include "Note Series.h"

#include "MainFrm.h"
#include "LeftView.h"
#include "Note SeriesView.h"
#include "CMyRibbonPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// Costruzione/distruzione di CMainFrame

CMainFrame::CMainFrame()
{
	CMFCRibbonButton* m_btnopen = nullptr;
	CMFCRibbonButton* m_btnsave = nullptr;
	CMFCRibbonButton* m_btnprint = nullptr;
	CMFCRibbonButton* m_btninfo = nullptr;
	CMFCRibbonButton* m_btnstat = nullptr;
	CMFCRibbonButton* m_btnexce = nullptr;
	CMFCRibbonButton* m_setting = nullptr;

	CMFCRibbonButton* m_mnuexce = nullptr;
	CMFCRibbonButton* m_mnuopen = nullptr;
	CMFCRibbonButton* m_mnusave = nullptr;
	CMFCRibbonButton* m_mnuprint = nullptr;
	CMFCRibbonButton* m_mnuset = nullptr;
	CMFCRibbonButton* m_mnuexit =nullptr;
}

CMainFrame::~CMainFrame()
{
	
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BOOL bNameValid;
	
	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Non è stato possibile creare la barra di stato.\n");
		return -1;      // impossibile creare
	}

	CString strTitlePane1;
	CString strTitlePane2;
	CString retUser;
	CRegKey chiave;
	ULONG len = 255;

	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings")))
	{
		chiave.QueryStringValue((_T("Utente")), retUser.GetBufferSetLength(len), &len);
		strTitlePane1 = retUser;
		retUser.ReleaseBuffer();
	}
	ASSERT(retUser);

	/*bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);*/
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), _T("User"));

	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings")))
	{
		chiave.QueryStringValue((_T("Società")), retUser.GetBufferSetLength(len), &len);
		strTitlePane2 = retUser;
		retUser.ReleaseBuffer();
	}
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), _T("Società"));
	// abilitare comportamento finestra di ancoraggio stile Visual Studio 2005.
	// CDockingManager::SetDockingMode(DT_SMART);
	// abilitare comportamento Nascondi automaticamente finestra di ancoraggio stile Visual Studio 2005.
	//EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// impostare il gestore visualizzazione utilizzato per disegnare tutti gli elementi dell'interfaccia utente.
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
	//CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));

	// impostare lo stile di visualizzazione utilizzato dal gestore visualizzazione.
	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
	
	// Barra della ribbon
	m_btnopen = (CMFCRibbonButton *)m_wndRibbonBar.FindByID(ID_PR_OPEN);
	m_btnsave = (CMFCRibbonButton *)m_wndRibbonBar.FindByID(ID_SALVA_COME);
	m_btnprint = (CMFCRibbonButton *)m_wndRibbonBar.FindByID(ID_STAMPA);
	m_btninfo = (CMFCRibbonButton *)m_wndRibbonBar.FindByID(ID_APP_ABOUT);
	m_btnstat = (CMFCRibbonButton *)m_wndRibbonBar.FindByID(ID_VIEW_STATUS_BAR);
	m_btnexce = (CMFCRibbonButton *)m_wndRibbonBar.FindByID(ID_FILE_EXPORT_EXCEL);
	m_setting = (CMFCRibbonButton *)m_wndRibbonBar.FindByID(ID_FILE_OPTIONS);
	// Menù ribbon
	m_mnuexce = (CMFCRibbonButton *)m_wndRibbonBar.FindByID(ID_FILE_EXPORT_EXCEL2);
	m_mnuopen = (CMFCRibbonButton *)m_wndRibbonBar.FindByID(ID_PR_OPEN2);
	m_mnusave = (CMFCRibbonButton *)m_wndRibbonBar.FindByID(ID_SALVA_COME2);
	m_mnuprint = (CMFCRibbonButton *)m_wndRibbonBar.FindByID(ID_STAMPA2);
	m_mnuset = (CMFCRibbonButton *)m_wndRibbonBar.FindByID(ID_OPTIONS2);
	m_mnuexit = (CMFCRibbonButton *)m_wndRibbonBar.FindByID(ID_APP_EXIT);

	m_wndRibbonBar.EnableToolTips(TRUE);
	GetDescUpdate();
	return 0;
}
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// creare finestra con barra di divisione.
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(300, 300), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CNoteSeriesView), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	//SetThreadLocale(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));
	/*ID_Lingua = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
	ID_StringTable = FindResourceEx(NULL, RT_STRING, str, ID_Lingua);
	LoadResource(hInstance, ID_StringTable);*/
	m_MainButton.SetImage(IDB_CAGATA);
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: modificare la classe o gli stili Window modificando 
	//  la struttura CREATESTRUCT
	cs.style &= ~(LONG)FWS_ADDTOTITLE;
	
	return CFrameWnd::PreCreateWindow(cs);
}

// Diagnostica di CMainFrame

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG
// Gestori di messaggi di CMainFrame

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	//if (pMsg->message == WM_RBUTTONDOWN)
		//return true;
	return CFrameWndEx::PreTranslateMessage(pMsg);
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	CString titolo = _T("");
	titolo.Format(AFX_IDS_APP_TITLE);

	CFrameWndEx::OnUpdateFrameTitle(bAddToTitle);
}

void CMainFrame::GetDescUpdate()
{
	CString temp = _T("");

	// Array di puntatori a pulsanti sulla ribbon e array con gli id delle descrizioni da assegnare
	CMFCRibbonButton *pBtn[7] = { m_btnopen,m_btnsave,m_btnprint,m_btnexce,m_btnstat,m_btninfo,m_setting};
	CMFCRibbonButton *pMnu[4] = { m_mnuopen,m_mnusave,m_mnuprint,m_mnuexce };
	int des[7] = { 156,157,158,159,160,161,162 };
	int tip[7] = { 164,165,166,167,168,169,170 };
	for (int i = 0; i < 7; i++)
	{
		pBtn[i]->SetText(m_currlang.GetDesc(des[i], temp));
		pBtn[i]->SetToolTipText(m_currlang.GetDesc(tip[i], temp));
	}
	for (int j = 0; j < 4; j++)
	{
		pMnu[j]->SetText(m_currlang.GetDesc(des[j], temp));
		pMnu[j]->SetToolTipText(m_currlang.GetDesc(tip[j], temp));
	}
	m_mnuset->SetText(m_currlang.GetDesc(162, temp));
	m_mnuset->SetToolTipText(m_currlang.GetDesc(170, temp));
	m_mnuexit->SetText(m_currlang.GetDesc(87, temp));
	m_mnuexit->SetToolTipText(m_currlang.GetDesc(171, temp));

	// Accedo al pannello visualizza e ne cambio la descrizione
	CMyRibbonPanel* m_ribbonPanel = new CMyRibbonPanel();
	m_ribbonPanel = static_cast<CMyRibbonPanel*>(m_wndRibbonBar.GetCategory(0)->GetPanel(1));
	m_ribbonPanel->SetName(m_currlang.GetDesc(163, temp));
}
