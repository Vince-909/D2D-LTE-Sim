#ifndef UELTEPHY_H_
#define UELTEPHY_H_

#include <vector>
#include "LtePhy.h"

class IdealControlMessage;
class TransmittedSignal;
class PacketBurst;

class UeLtePhy : public LtePhy
{

public:

	UeLtePhy();
	~UeLtePhy();

	virtual void DoSetBandwidthManager(void);

	virtual void StartTx(PacketBurst*, int);
	virtual void StartRx(PacketBurst*, TransmittedSignal*, int);

	void CreateCqiFeedbacks (std::vector<double> sinr);

	virtual void SendIdealControlMessage(IdealControlMessage*);
	virtual void ReceiveIdealControlMessage(IdealControlMessage*);

	std::vector<int> GetChannelsForTx(void);
	void SetChannelsForTx(int);

	std::vector<int> GetChannelsForRx(void);
	void SetChannelsForRx(std::vector<int>*);

	std::vector<int> GetMcsIndexForTx(void);

	std::vector<int> GetMcsIndexForRx(void);
	void SetMcsIndexForRx(std::vector<int>*);

private:

	std::vector<double> m_measuredSinr;
	TransmittedSignal* m_txSignalForRerferenceSymbols;

	std::vector<int> m_channelsForTx;
	std::vector<int> m_mcsIndexForTx;

	std::vector<int> m_channelsForRx;
	std::vector<int> m_mcsIndexForRx;

};

#endif