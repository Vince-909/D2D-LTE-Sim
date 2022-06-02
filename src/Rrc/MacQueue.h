#ifndef MACQUEUE_H_
#define MACQUEUE_H_

#include <queue>

class Packet;

class MacQueue
{


public:

	MacQueue();
	~MacQueue();

	void AddPacket(Packet*);

	std::queue<Packet*>* GetPacketQueue(void);
	int GetQueueLength(void);
	int GetQueueSize(void);
	Packet* Peek(void);
	void Dequeue(void);
	void DecreaseSize(int);
	bool isEmpty(void);

private:

	std::queue<Packet*>* m_packetQueue;
	int m_qLength;
	int m_qSize;

};

#endif