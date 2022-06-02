#ifndef AMCMODULE_H_
#define AMCMODULE_H_

#include <vector>

class AmcModule
{

public:

	AmcModule();
	virtual ~AmcModule();

	double GetSinrFromCQI (int);
	int GetMCSFromCQI(int);
	int GetCQIFromSinr(double);
	double GetEfficiencyFromCQI(int);
	int GetTBSizeFromMCS(int, int);
	int GetTBSizeFromMCS (int);
	int GetCQIFromMCS(int);

	int CreateCqiFeedback(double);

};

#endif