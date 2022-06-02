#include "NetworkNode.h"
#include <iostream>
#include "../Position/Position.h"
#include "../Physical/LtePhy.h"
#include <list>
#include "../Packet/Packet.h"
#include "../Packet/PacketBurst.h"
#include "../Rrc/RrcEntity.h"
#include "../Rrc/RadioBearer.h"
#include "../Rrc/RadioBearerSink.h"
#include <vector>
#include "../Application/Application.h"
#include "../ComponentManagers/NetworkManager.h"
#include "../PowerControl/PowerManager.h"

NetworkNode::NetworkNode()
{
	m_cell = NULL;
	m_pos = NULL;
	m_mac = NULL;
	m_rrc = NULL;
	m_phy = NULL;
	m_d2dManager = NULL;
}

NetworkNode::~NetworkNode()
{
	delete m_pos;
}

NetworkNode::NodeType NetworkNode::GetNodeType(void)
{
	return m_nodeType;
}

void NetworkNode::SetNodeType(NetworkNode::NodeType type)
{
	m_nodeType = type;
}

int NetworkNode::GetNodeID()
{
	return m_idNode;
}

void NetworkNode::SetNodeID(int id)
{
	m_idNode = id;
}

Cell* NetworkNode::GetCell()
{
	return m_cell;
}

void NetworkNode::SetCell(Cell* cell)
{
	m_cell = cell;
}

Position* NetworkNode::GetPosition()
{
	return m_pos;
}

void NetworkNode::SetPosition(double x, double y, double speed, double direction, double outerBound, double innerBound)
{
	m_pos = new Position(x, y, speed, direction, outerBound, innerBound);
}

void NetworkNode::SetPosition(double x, double y)
{
	m_pos = new Position(x, y);
}

RrcEntity* NetworkNode::GetRrcEntity(void)
{
	return m_rrc;
}

void NetworkNode::SetRrcEntity(RrcEntity* rrc)
{
	m_rrc = rrc;
}

LtePhy* NetworkNode::GetPhy(void)
{
	return m_phy;
}
	
void NetworkNode::SetPhy(LtePhy* phy)
{
	m_phy = phy;
}

MacEntity* NetworkNode::GetMacEntity(void)
{
	return m_mac;
}

D2DManager* NetworkNode::GetD2DManager(void)
{
	return m_d2dManager;
}

void NetworkNode::SetD2DManager(D2DManager* man)
{
	m_d2dManager = man;
}

void NetworkNode::SetMacEntity(MacEntity* mac)
{
	m_mac= mac;
}

void NetworkNode::SendPacketBurst(PacketBurst* pb, int mcs)
{
	GetPhy()->StartTx(pb, mcs);
}

void NetworkNode::ReceivePacketBurst(PacketBurst* pb)
{
  std::list<Packet*> packets = pb->GetPackets();
  std::list<Packet*>::iterator it;

  RrcEntity* rrc = GetRrcEntity();
  for (it = packets.begin(); it != packets.end(); it++)
  {
	  Packet* packet = (*it);
	  //if packet was destined to this MAC
	  if (packet->GetIdMac() == GetNodeID())
	  {
		  for (RrcEntity::RadioBearersSinkContainer::iterator itt = rrc->GetRadioBearerSinkContainer()->begin();
			  itt != rrc->GetRadioBearerSinkContainer()->end(); itt++)
		  {
			  RadioBearerSink* rbs = (*itt);
			  int RadioBearerSinkApplicationId = rbs->GetRadioBearer()->GetApplication()->GetApplicationID();
			  //find the radio bearer sink for this packet
			  if (RadioBearerSinkApplicationId == packet->GetApplicationId())
			  {
				  //forward packet to the radio bearer sink
				  rbs->Receive(packet->Copy());
				  break;
			  }
		  }
	  }

  }
}