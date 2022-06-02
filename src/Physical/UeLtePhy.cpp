#include "UeLtePhy.h"
#include "../Icmp/IdealControlMessage.h"
#include "../Device/NetworkNode.h"
#include "LtePhy.h"
#include <iostream>
#include "../Icmp/PdcchMapIdealControlMessage.h"
#include "../Mac/UeMacEntity.h"
#include "../Spectrum/BandwidthManager.h"
#include "../Core/Simulator.h"
#include "../Spectrum/TransmittedSignal.h"
#include "../Device/UserEquipment.h"
#include "../Cqi/CqiManager.h"
#include "../Device/ENodeB.h"
#include "../Physical/EnbLtePhy.h"
#include "../Channel/LteChannel.h"
#include <vector>
#include "../Mac/AmcModule.h"
#include "../Physical/ErrorModel.h"
#include "../Icmp/CqiIdealControlMessage.h"
#include "../Packet/PacketBurst.h"
#include "../Icmp/PowerControlIdealControlMessage.h"
#include "../Icmp/D2DIdealControlMessage.h"
#include "../D2DManager/D2DManager.h"
#include "../PowerControl/PowerManager.h"
#include "../Packet/Packet.h"

/*
 * Noise is computed as follows:
 *  - noise figure = 2.5
 *  - n0 = -174 dBm
 *  - sub channel bandwidth = 180 kHz
 *
 *  noise_db = noise figure + n0 + 10log10 (180000) - 30 = -148.95
 */
#define NOISE -148.95

UeLtePhy::UeLtePhy()
{

	m_channelsForRx.clear ();
	m_channelsForTx.clear ();
	m_mcsIndexForRx.clear ();
	m_mcsIndexForTx.clear ();
	SetDevice(NULL);
	SetDlChannel(NULL);
	SetUlChannel(NULL);
	SetTxSignal(NULL);
	SetErrorModel(NULL);
	
}

UeLtePhy::~UeLtePhy()
{
	Destroy ();
}

void UeLtePhy::DoSetBandwidthManager(void)
{
  BandwidthManager* s = GetBandwidthManager();
  std::vector<double> channels = s->GetUlSubChannels();

  TransmittedSignal* txSignal = new TransmittedSignal();

  std::vector<double> values;
  std::vector<double>::iterator it;
  for (it = channels.begin(); it != channels.end(); it++)
  {
	  values.push_back(0);
  }

  if (m_channelsForTx.size() > 0)
  {
	  //choose d2d or cellular transmission power
	  double txPowerToUse;
	  bool usingD2D = GetDevice()->GetD2DManager()->D2DinUse();
	  if (usingD2D) txPowerToUse = GetDevice()->GetPhy()->GetPowerManager()->GetTxPowerForD2D();
	  else txPowerToUse = GetDevice()->GetPhy()->GetPowerManager()->GetTxPowerForCellular();
	  
	  //calculate power for every subchannel
      double totPower = pow(10., (txPowerToUse - 30) / 10); // in natural unit
      double txPower = 10 * log10 (totPower / m_channelsForTx.size()); //in dB
      for (std::vector<int>::iterator it = m_channelsForTx.begin();
    		  it != m_channelsForTx.end(); it++)
      {
    	  int channel = (*it);
    	  values.at(channel) = txPower;
      }
  }
  txSignal->SetValues(values);
  SetTxSignal(txSignal);
}

void UeLtePhy::SendIdealControlMessage(IdealControlMessage *msg)
{
  NetworkNode* dst = msg->GetDestination();
  dst->GetPhy()->ReceiveIdealControlMessage(msg);
}

void UeLtePhy::ReceiveIdealControlMessage(IdealControlMessage* msg)
{
  if (msg->GetMessageType () == IdealControlMessage::ALLOCATION_MAP)
	{
		if (!(GetDevice()->GetD2DManager()->D2DinUse()))
		{
			m_channelsForRx.clear();
			m_mcsIndexForRx.clear();
		}
	  m_mcsIndexForTx.clear();
	  m_channelsForTx.clear();

	  PdcchMapIdealControlMessage *map = (PdcchMapIdealControlMessage*) msg;
      PdcchMapIdealControlMessage::IdealPdcchMessage *map2 = map->GetMessage();
      PdcchMapIdealControlMessage::IdealPdcchMessage::iterator it;

      int node = GetDevice()->GetNodeID();

      for (it = map2->begin(); it != map2->end(); it++)
        {
    	  if ((*it).m_ue->GetNodeID() == node)
    	    {
              if ((*it).m_direction == PdcchMapIdealControlMessage::DOWNLINK)
                {
            	  m_channelsForRx.push_back ((*it).m_idSubChannel);
            	  m_mcsIndexForRx.push_back((*it).m_mcsIndex);
                }
              else if ((*it).m_direction == PdcchMapIdealControlMessage::UPLINK)
                {
            	  m_channelsForTx.push_back ((*it).m_idSubChannel);
            	  m_mcsIndexForTx.push_back((*it).m_mcsIndex);
                }
    	    }
	    }

      if (m_channelsForTx.size () > 0)
	  {
		  //check if D2D test must be performed
		  if (GetDevice()->GetD2DManager()->D2DTestIsNeeded())
		  {
			  GetDevice()->GetD2DManager()->SetNeedForD2DTest(false);
			  D2DIdealControlMessage *msgd2d = new D2DIdealControlMessage();
			  msgd2d->SetD2DMessageType(D2DIdealControlMessage::TEST_DATA);
			  msgd2d->SetSource(GetDevice());
			  msgd2d->SetDestination(GetDevice()->GetD2DManager()->GetD2DTarget());
			  double totPower = pow(10., (GetPowerManager()->GetTxPowerForD2D() - 30) / 10); // in natural unit
			  double txPower = 10 * log10 (totPower / m_channelsForTx.size());
			  std::vector<double> values;
			  std::vector<double>::iterator it;
			  BandwidthManager* s = GetBandwidthManager();
			  std::vector<double> channels = s->GetUlSubChannels();
			  for (it = channels.begin(); it != channels.end(); it++)
			  {
				  values.push_back(0);
			  }
			  for (std::vector<int>::iterator it = m_channelsForTx.begin(); it != m_channelsForTx.end(); it++)
			  {
				  int channel = (*it);
				  values.at(channel) = txPower;
			  }
			  msgd2d->SetTestData((UserEquipment*) GetDevice(), m_channelsForTx, m_mcsIndexForTx, values);
			  SendIdealControlMessage(msgd2d);
		  }
    	  
		  DoSetBandwidthManager();

		  if (GetDevice()->GetD2DManager()->D2DinUse())
		  {
			  	D2DIdealControlMessage* msg2 = new D2DIdealControlMessage();
				msg2->SetD2DMessageType(D2DIdealControlMessage::CHANNEL_MAP);
				msg2->SetSource(GetDevice());
				msg2->SetDestination(((UserEquipment*) GetDevice())->GetTargetEnb());
				std::vector<int>* channelstoReceive = &m_channelsForTx;
				std::vector<int>* mcsIndextoReceive = &m_mcsIndexForTx;
				msg2->SetChannels(channelstoReceive);
				msg2->SetMcs(mcsIndextoReceive);
				((UserEquipment*) GetDevice())->GetTargetEnb()->GetPhy()->ReceiveIdealControlMessage(msg2);
		  }

    	  UeMacEntity* mac = (UeMacEntity*) GetDevice()->GetMacEntity();
    	  mac->ScheduleUplinkTransmission(m_channelsForTx.size(), m_mcsIndexForTx.at(0));
        }
	}
  else if (msg->GetMessageType () == IdealControlMessage::POWER_CONTROL)
  {
	  PowerControlIdealControlMessage* pc = (PowerControlIdealControlMessage*) msg;
	  GetPowerManager()->ReceivePowerControlIdealControlMessage(pc);
  }
  else if (msg->GetMessageType() == IdealControlMessage::D2D)
  {
	  D2DIdealControlMessage* d2dmsg = (D2DIdealControlMessage*) msg;
	  UserEquipment* thisUe = (UserEquipment*) GetDevice();
	  thisUe->GetD2DManager()->ReceiveD2DIdealControlMessage(d2dmsg);
  }
}

void UeLtePhy::CreateCqiFeedbacks(std::vector<double> sinr)
{
  UserEquipment* thisNode = (UserEquipment*) GetDevice();
  if (thisNode->GetCqiManager()->NeedToSendFeedbacks())
  {
      thisNode->GetCqiManager()->CreateCqiFeedbacks(sinr);
  }
}

void UeLtePhy::StartTx(PacketBurst* p, int mcs)
{
	GetUlChannel()->StartTx(p, GetTxSignal(), GetDevice(), mcs);
}

void UeLtePhy::StartRx(PacketBurst* p, TransmittedSignal* txSignal, int mcs)
{
	m_measuredSinr.clear();
	//compute SINR
	std::vector<double> rxSignalValues;
	std::vector<double>::iterator it;

	rxSignalValues = txSignal->Getvalues();

	//compute noise + interference
	double interference = 0;

	double noise_interference = 10. * log10(pow(10., NOISE/10) + interference); // dB


	for (it = rxSignalValues.begin(); it != rxSignalValues.end(); it++)
    {
		double power; // power transmission for the current sub channel [dB]
		if ((*it) != 0.)
        {
          power = (*it);
		  m_measuredSinr.push_back (power - noise_interference);
        }
		else
        {
          power = 0.;
		  m_measuredSinr.push_back(0.0);
		}
    }
	
	//check for phy error
	bool phyError;
	if (GetErrorModel() != NULL && m_channelsForRx.size() > 0)
	{ 
	  std::vector<int> cqi_;
	  for (int i = 0; i < m_mcsIndexForRx.size(); i++)
	  {
		  AmcModule *amc = GetDevice()->GetMacEntity()->GetAmcModule();
		  int cqi =  amc->GetCQIFromMCS(m_mcsIndexForRx.at(i));
		  cqi_.push_back(cqi);
	  }
	  phyError = GetErrorModel()->CheckForPhysicalError(m_channelsForRx, cqi_, m_measuredSinr);
    }
	else phyError = false;
	if (!phyError && p->GetNPackets() > 0)
    {
	  //forward packets to the device
	  GetDevice()->ReceivePacketBurst(p);
    }
	//else if (phyError) std::cout << "ue PHY ERROR" << std::endl;
	
	//report CQI/UCSI
	if (GetDevice()->GetD2DManager()->D2DinUse())
	{
		CqiIdealControlMessage* csi = new CqiIdealControlMessage();
		csi->SetSource(GetDevice());
		csi->SetDestination(((UserEquipment*) GetDevice())->GetTargetEnb());
		csi->SetisD2DUplinkChannelStatusIndicator(true);
		csi->measuredSinr.clear();
		for (int g = 0 ; g < m_measuredSinr.size() ; g ++) csi->measuredSinr.push_back(m_measuredSinr.at(g));
	}
	else CreateCqiFeedbacks(m_measuredSinr);

	//m_channelsForRx.clear();
	m_channelsForTx.clear();
	//m_mcsIndexForRx.clear();
	m_mcsIndexForTx.clear();

	delete txSignal;
	delete p;
}

std::vector<int> UeLtePhy::GetChannelsForTx(void)
{
	return m_channelsForTx;
}

std::vector<int> UeLtePhy::GetMcsIndexForTx(void)
{
	return m_mcsIndexForTx;
}

void UeLtePhy::SetChannelsForTx(int channels)
{
	m_channelsForTx.clear();
	for (int i = 0 ; i < channels ; i ++) m_channelsForTx.push_back(i);
}

void UeLtePhy::SetChannelsForRx(std::vector<int>* channels)
{
	m_channelsForRx.clear();
	for (int i = 0 ; i < channels->size() ; i ++) m_channelsForRx.push_back(channels->at(i));
}

void UeLtePhy::SetMcsIndexForRx(std::vector<int>* mcs)
{
	m_mcsIndexForRx.clear();
	for (int i = 0 ; i < mcs->size() ; i ++) m_mcsIndexForRx.push_back(mcs->at(i));
}

std::vector<int> UeLtePhy::GetChannelsForRx(void)
{
	return m_channelsForRx;
}

std::vector<int> UeLtePhy::GetMcsIndexForRx(void)
{
	return m_mcsIndexForRx;
}