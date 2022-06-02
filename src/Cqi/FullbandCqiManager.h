#ifndef FULLBANDCQIMANAGER_H_
#define FULLBANDCQIMANAGER_H_

#include "CqiManager.h"
#include <vector>

class FullbandCqiManager : public CqiManager
{

public:

	FullbandCqiManager();
	virtual ~FullbandCqiManager();

	virtual void CreateCqiFeedbacks (std::vector<double>);

};

#endif