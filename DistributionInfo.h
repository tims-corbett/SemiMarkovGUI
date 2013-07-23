#pragma once

#include "Info.h"

class CDistributionInfo : public CObArray
{
protected:
	int m_DistType;
	double m_mean;
	double m_cov;

public:

	//DECLARE_DYNAMIC ( CDistributionInfo )

	CDistributionInfo(double mean, double cov, int  iDistType );
	int GetDistributionType(void);
	~CDistributionInfo(void);
	void UpdateDistribution(double mean, double cov, int iDistType );

	double GetMean(void);
	double GetCOV(void);

	void Serialize(CArchive& ar);

	CDistributionInfo(void);
	//CDistributionInfo(const CDistributionInfo&);
	//void operator=(const CDistributionInfo&);
};

