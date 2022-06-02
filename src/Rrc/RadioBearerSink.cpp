#include "RadioBearerSink.h"
#include <iostream>
#include "../Packet/Packet.h"
#include "RadioBearer.h"
#include "RrcEntity.h"
#include "../Device/NetworkNode.h"
#include "../Application/ApplicationSink.h"
#include "../Application/Application.h"
#include "../Device/UserEquipment.h"
#include "../Device/ENodeB.h"
#include "../Rrc/RrcEntity.h"
#include <vector>

RadioBearerSink::RadioBearerSink()
{
	m_bearer = NULL;
	m_appSink = NULL;
	m_fragments = new std::vector<std::vector<Packet*>*>;
}

RadioBearerSink::~RadioBearerSink()
{
}

RadioBearer* RadioBearerSink::GetRadioBearer(void)
{
	return m_bearer;
}

void RadioBearerSink::SetRadioBearer(RadioBearer* bearer)
{
	m_bearer = bearer;
}

ApplicationSink* RadioBearerSink::GetApplicationSink(void)
{
	return m_appSink;
}

void RadioBearerSink::SetApplicationSink(ApplicationSink* sink)
{
	m_appSink = sink;
}

std::vector<std::vector<Packet*>*>* RadioBearerSink::GetFragments(void)
{
	return m_fragments;
}

void RadioBearerSink::Receive(Packet* packet)
{
	Packet* np = NULL;
	//if packet is a fragment
	if (packet->GetFragmentSize() > 0) if (Defragment(&packet)) return;

	//if there is an application sink, the device is a UE, forward packet to the application sink.
	if (GetApplicationSink() != NULL) GetApplicationSink()->Receive(packet);
	//else it is an ENodeB, enqueue the packet for downlink scheduling
	else
	{
		ENodeB* thisNode = GetRadioBearer()->GetApplication()->GetApplicationSource()->GetTargetEnb();
		int thisBearerSinkAppId = GetRadioBearer()->GetApplication()->GetApplicationID();
		std::vector<RadioBearer*>* RadioBearers = thisNode->GetRrcEntity()->GetRadioBearerContainer();
		std::vector<RadioBearer*>::iterator it;
		for (it = RadioBearers->begin(); it != RadioBearers->end(); it++)
		{
			RadioBearer* rb = (*it);
			//find the appropriate radio bearer sink for this packet
			if (rb->GetApplication()->GetApplicationID() == thisBearerSinkAppId)
			{
				//enqueue packet for downlink scheduling
				rb->Enqueue(packet);
				break;
			}

		}
	}
}

bool RadioBearerSink::Defragment(Packet** packet)
{

	Packet* np = NULL;
	bool found = false;
	std::vector<std::vector<Packet*>*>* frags = GetFragments();

	//if there are more fragments of the same packet
	if (!(frags->empty()) && ((*packet)->GetId() == frags->front()->front()->GetId()))
			{
				found = true;
				int totalSize = 0;
				for (int cc = 0 ; cc < frags->front()->size() ; cc ++) totalSize += frags->front()->at(cc)->GetFragmentSize();
				//if all fragments have been received
				if ((totalSize + (*packet)->GetFragmentSize()) == frags->front()->front()->GetSize())
				{
					np = (*packet)->Copy();
					np->SetFragmentSize(0);
					for (int ccc = 0 ; ccc < frags->front()->size() ; ccc ++) delete frags->front()->at(ccc);
					delete frags->front();
					frags->erase(frags->begin());
					delete *packet;
					(*packet) = np;
				}
				else
				{
						frags->front()->push_back(*packet);
						return true;
				}
			}

	if (!found)
	{
		if (!(frags->empty()))
		{
			for (int ccc = 0 ; ccc < frags->front()->size() ; ccc ++) delete frags->front()->at(ccc);
			delete frags->front();
			frags->erase(frags->begin());
		}
			std::vector<Packet*>* samePacketFragments = new std::vector<Packet*>;
			frags->push_back(samePacketFragments);
			frags->back()->push_back(*packet);
			return true;
	}
return false;
}
