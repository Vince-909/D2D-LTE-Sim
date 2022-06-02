#ifndef UEMACENTITY_H_
#define UEMACENTITY_H_

#include "MacEntity.h"

class UeMacEntity : public MacEntity
{

public:
	
	UeMacEntity();
	virtual ~UeMacEntity();

	void SendSchedulingRequest(void);
	void ScheduleUplinkTransmission(int, int);

	double GetSchedulingRequestInterval(void);
	void SetSchedulingRequestInterval(double);

private:

	double m_schedulingRequestInterval; //seconds

};

#endif