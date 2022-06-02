#include "LteChannel.h"
#include "../Core/Simulator.h"
#include "../Device/NetworkNode.h"
#include <vector>
#include "../Spectrum/TransmittedSignal.h"
#include "../Packet/PacketBurst.h"
#include "../Physical/LtePhy.h"
#include "PropagationLossModel.h"

LteChannel::LteChannel()
{
  m_attachedDevices = new std::vector<NetworkNode*>();
  m_propagationLossModel = new PropagationLossModel();
}

LteChannel::~LteChannel()
{
	m_attachedDevices->clear();
	delete m_attachedDevices;
	delete m_propagationLossModel;
}


void LteChannel::StartTx(PacketBurst* p, TransmittedSignal* txSignal, NetworkNode* src, int mcs)
{
	
	Simulator::Init()->Schedule(0.001, &LteChannel::StartRx, this, p, txSignal, src, mcs);
}

void LteChannel::StartRx(PacketBurst* p, TransmittedSignal* txSignal, NetworkNode* src, int mcs)
{
	//for every device attached to the channel
	for (std::vector<NetworkNode*>::iterator it = GetDevices()->begin(); it != GetDevices()->end(); it++)
	  {
		  NetworkNode* dst = *it;
		  //apply the propagation loss model
		  TransmittedSignal* rxSignal;
		  if (m_propagationLossModel != NULL)
		  {
			  rxSignal = GetPropagationLossModel()->AddLossModel(src, dst, txSignal);
			  if (rxSignal == NULL) continue;
		  }
		  else
		  {
			  rxSignal = txSignal->Copy();
		  }
		  //deliver packets to the physical layer
		  dst->GetPhy()->StartRx(p->Copy(), rxSignal, mcs);
		}

	  delete p;
}

void LteChannel::AddDevice(NetworkNode* d)
{
	m_attachedDevices->push_back(d);
}

void LteChannel::DelDevice(NetworkNode* d)
{
  std::vector<NetworkNode*> *new_list = new std::vector<NetworkNode*>();
  for (std::vector<NetworkNode*>::iterator it = GetDevices()->begin();
		  it != GetDevices()->end(); it++)
  {
	  NetworkNode* node = *it;

	  if (node->GetNodeID() != d->GetNodeID())
	  {
		  new_list->push_back(node);
	  }
  }

  m_attachedDevices->clear();
  delete m_attachedDevices;
  m_attachedDevices = new_list;
}

bool
LteChannel::IsAttached(NetworkNode* d)
{
  for (std::vector<NetworkNode*>::iterator it = GetDevices()->begin();
		  it != GetDevices()->end(); it++)
    {
	  NetworkNode* node = *it;

	  if (node->GetNodeID() == d->GetNodeID())
	    {
		  return true;
	    }
    }
  return false;
}

std::vector<NetworkNode*>* LteChannel::GetDevices(void)
{
	return m_attachedDevices;
}

void LteChannel::SetPropagationLossModel(PropagationLossModel* m)
{
  m_propagationLossModel = m;
}

PropagationLossModel* LteChannel::GetPropagationLossModel(void)
{
  return m_propagationLossModel;
}

void LteChannel::SetChannelId(int id)
{
  m_channelId = id;
}

int LteChannel::GetChannelId(void)
{
  return m_channelId;
}
