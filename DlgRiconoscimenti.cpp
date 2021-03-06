// DlgRiconoscimenti.cpp: file di implementazione
//

#include "stdafx.h"
#include "Note Series.h"
#include "DlgRiconoscimenti.h"
#include "Note SeriesView.h"
#include "afxdialogex.h"


// Finestra di dialogo DlgRiconoscimenti

IMPLEMENT_DYNAMIC(DlgRiconoscimenti, CDialogEx)

DlgRiconoscimenti::DlgRiconoscimenti(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RICONOSCIMENTI, pParent)
{
	retVal = _T("");
}

DlgRiconoscimenti::~DlgRiconoscimenti()
{
}

void DlgRiconoscimenti::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTA, m_list);
	DDX_Control(pDX, IDC_DESCR, m_descr);
}


BEGIN_MESSAGE_MAP(DlgRiconoscimenti, CDialogEx)
END_MESSAGE_MAP()


// Gestori di messaggi di DlgRiconoscimenti


BOOL DlgRiconoscimenti::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	CString arr[9] =
	{
		L"Federico Barbero",
		L"Alberto Visconti",
		L"Daniela Protti",
		L"Fabio Santoro",
		L"Giorgio Ferrè",
		L"Lorenzo Gasperina",
		L"Matteo Macchi",
		L"Marianna Ciravolo"
	};
	
	for (short i = 0; i < 9; i++)
		m_list.AddString(arr[i]);
	
	m_descr.SetWindowTextW(m_currlang.GetDesc(136, retVal));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ECCEZIONE: le pagine delle proprietà OCX devono restituire FALSE
}
