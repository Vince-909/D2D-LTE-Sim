#ifndef DOWNLINKPACKETSCHEDULER_H_
#define DOWNLINKPACKETSCHEDULER_H_

#include "PacketScheduler.h"

class DownlinkPacketScheduler : public PacketScheduler
{

public:

	DownlinkPacketScheduler();
	virtual ~DownlinkPacketScheduler();

	virtual void DoSchedule(void);
	virtual void DoStopSchedule(void);

	void SelectFlowsToSchedule(void);

	void UpdateAverageTransmissionRate(void);

	virtual void RBsAllocation();

	virtual double ComputeSchedulingMetric(RadioBearer*, double, int) = 0;

private:

};

#endif