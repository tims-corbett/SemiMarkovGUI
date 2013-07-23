#pragma once
#include "afx.h"


class CInfo :
	public CObject
{
public:
	DECLARE_DYNAMIC ( CInfo )

	CInfo(void);
	~CInfo(void);
	
	static enum InfoType { iStateInfo, iTransitionInfo, iDistributionInfo };

	CInfo(InfoType itInfoType);
	CInfo(const CInfo&);
	void operator=(const CInfo&);

	InfoType m_InfoType;
};

