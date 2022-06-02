#include "EnbLtePhy.h"
#include "LtePhy.h"
#include "../Device/NetworkNode.h"
#include "../Icmp/IdealControlMessage.h"
#include <iostream>
#include "../Icmp/CqiIdealControlMessage.h"
#include "../Icmp/SchedulingRequestIdealControlMessage.h"
#include "../Mac/EnbMacEntity.h"
#include "../Device/ENodeB.h"
#include "../Device/UserEquipment.h"
#include "../Spectrum/BandwidthManager.h"
#include "../Spectrum/TransmittedSignal.h"
#include "../Packet/PacketBurst.h"
#include "../Channel/LteChannel.h"
#include "../Physical/ErrorModel.h"
#include "../Channel/PropagationLossModel.h"
#include "../Position/Position.h"
#include "../Packet/Packet.h"
#include "../ComponentManagers/NetworkManager.h"
#include "../Mac/AmcModule.h"
#include "UeLtePhy.h"
#include "../Icmp/PowerControlIdealControlMessage.h"
#include "../Icmp/QoeIdealControlMessage.h"
#include "../Icmp/D2DIdealControlMessage.h"
#include "../D2DManager/D2DManager.h"
#include "../PowerControl/PowerManager.h"

/*
 * Noise is computed as follows:
 *  - noise figure = 2.5
 *  - n0 = -174 dBm
 *  - sub channel bandwidth = 180 kHz
 *
 *  noise_db = noise figure + n0 + 10log10 (180000) - 30 = -148.95
 */

#define NOISE -148.95

EnbLtePhy::EnbLtePhy()
{
	SetDevice(NULL);
	SetDlChannel(NULL);
	SetUlChannel(NULL);
	SetTxSignal(NULL);
	SetErrorModel(NULL);
}

EnbLtePhy::~EnbLtePhy()
{
  Destroy();
}

void EnbLtePhy::DoSetBandwidthManager(void)
{
  BandwidthManager* s = GetBandwidthManager();
  std::vector<double> channels = s->GetDlSubChannels();

  TransmittedSignal* txSignal = new TransmittedSignal();

  std::vector<double> values;
  std::vector<double>::iterator it;

  double powerTx = pow (10., (GetPowerManager()->GetTxPowerForCellular() - 30) / 10); // in natural unit

  double txPower = 10 * log10 (powerTx / channels.size()); //in dB

  for (it = channels.begin(); it != channels.end(); it++ )
  {
      values.push_back(txPower);
  }

  txSignal->SetValues(values);

  SetTxSignal(txSignal);
}

void EnbLtePhy::SendIdealControlMessage(IdealControlMessage* msg)
{
  if (msg->GetMessageType() == IdealControlMessage::ALLOCATION_MAP)
  {
	  ENodeB *enb = (ENodeB*) GetDevice();
	  ENodeB::UserEquipmentRecords* registeredUe = enb->GetUserEquipmentRecords();
	  ENodeB::UserEquipmentRecords::iterator it;

	  for (it = registeredUe->begin(); it != registeredUe->end(); it++)
	  {
			  (*it)->GetUE()->GetPhy()->ReceiveIdealControlMessage(msg);
	  }
  }
  else if (msg->GetMessageType() == IdealControlMessage::POWER_CONTROL)
  {
	  msg->GetDestination()->GetPhy()->ReceiveIdealControlMessage(msg);
  }
  else if (msg->GetMessageType() == IdealControlMessage::D2D)
  {
	  msg->GetDestination()->GetPhy()->ReceiveIdealControlMessage(msg);
  }
}

void EnbLtePhy::ReceiveIdealControlMessage(IdealControlMessage* msg)
{
	if (msg->GetMessageType() == IdealControlMessage::CQI_FEEDBACKS)
    {
		CqiIdealControlMessage *cqiMsg = (CqiIdealControlMessage*) msg;
		EnbMacEntity *mac = (EnbMacEntity*) GetDevice()->GetMacEntity();
		mac->ReceiveCqiIdealControlMessage(cqiMsg);
    }
	if (msg->GetMessageType() == IdealControlMessage::SCHEDULING_REQUEST)
    {
		SchedulingRequestIdealControlMessage *srMsg = (SchedulingRequestIdealControlMessage*) msg;
		EnbMacEntity *mac = (EnbMacEntity*) GetDevice()->GetMacEntity();
		mac->ReceiveSchedulingRequestIdealControlMessage(srMsg);
    }
	if (msg->GetMessageType() == IdealControlMessage::QOE_REPORT)
	{
		QoeIdealControlMessage* qoeMsg = (QoeIdealControlMessage*) msg;
		EnbMacEntity* mac = (EnbMacEntity*) GetDevice()->GetMacEntity();
		mac->ReceiveQoeIdealControlMessage(qoeMsg);
	}
	if (msg->GetMessageType() == IdealControlMessage::D2D)
	{
		D2DIdealControlMessage* d2dmsg = (D2DIdealControlMessage*) msg;
		GetDevice()->GetD2DManager()->ReceiveD2DIdealControlMessage(d2dmsg);
	}
}

void EnbLtePhy::StartRx(PacketBurst* p, TransmittedSignal* txSignal, int mcs)
{
	int senderId = p->GetPackets().front()->GetSource();
	UserEquipment* ue = (UserEquipment*) NetworkManager::Init()->GetNetworkNodeByID(senderId);
	UeLtePhy* uePhy = ((UeLtePhy*)(ue->GetPhy()));
	ENodeB::UserEquipmentRecord* user = ((ENodeB*) GetDevice())->GetUserEquipmentRecord(senderId);
	std::vector<double> measuredSinr;
	std::vector<int> channelsForRx;
	std::vector<double> rxSignalValues;
	std::vector<double>::iterator it;
	rxSignalValues = txSignal->Getvalues();

	double interference = 0;
	double noise_interference = 10. * log10(pow(10., NOISE/10) + interference); // dB

	int chId = 0;
	for (it = rxSignalValues.begin(); it != rxSignalValues.end(); it++)
	{
		double power; // power transmission for the current sub channel [dB]
		if ((*it) != 0.)
		{
			power = (*it);
			channelsForRx.push_back(chId);
			measuredSinr.push_back(power - noise_interference);
		}
		else
        {
			power = 0.;
			measuredSinr.push_back(0.0);
        }
		chId++;
	}
	if ((measuredSinr.at(channelsForRx.front()) > 38.) || (measuredSinr.at(channelsForRx.front()) < 0.))
	{
		PowerControlIdealControlMessage* pc = new PowerControlIdealControlMessage;
		if (measuredSinr.front() > 38.) pc->SetAction(PowerControlIdealControlMessage::DECREASE_POWER);
		else pc->SetAction(PowerControlIdealControlMessage::INCREASE_POWER);
		pc->SetSource(this->GetDevice());
		pc->SetDestination(ue);
		SendIdealControlMessage(pc);
	}
	user->SetUplinkChannelStatusIndicator(measuredSinr);

	//check for phy error
	bool phyError = false;
	
	if (GetErrorModel() != NULL)
    {
	  std::vector<int> cqi_;
	  for (int i = 0; i < channelsForRx.size(); i++)
	  {
		  
		  AmcModule *amc = GetDevice()->GetMacEntity()->GetAmcModule();
		  int cqi =  amc->GetCQIFromMCS(mcs);
		  
		  
		  cqi_.push_back(cqi);
	  }
	  phyError = GetErrorModel()->CheckForPhysicalError(channelsForRx, cqi_, measuredSinr);
    }
	else
    {
	  phyError = false;
    }
	
	if (!phyError) GetDevice()->ReceivePacketBurst(p);
	//else if (phyError) std::cout << "enb PHY ERROR" << std::endl;

	delete txSignal;
	delete p;
}

void EnbLtePhy::StartTx(PacketBurst* p, int mcs)
{
	GetDlChannel()->StartTx(p, GetTxSignal(), GetDevice());
}