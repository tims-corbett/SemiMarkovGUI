#pragma once

#include "TransitionInfo.h"

class CStateInfo;
class JaggedMatrix;
class CStateCollection;

class CTransitionCollection
{
	CTypedPtrArray<CObArray, CTransitionInfo*> m_TransitionList;
	CStateCollection* m_pStateCollection;

public:
	CTransitionCollection(void);
	~CTransitionCollection(void);

	CTransitionInfo* GetTransition(CStateInfo* pFromState, CStateInfo* pToState);
	CTransitionInfo* GetNewTransition(CStateInfo* pFromState, CStateInfo* pToState, CString transitionLabel);

	CTransitionInfo* GetTransition(int i);
	int GetTransitionCount(void);
	JaggedMatrix* GetJaggedMatrix(void);
	
	void SetStateList(CStateCollection* pStateCollection);
	CStateCollection* GetStateList(void);
	void Serialize(CArchive& ar);
	void ClearCollection(void);

};

