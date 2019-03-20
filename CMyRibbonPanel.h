// Classe derivata da CMFCRibbonPanel per accedere ad attributi protetti di quella classe
#pragma once
#include <afxribbonpanel.h>
class CMyRibbonPanel :
	public CMFCRibbonPanel
{
public:
	CMyRibbonPanel();
	~CMyRibbonPanel();
	// Funzione che modifica l'attributo protetto "m_strName"
	void SetName(CString& name);
};

