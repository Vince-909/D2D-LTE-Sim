#ifndef PACKETBURST_H_
#define PACKETBURST_H_

#include <list>
#include <cstdint>

class Packet;

class PacketBurst
{

public:

  PacketBurst();
  virtual ~PacketBurst();

  PacketBurst* Copy(void);
  void AddPacket (Packet*);
  std::list<Packet*> GetPackets(void);
  uint32_t GetNPackets(void);
  uint32_t GetSize(void);

  std::list<Packet* >::const_iterator Begin(void);
  std::list<Packet* >::const_iterator End(void);

private:

  std::list<Packet* > m_packets;

};

#endif