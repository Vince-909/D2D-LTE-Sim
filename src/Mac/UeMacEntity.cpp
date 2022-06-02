#include "UeMacEntity.h"
#include "../Core/Simulator.h"
#include "AmcModule.h"
#include "../Rrc/RrcEntity.h"
#include "../Device/NetworkNode.h"
#include "../Rrc/RadioBearer.h"
#include "../Application/Application.h"
#include "../Rrc/MacQueue.h"
#include "../Device/UserEquipment.h"
#include "../Icmp/SchedulingRequestIdealControlMessage.h"
#include "../Physical/LtePhy.h"
#include "../Packet/PacketBurst.h"
#include "../PowerControl/PowerManager.h"
#include "../Packet/Packet.h"
#include "../Scheduler/DownlinkPacketScheduler.h"
#include "../Mac/EnbMacEntity.h"
#include "../Device/ENodeB.h"
#include "../Spectrum/TransmittedSignal.h"
#include "../D2DManager/D2DManager.h"

UeMacEntity::UeMacEntity()
{
	SetAmcModule(new AmcModule());
	SetDevice(NULL);
	m_schedulingRequestInterval = 0.001;
	Simulator::Init()->Schedule(GetSchedulingRequestInterval(), &UeMacEntity::SendSchedulingRequest, this);
}

UeMacEntity::~UeMacEntity()
{
	Destroy();
}

double UeMacEntity::GetSchedulingRequestInterval()
{
	return m_schedulingRequestInterval;
}

void UeMacEntity::SetSchedulingRequestInterval(double t)
{
	m_schedulingRequestInterval = t;
}

void UeMacEntity::SendSchedulingRequest()
{
  //compute the buffer status report
  int bufferStatusReport = 0;
  RrcEntity *rrc = GetDevice()->GetRrcEntity();

  if (rrc->GetRadioBearerContainer()->size() > 0)
  {
	  for (RrcEntity::RadioBearersContainer::iterator it = rrc->GetRadioBearerContainer()->begin();
			  it != rrc->GetRadioBearerContainer()->end(); it++)
	  {
		  RadioBearer *b = (*it);
		  bufferStatusReport += b->GetMacQueue()->GetQueueSize();
	  }
  }

  if (bufferStatusReport > 0)
  {
      //create the message
	  SchedulingRequestIdealControlMessage* msg = new SchedulingRequestIdealControlMessage();
	  UserEquipment* thisNode = (UserEquipment*) GetDevice();
	  msg->SetSource(thisNode);
	  msg->SetDestination((NetworkNode*)thisNode->GetTargetEnb());
      msg->SetBufferStatusReport(bufferStatusReport);

      //send the message
	  GetDevice()->GetPhy()->SendIdealControlMessage(msg);
    }


  //schedule the next event
  Simulator::Init()->Schedule(GetSchedulingRequestInterval(), &UeMacEntity::SendSchedulingRequest, this);

}

void UeMacEntity::ScheduleUplinkTransmission(int nbPrb, int mcs)
{
	bool usingD2D = GetDevice()->GetD2DManager()->D2DinUse();
	int availableBytes = GetAmcModule()->GetTBSizeFromMCS(mcs, nbPrb)/8;
	PacketBurst *pb = new PacketBurst();
	RrcEntity *rrc = GetDevice()->GetRrcEntity();

	if (rrc->GetRadioBearerContainer()->size() > 0)
    {
	  for (RrcEntity::RadioBearersContainer::iterator it = rrc->GetRadioBearerContainer()->begin();
			  it != rrc->GetRadioBearerContainer()->end(); it++)
		{
		  RadioBearer *b = (*it);
		  if (availableBytes > 0)
		  {
			  while (!(b->GetMacQueue()->isEmpty()) && (availableBytes > 0))
			  {
					int sizeToSend;
					if (b->GetMacQueue()->Peek()->GetFragmentSize() > 0)
						sizeToSend = b->GetMacQueue()->Peek()->GetFragmentSize();
					else
						sizeToSend = b->GetMacQueue()->Peek()->GetSize();
					//if packet can be transmitted as a whole
					if (sizeToSend <= availableBytes)
					{
						availableBytes -= sizeToSend;
						UserEquipment* thisNode = (UserEquipment*) GetDevice();
						if (!usingD2D) b->GetMacQueue()->Peek()->SetIdMac(thisNode->GetTargetEnb()->GetNodeID());
						else b->GetMacQueue()->Peek()->SetIdMac(b->GetMacQueue()->Peek()->GetDestination());
						pb->AddPacket(b->GetMacQueue()->Peek());
						b->GetMacQueue()->Dequeue();
					}
					//else fragment the packet
					else
					{
						if (b->GetMacQueue()->Peek()->GetFragmentSize() > 0)
							b->GetMacQueue()->Peek()->SetFragmentSize(b->GetMacQueue()->Peek()->GetFragmentSize() - availableBytes);
						else
							b->GetMacQueue()->Peek()->SetFragmentSize(b->GetMacQueue()->Peek()->GetSize() - availableBytes);
						Packet* pf = b->GetMacQueue()->Peek()->Copy();
						pf->SetFragmentSize(availableBytes);
						UserEquipment* thisNode = (UserEquipment*) GetDevice();
						if (!usingD2D) pf->SetIdMac(thisNode->GetTargetEnb()->GetNodeID());
						else pf->SetIdMac(pf->GetDestination());
						b->GetMacQueue()->DecreaseSize(availableBytes);
						availableBytes = 0;
						pb->AddPacket(pf);
					}
			  }
		  }
		  GetDevice()->SendPacketBurst(pb, mcs);
    }
  }
}
