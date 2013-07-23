#pragma once

#include "Info.h"

class CStateInfo: public CObArray
{
	
protected:
	CString m_szStateName;
	CString m_szStateShortName;
	int m_StateNumber;
public:

	CString GetStateName(void);
	int GetStateNumber(void);
	CString GetStateShortName(void);

	void SetStateInfo( CString szStateName, CString szStateShortName, int iStateNumber);

	CStateInfo();
	~CStateInfo();

	void Serialize(CArchive& ar);
};

