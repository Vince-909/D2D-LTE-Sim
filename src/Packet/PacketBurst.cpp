#include "PacketBurst.h"
#include "Packet.h"
#include <cstdint>

PacketBurst::PacketBurst()
{
}

PacketBurst::~PacketBurst()
{
  for (std::list<Packet*>::const_iterator iter = m_packets.begin(); iter != m_packets.end(); ++iter)
  {
	  delete *iter;
  }
  m_packets.clear();
}

PacketBurst* PacketBurst::Copy(void)
{
  PacketBurst* pb = new PacketBurst();

  std::list<Packet*> packets = GetPackets();
  std::list<Packet*>::iterator it;

  for (it = packets.begin(); it != packets.end(); it++)
  {
	  Packet* packet = (*it)->Copy();
	  pb->AddPacket(packet);
  }

  return pb;
}

void PacketBurst::AddPacket(Packet* packet)
{
  if (packet)
  {
      m_packets.push_back(packet);
  }
}

std::list<Packet*> PacketBurst::GetPackets(void)
{
  return m_packets;
}

uint32_t PacketBurst::GetNPackets(void)
{
  return m_packets.size();
}

uint32_t PacketBurst::GetSize(void)
{
  uint32_t size = 0;
  for (std::list<Packet*>::const_iterator iter = m_packets.begin(); iter != m_packets.end(); ++iter)
  {
      Packet* packet = *iter;
      size += packet->GetSize();
  }
  return size;
}
