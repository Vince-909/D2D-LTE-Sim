#ifndef PROPORTIONALFAIRDOWNLINKPACKETSCHEDULER_H_
#define PROPORTIONALFAIRDOWNLINKPACKETSCHEDULER_H_

#include "DownlinkPacketScheduler.h"

class ProportionalFairDownlinkPacketScheduler : public DownlinkPacketScheduler
{

public:

	ProportionalFairDownlinkPacketScheduler();
	virtual ~ProportionalFairDownlinkPacketScheduler();

	virtual double ComputeSchedulingMetric(RadioBearer*, double, int);

};

#endif