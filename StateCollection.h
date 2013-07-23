#pragma once

//#include "StateInfo.h"
class CStateInfo;

#include <vector> //for std::vector

class CStateCollection
{
protected:
	CTypedPtrArray<CObArray, CStateInfo*> m_lstStateInfoList;
public:
	CStateCollection(void);
	~CStateCollection(void);

	void AddState(CStateInfo* oStateInfo);
	std::vector<std::string> GetStateNames(void);
	int GetStateCount(void);
	CStateInfo* GetState(int i);
	void Serialize(CArchive& ar);
	void ClearCollection(void);

};

