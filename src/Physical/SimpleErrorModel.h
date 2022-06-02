#ifndef SIMPLEERRORMODEL_H_
#define SIMPLEERRORMODEL_H_

#include "ErrorModel.h"
#include <vector>

class SimpleErrorModel : public ErrorModel
{

public:

	SimpleErrorModel();
	virtual ~SimpleErrorModel();

	virtual bool CheckForPhysicalError(std::vector<int>, std::vector<int>, std::vector<double>);

};

#endif