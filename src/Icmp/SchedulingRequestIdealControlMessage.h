#ifndef SCHEDULINGREQUESTIDEALCONTROLMESSAGE_H_
#define SCHEDULINGREQUESTIDEALCONTROLMESSAGE_H_

#include "IdealControlMessage.h"

class SchedulingRequestIdealControlMessage : public IdealControlMessage
{

public:

	SchedulingRequestIdealControlMessage();
	~SchedulingRequestIdealControlMessage();

	int GetBufferStatusReport(void);
	void SetBufferStatusReport(int);

private:

	int m_bufferStatusReport;

};

#endif