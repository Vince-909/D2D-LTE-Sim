#include "Packet.h"

Packet::Packet()
{
	m_fragmentSize = 0;
}

Packet::~Packet()
{
}

void Packet::SetId(int id)
{
	m_id = id;
}

void Packet::SetTimeStamp(double timeStamp)
{
	m_timeStamp = timeStamp;
}

void Packet::SetSize(int size)
{
	m_size = size;
}

void Packet::SetSource(int source)
{
	m_idSource = source;
}

void Packet::SetDestination(int dest)
{
	m_idDestination = dest;
}

int Packet::GetId(void)
{
	return m_id;
}

double Packet::GetTimeStamp(void)
{
	return m_timeStamp;
}

int Packet::GetSize(void)
{
	return m_size;
}

int Packet::GetSource(void)
{
	return m_idSource;
}

int Packet::GetDestination(void)
{
	return m_idDestination;
}

int Packet::GetApplicationId(void)
{
	return m_idApplication;
}

void Packet::SetApplicationId(int id)
{
	m_idApplication = id;
}

int Packet::GetIdMac(void)
{
	return m_idMacDestination;
}

void Packet::SetIdMac(int mac)
{
	m_idMacDestination = mac;
}

int Packet::GetFragmentSize(void)
{
	return m_fragmentSize;
}

void Packet::SetFragmentSize(int size)
{
	m_fragmentSize = size;
}

Packet* Packet::Copy(void)
{
	Packet* p = new Packet();
	p->SetDestination(GetDestination());
	p->SetSource(GetSource());
	p->SetTimeStamp(GetTimeStamp());
	p->SetApplicationId(GetApplicationId());
	p->SetIdMac(GetIdMac());
	p->SetSize(GetSize());
	p->SetId(GetId());
	p->SetFragmentSize(GetFragmentSize());

	return p;
}