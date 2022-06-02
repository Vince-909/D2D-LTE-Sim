#include "MacQueue.h"
#include "../Packet/Packet.h"
#include <queue>

MacQueue::MacQueue()
{
	m_packetQueue = new std::queue<Packet*>;
	m_qLength = 0;
	m_qSize = 0;
}

MacQueue::~MacQueue()
{
	delete m_packetQueue;
}

std::queue<Packet*>* MacQueue::GetPacketQueue(void)
{
	return m_packetQueue;
}

void MacQueue::AddPacket(Packet* p)
{
	m_packetQueue->push(p);
	m_qLength += 1;
	m_qSize += p->GetSize();
}

int MacQueue::GetQueueLength(void)
{
	return m_qLength;
}

Packet* MacQueue::Peek(void)
{
	return m_packetQueue->front();
}

int MacQueue::GetQueueSize(void)
{
	return m_qSize;
}

void MacQueue::Dequeue(void)
{
	m_qLength -= 1;
	if (Peek()->GetFragmentSize() > 0) m_qSize -= Peek()->GetFragmentSize();
	else m_qSize -= Peek()->GetSize();
	m_packetQueue->pop();
}

bool MacQueue::isEmpty(void)
{
	return GetPacketQueue()->empty();
}

void MacQueue::DecreaseSize(int s)
{
	m_qSize -= s;
}