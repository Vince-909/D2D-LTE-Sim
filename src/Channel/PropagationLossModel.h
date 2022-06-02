#ifndef PROPAGATIONLOSSMODEL_H_
#define PROPAGATIONLOSSMODEL_H_

#include <map>
#include <utility>

class TransmittedSignal;
class NetworkNode;
class ChannelRealization;

class PropagationLossModel
{

public:

	PropagationLossModel();
	virtual ~PropagationLossModel();

	typedef std::pair<NetworkNode*, NetworkNode*> ChannelRealizationId_t;
	typedef std::map <PropagationLossModel::ChannelRealizationId_t, ChannelRealization*> ChannelRealizationMap;

	void AddChannelRealization(ChannelRealization*);
	void DelChannelRealization(NetworkNode*, NetworkNode*);

	ChannelRealization* GetChannelRealization (NetworkNode*, NetworkNode*);
	ChannelRealizationMap GetChannelRealizationMap(void);

	TransmittedSignal* AddLossModel(NetworkNode*, NetworkNode*, TransmittedSignal*);

private:

	ChannelRealizationMap m_channelRealizationMap;

};

#endif