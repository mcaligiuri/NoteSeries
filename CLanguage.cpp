#include "stdafx.h"
#include "CLanguage.h"

CLanguage m_currlang;
extern "C" _declspec(dllimport) CString SetDesc(int iddescr);
extern "C" _declspec(dllimport) CString SetDescEng(int iddescr);

CLanguage::CLanguage() {
	retStr =_T("");
	len = 0;
}


CLanguage::~CLanguage()
{
}

// A seconda della chiave di registro prendo la traduzione
CString CLanguage::GetDesc(int iddescr, CString temp) {
	CRegKey chiave;
	len = 10;
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings"))) {
		chiave.QueryStringValue(_T("Lingua"), retStr.GetBufferSetLength(len), &len);
		retStr.ReleaseBuffer();
		if (retStr == _T("ITALIANO"))
			temp =  SetDesc(iddescr);
		else
			temp = SetDescEng(iddescr);
	}
	return temp;
}