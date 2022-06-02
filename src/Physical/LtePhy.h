#ifndef LTEPHY_H_
#define LTEPHY_H_

class NetworkNode;
class LteChannel;
class IdealControlMessage;
class BandwidthManager;
class TransmittedSignal;
class LteChannel;
class PacketBurst;
class ErrorModel;
class PowerManager;

class LtePhy
{

public:

	LtePhy();
	virtual ~LtePhy();

	void Destroy (void);
	
	virtual void StartTx(PacketBurst*, int mcs = -1) = 0;
	virtual void StartRx(PacketBurst*, TransmittedSignal*, int mcs = -1) = 0;

	NetworkNode* GetDevice(void);
	void SetDevice(NetworkNode*);

	void SetDlChannel(LteChannel*);
	LteChannel* GetDlChannel(void);
	void SetUlChannel(LteChannel*);
	LteChannel* GetUlChannel(void);
	
	void SetBandwidthManager(BandwidthManager*);
	BandwidthManager* GetBandwidthManager(void);
	virtual void DoSetBandwidthManager(void) = 0;

	void SetTxSignal(TransmittedSignal*);
	TransmittedSignal* GetTxSignal(void);
	
	virtual void SendIdealControlMessage(IdealControlMessage*) = 0;
	virtual void ReceiveIdealControlMessage(IdealControlMessage*) = 0;
	
	void SetErrorModel(ErrorModel*);
	ErrorModel* GetErrorModel(void);

	PowerManager* GetPowerManager(void);
	void SetPowerManager(PowerManager*);
	
private:
	
    NetworkNode* m_device;
    LteChannel* m_dlChannel;
    LteChannel* m_ulChannel;

	BandwidthManager* m_bandwidthManager;

    TransmittedSignal* m_txSignal;

    ErrorModel* m_errorModel;

	PowerManager* m_powerManager;

};

#endif
