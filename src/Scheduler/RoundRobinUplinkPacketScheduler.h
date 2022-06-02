#ifndef ROUNDROBINUPLINKPAKETSCHEDULER_H_
#define ROUNDROBINUPLINKPAKETSCHEDULER_H_

#include "UplinkPacketScheduler.h"

class RoundRobinUplinkPacketScheduler : public UplinkPacketScheduler
{

public:

	RoundRobinUplinkPacketScheduler();
	virtual ~RoundRobinUplinkPacketScheduler();

	void RBsAllocation();

private:

	int m_roundRobinId;

};

#endif