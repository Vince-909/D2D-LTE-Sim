#include "DownlinkPacketScheduler.h"
#include "../Rrc/RrcEntity.h"
#include "../Mac/MacEntity.h"
#include "../Device/NetworkNode.h"
#include "../Rrc/RadioBearer.h"
#include <vector>
#include "../Rrc/MacQueue.h"
#include "../Application/Application.h"
#include "../Device/UserEquipment.h"
#include "../Device/ENodeB.h"
#include "../Physical/LtePhy.h"
#include "../Spectrum/BandwidthManager.h"
#include "../Mac/AmcModule.h"
#include "../PowerControl/PowerManager.h"
#include "../Utility/EesmEffectiveSinr.h"
#include "../Icmp/PdcchMapIdealControlMessage.h"
#include "../Packet/PacketBurst.h"
#include "../Rrc/MacQueue.h"
#include "../Packet/Packet.h"
#include <iostream>

DownlinkPacketScheduler::DownlinkPacketScheduler()
{
}

DownlinkPacketScheduler::~DownlinkPacketScheduler()
{
	Destroy();
}

void DownlinkPacketScheduler::DoSchedule(void)
{
	
	UpdateAverageTransmissionRate();
	SelectFlowsToSchedule();
	if (GetFlowsToSchedule()->size() > 0) RBsAllocation();
	StopSchedule();
	
}

void DownlinkPacketScheduler::DoStopSchedule(void)
{
	PacketBurst* pb = new PacketBurst();

	//Create Packet Burst
	FlowsToSchedule* flowsToSchedule = GetFlowsToSchedule();

	for (FlowsToSchedule::iterator it = flowsToSchedule->begin(); it != flowsToSchedule->end(); it++)
    {
		FlowToSchedule *flow = (*it);
		int availableBytes = flow->GetAllocatedBits()/8;
		RadioBearer* b = flow->GetBearer();
		if (availableBytes > 0)
		  {
			  while (!(b->GetMacQueue()->isEmpty()) && (availableBytes > 0))
			  {
					int sizeToSend;
					if (b->GetMacQueue()->Peek()->GetFragmentSize() > 0)
						sizeToSend = b->GetMacQueue()->Peek()->GetFragmentSize();
					else
						sizeToSend = b->GetMacQueue()->Peek()->GetSize();
					if (sizeToSend <= availableBytes)
					{
						availableBytes -= sizeToSend;
						b->GetMacQueue()->Peek()->SetIdMac(b->GetMacQueue()->Peek()->GetDestination());
						pb->AddPacket(b->GetMacQueue()->Peek());
						b->GetMacQueue()->Dequeue();
					}
					else
					{
						//fragment
						if (b->GetMacQueue()->Peek()->GetFragmentSize() > 0)
							b->GetMacQueue()->Peek()->SetFragmentSize(b->GetMacQueue()->Peek()->GetFragmentSize() - availableBytes);
						else
							b->GetMacQueue()->Peek()->SetFragmentSize(b->GetMacQueue()->Peek()->GetSize() - availableBytes);
						Packet* pf = b->GetMacQueue()->Peek()->Copy();
						pf->SetFragmentSize(availableBytes);
						pf->SetIdMac(pf->GetDestination());
						b->GetMacQueue()->DecreaseSize(availableBytes);
						availableBytes = 0;
						pb->AddPacket(pf);
					}
			  }
		  }

    }
	if (pb->GetNPackets() > 0) GetMacEntity()->GetDevice()->SendPacketBurst(pb);
}

void DownlinkPacketScheduler::UpdateAverageTransmissionRate(void)
{
	RrcEntity *rrc = GetMacEntity()->GetDevice()->GetRrcEntity();
	RrcEntity::RadioBearersContainer* bearers = rrc->GetRadioBearerContainer();

  for (std::vector<RadioBearer* >::iterator it = bearers->begin(); it != bearers->end(); it++)
    {
		RadioBearer *bearer = (*it);
		bearer->UpdateAverageTransmissionRate();
    }
}

void DownlinkPacketScheduler::SelectFlowsToSchedule(void)
{
	ClearFlowsToSchedule();

	RrcEntity *rrc = GetMacEntity()->GetDevice()->GetRrcEntity();
	RrcEntity::RadioBearersContainer* bearers = rrc->GetRadioBearerContainer();

	for (std::vector<RadioBearer*>::iterator it = bearers->begin(); it != bearers->end(); it++)
	{
	  //SELECT FLOWS TO SCHEDULE
	  RadioBearer* bearer = (*it);
	  if (bearer->GetMacQueue()->GetQueueSize() == 0) continue;
	  int dataToTransmit;
	  
	  dataToTransmit = bearer->GetMacQueue()->GetQueueSize();
	  //compute spectral efficiency
	  ENodeB *enb = (ENodeB*) GetMacEntity()->GetDevice();
	  ENodeB::UserEquipmentRecord *ueRecord = enb->GetUserEquipmentRecord(bearer->GetApplication()->GetApplicationDestination()->GetNodeID());
	  std::vector<double> spectralEfficiency;
	  std::vector<int> cqiFeedbacks = ueRecord->GetCQI();
	  int numberOfCqi = cqiFeedbacks.size ();
	  for (int i = 0; i < numberOfCqi; i++)
	  {
		  double sEff = GetMacEntity()->GetAmcModule()->GetEfficiencyFromCQI(cqiFeedbacks.at(i));
		  spectralEfficiency.push_back(sEff);
	  }
	  
	  //create flow to scheduler record
	  InsertFlowToSchedule(bearer, dataToTransmit, spectralEfficiency, cqiFeedbacks);
	}
}

void DownlinkPacketScheduler::RBsAllocation(void)
{
	FlowsToSchedule* flows = GetFlowsToSchedule();
	int nbOfRBs = GetMacEntity()->GetDevice()->GetPhy()->GetBandwidthManager()->GetDlSubChannels().size();

	//create a matrix of flow metrics
	double** metrics = new double*[nbOfRBs];
	for (int l = 0 ; l < nbOfRBs ; l ++) metrics[l] = new double[flows->size()];
	for (int i = 0; i < nbOfRBs; i++)
    {
	  for (int j = 0; j < flows->size(); j++)
	  {
		  metrics[i][j] = ComputeSchedulingMetric(flows->at(j)->GetBearer(), flows->at(j)->GetSpectralEfficiency().at(i), i);
	  }
    }


  AmcModule* amc = GetMacEntity()->GetAmcModule();
  double l_dAllocatedRBCounter = 0;

  int l_iNumberOfUsers = ((ENodeB*) GetMacEntity()->GetDevice())->GetNbOfUserEquipmentRecords();

  bool* l_bFlowScheduled = new bool[flows->size()];
  int l_iScheduledFlows = 0;
  std::vector<double> * l_bFlowScheduledSINR = new std::vector<double>[flows->size()];
  for (int k = 0; k < flows->size(); k++) l_bFlowScheduled[k] = false;

  //RBs allocation
  for (int s = 0; s < nbOfRBs; s++)
  {
      if (l_iScheduledFlows == flows->size()) break;

      double targetMetric = 0;
      bool RBIsAllocated = false;
      FlowToSchedule* scheduledFlow;
      int l_iScheduledFlowIndex = 0;

      for (int k = 0; k < flows->size(); k++)
      {
          if (metrics[s][k] >= targetMetric && !l_bFlowScheduled[k])
          {
              targetMetric = metrics[s][k];
              RBIsAllocated = true;
              scheduledFlow = flows->at(k);
              l_iScheduledFlowIndex = k;
          }
      }

      if (RBIsAllocated)
      {
          l_dAllocatedRBCounter++;

          scheduledFlow->GetListOfAllocatedRBs()->push_back(s); // the s RB has been allocated to that flow!
          double sinr = amc->GetSinrFromCQI(scheduledFlow->GetCqiFeedbacks().at(s));
          l_bFlowScheduledSINR[l_iScheduledFlowIndex].push_back(sinr);

          double effectiveSinr = GetEesmEffectiveSinr(l_bFlowScheduledSINR[l_iScheduledFlowIndex]);
          int mcs = amc->GetMCSFromCQI(amc->GetCQIFromSinr(effectiveSinr));
          int transportBlockSize = amc->GetTBSizeFromMCS(mcs, scheduledFlow->GetListOfAllocatedRBs()->size());
          if (transportBlockSize >= scheduledFlow->GetDataToTransmit() * 8)
          {
              l_bFlowScheduled[l_iScheduledFlowIndex] = true;
              l_iScheduledFlows++;
          }

      }
  }

  delete [] l_bFlowScheduled;
  delete [] l_bFlowScheduledSINR;


  //Finalize the allocation
  PdcchMapIdealControlMessage *pdcchMsg = new PdcchMapIdealControlMessage();
  
  for (FlowsToSchedule::iterator it = flows->begin(); it != flows->end(); it++)
  {
      FlowToSchedule *flow = (*it);
      if (flow->GetListOfAllocatedRBs()->size () > 0)
      {
          //this flow has been scheduled
          std::vector<double> estimatedSinrValues;
          for (int rb = 0; rb < flow->GetListOfAllocatedRBs()->size(); rb++)
		  {
              double sinr = amc->GetSinrFromCQI(flow->GetCqiFeedbacks().at(flow->GetListOfAllocatedRBs()->at(rb)));
              estimatedSinrValues.push_back (sinr);
          }

          //compute the effective sinr
          double effectiveSinr = GetEesmEffectiveSinr(estimatedSinrValues);

          //get the MCS for transmission

          int mcs = amc->GetMCSFromCQI(amc->GetCQIFromSinr(effectiveSinr));
          //define the amount of bytes to transmit
          //int transportBlockSize = amc->GetTBSizeFromMCS (mcs);
          int transportBlockSize = amc->GetTBSizeFromMCS(mcs, flow->GetListOfAllocatedRBs()->size());
          double bitsToTransmit = transportBlockSize;
          flow->UpdateAllocatedBits(bitsToTransmit);

		  //create PDCCH messages
		  for (int rb = 0; rb < flow->GetListOfAllocatedRBs()->size(); rb++)
		  {
			  pdcchMsg->AddNewRecord(PdcchMapIdealControlMessage::DOWNLINK, flow->GetListOfAllocatedRBs()->at(rb),
				  flow->GetBearer()->GetApplication()->GetApplicationDestination(), mcs);
		  }
	  }
  }

  if (pdcchMsg->GetMessage()->size() > 0)
  {
      GetMacEntity()->GetDevice()->GetPhy()->SendIdealControlMessage(pdcchMsg);
  }
  delete pdcchMsg;

  for (int l = 0 ; l < nbOfRBs ; l ++) delete[] metrics[l];
  delete metrics;
}