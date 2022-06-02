#ifndef ENBMACENTITY_H_
#define ENBMACENTITY_H_

#include "MacEntity.h"

class CqiIdealControlMessage;
class SchedulingRequestIdealControlMessage;
class QoeIdealControlMessage;
class PacketScheduler;

class EnbMacEntity : public MacEntity
{

public:
	
	EnbMacEntity();
	virtual ~EnbMacEntity();
	
	PacketScheduler* GetUplinkPacketScheduler(void);
	void SetUplinkPacketScheduler(PacketScheduler*);

	PacketScheduler* GetDownlinkPacketScheduler(void);
	void SetDownlinkPacketScheduler(PacketScheduler*);

	void ReceiveCqiIdealControlMessage(CqiIdealControlMessage*);
	void ReceiveSchedulingRequestIdealControlMessage(SchedulingRequestIdealControlMessage*);
	void ReceiveQoeIdealControlMessage(QoeIdealControlMessage*);

private:
	
	PacketScheduler* m_uplinkScheduler;
	PacketScheduler* m_downlinkScheduler;

};

#endif