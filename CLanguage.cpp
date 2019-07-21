#include "stdafx.h"
#include "CLanguage.h"

CLanguage m_currlang;
extern "C" _declspec(dllimport) CString SetDesc(int iddescr);
extern "C" _declspec(dllimport) CString SetDescEng(int iddescr);

CLanguage::CLanguage()
{
	retStr =_T("");
}


CLanguage::~CLanguage()
{
}

/*BOOL CLanguage::dbopen()
{
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
	{
		ULONG len = 255;
		chiave.QueryStringValue((_T("SharedMDB")), retUser.GetBufferSetLength(len), &len);
		m_dbfLang.Format(_T("%s/language.mdb"), retUser.GetString());
	}
	try
	{
		m_db.Format(_T("ODBC;DRIVER={%s};DSN='';DBQ=%s;PWD=%s"), _T("MICROSOFT ACCESS DRIVER (*.mdb)"), m_dbfLang.GetString(), m_dbPass.GetString());
		if (!language.Open(NULL, FALSE, FALSE, m_db))
			return FALSE;
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("Database error: ") + e->m_strError);
		return FALSE;
	}
	return TRUE;
}*/

// A seconda della chiave di registro prendo la traduzione
CString CLanguage::GetDesc(int iddescr, CString temp) {
	/*if (!dbopen())
	{
		AfxMessageBox(_T("Error: database not found in "+ retUser + " the program will be close."));
		PostQuitMessage(0);
	}
	if(!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings")))
	{
		chiave.QueryStringValue(_T("Lingua"), retStr.GetBufferSetLength(len), &len);
		retStr.ReleaseBuffer();
		if (retStr == _T("ITALIANO"))
		{
			sql.Format(_T("SELECT DESCR FROM [ITALIANO] WHERE IDDESCR = %d"), iddescr);
		}
		else
		{
			sql.Format(_T("SELECT DESCR FROM [ENGLISH] WHERE IDDESCR = %d"), iddescr);
		}
		try
		{
			CRecordset righe(&language);
			righe.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
			while (!righe.IsEOF())
			{
				righe.GetFieldValue(L"DESCR", temp);
				righe.MoveNext();
			}
			righe.Close();
			language.Close();
		}
		catch (CDBException* e)
		{
			AfxMessageBox(_T("Database error: ") + e->m_strError);
		}
	}
	return temp;*/
	CRegKey chiave;
	ULONG len = 10;
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings"))) {
		chiave.QueryStringValue(_T("Lingua"), retStr.GetBufferSetLength(len), &len);
		retStr.ReleaseBuffer();
		if (retStr == _T("ITALIANO"))
			return SetDesc(iddescr);

		else
			return SetDescEng(iddescr);
	}
}