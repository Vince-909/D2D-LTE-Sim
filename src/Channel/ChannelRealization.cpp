#include "ChannelRealization.h"
#include "../Core/Simulator.h"
#include "../Device/NetworkNode.h"
#include "../Physical/LtePhy.h"
#include "../Spectrum/BandwidthManager.h"
#include "../Device/UserEquipment.h"
#include "../Position/Mobility.h"
#include "../Position/Position.h"
#include "../Utility/RandomVariable.h"

ChannelRealization::ChannelRealization()
{
  m_src = NULL;
  m_dst = NULL;
  m_samplingPeriod = 0.5;
  m_lastUpdate = NULL;
}

ChannelRealization::~ChannelRealization()
{
  Destroy ();
}

void ChannelRealization::Destroy()
{
  m_src = NULL;
  m_dst = NULL;
}

void ChannelRealization::SetSourceNode(NetworkNode* src)
{
  m_src = src;
}

NetworkNode* ChannelRealization::GetSourceNode(void)
{
  return m_src;
}

void ChannelRealization::SetDestinationNode(NetworkNode* dst)
{
  m_dst = dst;
}

NetworkNode* ChannelRealization::GetDestinationNode(void)
{
  return m_dst;
}

void ChannelRealization::SetLastUpdate(void)
{
  m_lastUpdate = Simulator::Init()->Now();
}


double ChannelRealization::GetLastUpdate(void)
{
  return m_lastUpdate;
}


void ChannelRealization::SetSamplingPeriod(double sp)
{
  m_samplingPeriod = sp;
}


double ChannelRealization::GetSamplingPeriod(void)
{
  return m_samplingPeriod;
}

bool ChannelRealization::NeedForUpdate(void)
{
  if (Simulator::Init()->Now () >= (GetLastUpdate () + GetSamplingPeriod ()) - 0.001)
    {
      return true;
    }
  else
    {
      return false;
    }
}
