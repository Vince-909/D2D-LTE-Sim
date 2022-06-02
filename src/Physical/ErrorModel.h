#ifndef ERRORMODEL_H_
#define ERRORMODEL_H_

#include <vector>

class ErrorModel
{

public:

	ErrorModel();
	virtual ~ErrorModel();

	virtual bool CheckForPhysicalError(std::vector<int>, std::vector<int>, std::vector<double>) = 0;

};

#endif