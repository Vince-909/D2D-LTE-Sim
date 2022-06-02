#include "PropagationLossModel.h"
#include "../Spectrum/TransmittedSignal.h"
#include "ChannelRealization.h"
#include <iostream>
#include "../Device/NetworkNode.h"
#include <map>

PropagationLossModel::PropagationLossModel()
{
}

PropagationLossModel::~PropagationLossModel()
{
  m_channelRealizationMap.clear();
}

void PropagationLossModel::AddChannelRealization(ChannelRealization* chRealization)
{
  ChannelRealizationId_t idMap = std::make_pair(chRealization->GetSourceNode(), chRealization->GetDestinationNode());
  m_channelRealizationMap.insert(std::pair <ChannelRealizationId_t, ChannelRealization*> (idMap, chRealization));
}

void PropagationLossModel::DelChannelRealization(NetworkNode* src, NetworkNode* dst)
{
  ChannelRealizationId_t idMap = std::make_pair(src,dst);

  if (m_channelRealizationMap.find(idMap) != m_channelRealizationMap.end())
    {
	  m_channelRealizationMap.find(idMap)->second->Destroy();
      m_channelRealizationMap.erase(idMap);
    }
}


ChannelRealization* PropagationLossModel::GetChannelRealization(NetworkNode* src, NetworkNode* dst)
{
  ChannelRealizationMap::iterator it;

  ChannelRealizationId_t idMap = std::make_pair(src, dst);
  it = m_channelRealizationMap.find(idMap);
  if (it == m_channelRealizationMap.end()) return NULL;
  else return it->second;
}

PropagationLossModel::ChannelRealizationMap PropagationLossModel::GetChannelRealizationMap(void)
{
  return m_channelRealizationMap;
}

TransmittedSignal* PropagationLossModel::AddLossModel(NetworkNode* src, NetworkNode* dst, TransmittedSignal* txSignal)
{
  TransmittedSignal* rxSignal = txSignal->Copy();

  ChannelRealization* c = GetChannelRealization(src, dst);
  if (c == NULL) return NULL;
  std::vector<double> rxSignalValues = rxSignal->Getvalues();
  std::vector<double> loss = c->GetLoss();

  int nbOfSubChannels = rxSignalValues.size();

  for (int i = 0; i < nbOfSubChannels; i++)
  {
	  if (rxSignalValues.at(i) != 0)
	  {
		  double rxPower = rxSignalValues.at(i) - loss.at(i); // add propagation loss
		  rxSignalValues.at(i) = rxPower; // in W/Hz
	  }
  }

  rxSignal->SetValues(rxSignalValues);

  return rxSignal;
}