#pragma once

#include "Info.h"
//#include "DistributionInfo.h"
//
//#include <boost/numeric/ublas/vector.hpp>

class CStateInfo;
class CDistributionInfo;

class CTransitionInfo : public CObArray
{

protected:
	CStateInfo* m_pFromState;
	CStateInfo* m_pToState;
	CString m_TranstionLabel;

public:

	//DECLARE_DYNAMIC ( CTransitionInfo )

protected:
	//std::vector<CDistributionInfo> m_DistributionList;
	CTypedPtrArray<CObArray, CDistributionInfo*> m_DistributionList;

public:
	CTransitionInfo();
	CTransitionInfo(CStateInfo* pFromState, CStateInfo* pToState, CString transitionLabel);
	~CTransitionInfo();

	void UpdateTransition(CStateInfo* pFromState, CStateInfo* pToState, CString transitionLabel);
	void AddDistribution(double mean, double cov, int distributionType);

	int GetDistributionCount(void);
	CDistributionInfo* GetDistribution(int i);

	CStateInfo* GetFromState(void);
	CStateInfo* GetToState(void);
	CString GetTransitionLabel(void);

	void SetTransitionStates(CStateInfo* pFromState, CStateInfo* ToState);

	void Serialize(CArchive& ar, int* iFromState, int* iToState);

//	CTransitionInfo(CTransitionInfo&);
//	void operator=(CTransitionInfo&);



};