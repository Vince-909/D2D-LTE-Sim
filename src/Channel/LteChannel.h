#ifndef LTECHANNEL_H_
#define LTECHANNEL_H_

#include <vector>

class PropagationLossModel;
class NetworkNode;
class PacketBurst;
class TransmittedSignal;

class LteChannel
{
	
public:

	LteChannel();
	~LteChannel();

    void StartTx(PacketBurst*, TransmittedSignal*, NetworkNode*, int mcs = -1);
    void StartRx(PacketBurst*, TransmittedSignal*, NetworkNode*, int mcs = -1);

    void AddDevice(NetworkNode*);
    void DelDevice(NetworkNode*);
    bool IsAttached(NetworkNode*);

    std::vector<NetworkNode*>* GetDevices(void);

    void SetPropagationLossModel(PropagationLossModel*);
    PropagationLossModel* GetPropagationLossModel(void);

    void SetChannelId(int);
    int GetChannelId(void);

private:

	std::vector<NetworkNode*>* m_attachedDevices;
    PropagationLossModel* m_propagationLossModel;
    int m_channelId;

};

#endif
