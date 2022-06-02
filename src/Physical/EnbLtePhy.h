#ifndef ENBLTEPHY_H_
#define ENBLTEPHY_H_

#include "LtePhy.h"

class IdealControlMessage;
class NetworkNode;
class TransmittedSignal;
class PacketBurst;

class EnbLtePhy : public LtePhy
{

public:

	EnbLtePhy();
	~EnbLtePhy();

	virtual void DoSetBandwidthManager(void);

	virtual void StartTx(PacketBurst*, int);
	virtual void StartRx(PacketBurst*, TransmittedSignal*, int);

	virtual void SendIdealControlMessage(IdealControlMessage*);
	virtual void ReceiveIdealControlMessage(IdealControlMessage*);

	void ReceiveReferenceSymbols(NetworkNode*, TransmittedSignal*);

};

#endif