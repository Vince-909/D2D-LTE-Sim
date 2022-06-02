#include "EnbMacEntity.h"
#include "../Device/UserEquipment.h"
#include "../Icmp/SchedulingRequestIdealControlMessage.h"
#include "../Device/ENodeB.h"
#include <iostream>
#include "../Icmp/CqiIdealControlMessage.h"
#include "../Icmp/QoeIdealControlMessage.h"
#include "../ComponentManagers/FlowsManager.h"
#include <vector>
#include "../D2DManager/D2DManager.h"
#include "../Position/Position.h"
#include <fstream>
#include "../Core/Simulator.h"
#include <iomanip>
#include "../Physical/EnbLtePhy.h"
#include "../Channel/MacroCellUrbanAreaChannelRealization.h"
#include "../Channel/PropagationLossModel.h"
#include "../Channel/LteChannel.h"

EnbMacEntity::EnbMacEntity()
{
}

EnbMacEntity::~EnbMacEntity()
{
}

void EnbMacEntity::ReceiveSchedulingRequestIdealControlMessage(SchedulingRequestIdealControlMessage* msg)
{
	UserEquipment* ue = (UserEquipment*) msg->GetSource();
	ENodeB* enb = (ENodeB*) GetDevice();
	ENodeB::UserEquipmentRecord* record = enb->GetUserEquipmentRecord(ue->GetNodeID());

	int bufferStatusReport = msg->GetBufferStatusReport();

	if (record != NULL)
	{
		record->SetSchedulingRequest(bufferStatusReport);
	}

	delete msg;
}

PacketScheduler* EnbMacEntity::GetUplinkPacketScheduler(void)
{
	return m_uplinkScheduler;
}

void EnbMacEntity::SetUplinkPacketScheduler(PacketScheduler* s)
{
	m_uplinkScheduler = s;
}

PacketScheduler* EnbMacEntity::GetDownlinkPacketScheduler(void)
{
	return m_downlinkScheduler;
}

void EnbMacEntity::SetDownlinkPacketScheduler(PacketScheduler* s)
{
	m_downlinkScheduler = s;
}

void EnbMacEntity::ReceiveCqiIdealControlMessage(CqiIdealControlMessage* msg)
{
  CqiIdealControlMessage::CqiFeedbacks *cqi = msg->GetMessage();

  UserEquipment* ue = (UserEquipment*) msg->GetSource();
  ENodeB* enb = (ENodeB*) GetDevice();
  ENodeB::UserEquipmentRecord* record = enb->GetUserEquipmentRecord(ue->GetNodeID());

  if (record != NULL)
  {
	  if (ue->GetD2DManager()->D2DinUse()) record->SetUplinkChannelStatusIndicator(msg->measuredSinr);
	  else
	  {
		std::vector<int> cqiFeedback;
		for (CqiIdealControlMessage::CqiFeedbacks::iterator it = cqi->begin(); it != cqi->end(); it++)
		  cqiFeedback.push_back((*it).m_cqi);
		record->SetCQI(cqiFeedback);
	  }
  }

  delete msg;
}

void EnbMacEntity::ReceiveQoeIdealControlMessage(QoeIdealControlMessage* msg)
{
  
	UserEquipment* ue1;
	UserEquipment* ue2;
	std::vector<Application*>* apps = FlowsManager::Init()->GetApplicationsContainer();
	for (int i = 0 ; i < apps->size() ; i ++)
		if (apps->at(i)->GetApplicationDestination()->GetNodeID() == msg->GetSource()->GetNodeID())
			ue1 = apps->at(i)->GetApplicationSource();
	ue2 = (UserEquipment*) msg->GetSource();

	double mos = msg->GetMos();

	ENodeB* enb = (ENodeB*) GetDevice();
	ENodeB::UserEquipmentRecord* record1 = enb->GetUserEquipmentRecord(ue1->GetNodeID());
	ENodeB::UserEquipmentRecord* record2 = enb->GetUserEquipmentRecord(ue2->GetNodeID());
	delete msg;
	if (ue1->GetD2DManager()->D2DinUse())
	{
		std::ofstream out;
		int m;
		std::vector<Application*>* apps = FlowsManager::Init()->GetApplicationsContainer();
		for (m = 1 ; m <= apps->size() ; m ++) if ((apps->at(m-1)->GetApplicationDestination()->GetNodeID() == ue2->GetNodeID()) ||
				(apps->at(m-1)->GetApplicationSource()->GetNodeID() == ue1->GetNodeID())) break;
		out.open("D2DManagementOutput.txt", std::ofstream::out | std::ofstream::app);
				out << m << " D2D " << std::fixed << std::setprecision(2) << record1->GetQoe() <<
				" " << mos << " " << std::fixed << std::setprecision(1) <<
				1000*ue1->GetPosition()->CalculateDistance(ue2) << "\t" <<
				std::fixed << std::setprecision(3) << Simulator::Init()->Now();

		//stop d2d communication if d2d mos has gotten low
		if ((mos < record1->GetQoe()) || (mos == 1.0))
		{
			out << "\tD2D STOP" << std::endl;
			enb->GetD2DManager()->StopD2DCommunication(ue1, ue2);
		}
		else out << std::endl;
		out.close();
		return;
	}
	else
	{
		if (record1 != NULL) record1->SetQoe(mos);
		if (record2 != NULL) record2->SetQoe(mos);
	}

	//test d2d communication if d2d is enabled for the simulation
	if (((ENodeB*) GetDevice())->CheckD2DAvailability())
		if (!(ue1->GetD2DManager()->D2DinUse()))
		{
			//create a new channel realization for the two devices
			MacroCellUrbanAreaChannelRealization* c_ul = new MacroCellUrbanAreaChannelRealization(ue1, ue2);
			LteChannel* ulCh = GetDevice()->GetPhy()->GetUlChannel();
			PropagationLossModel* plm = ulCh->GetPropagationLossModel();
			plm->AddChannelRealization(c_ul);
			
			D2DIdealControlMessage *nmsg = new D2DIdealControlMessage();
			nmsg->SetD2DMessageType(D2DIdealControlMessage::NEED_TEST);
			nmsg->SetSource(GetDevice());
			nmsg->SetDestination(ue1);
			nmsg->SetTestData(ue2);
			GetDevice()->GetPhy()->SendIdealControlMessage(nmsg);
			delete nmsg;
		}	
}

