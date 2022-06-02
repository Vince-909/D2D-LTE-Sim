#include "UplinkPacketScheduler.h"
#include "../Device/ENodeB.h"
#include "../Mac/MacEntity.h"
#include "../Physical/LtePhy.h"
#include "../Spectrum/BandwidthManager.h"
#include "../Mac/AmcModule.h"
#include "../Icmp/PdcchMapIdealControlMessage.h"
#include "../Utility/EesmEffectiveSinr.h"
#include "../Device/NetworkNode.h"
#include "../D2DManager/D2DManager.h"
#include <iostream>

UplinkPacketScheduler::UplinkPacketScheduler()
{
}

UplinkPacketScheduler::~UplinkPacketScheduler()
{
	Destroy();
	DeleteUsersToSchedule();
}

void UplinkPacketScheduler::CreateUsersToSchedule(void)
{
	m_usersToSchedule = new UsersToSchedule();
}

void UplinkPacketScheduler::DeleteUsersToSchedule(void)
{
	if (m_usersToSchedule != NULL)
    {
		if (m_usersToSchedule->size() > 0) ClearUsersToSchedule();
		delete m_usersToSchedule;
    }
}

void UplinkPacketScheduler::ClearUsersToSchedule()
{
	UsersToSchedule*  records = GetUsersToSchedule();
	UsersToSchedule::iterator iter;

	for (iter = records->begin(); iter != records->end(); iter++)
	{
		delete *iter;
	}

	GetUsersToSchedule()->clear();
}

UplinkPacketScheduler::UsersToSchedule* UplinkPacketScheduler::GetUsersToSchedule(void)
{
	return m_usersToSchedule;
}

void UplinkPacketScheduler::SelectUsersToSchedule()
{

  CreateUsersToSchedule();

  ENodeB *node = (ENodeB*) GetMacEntity()->GetDevice();
  ENodeB::UserEquipmentRecords *records = node->GetUserEquipmentRecords();
  ENodeB::UserEquipmentRecord *record;
  ENodeB::UserEquipmentRecords::iterator iter;

  for (iter = records->begin(); iter != records->end(); iter++)
  {
	  record = *iter;
      //if (record->GetSchedulingRequest() > 0)
	   if ((((NetworkNode*) (record->GetUE()))->GetNodeID() % 2) == 0)
        {
			UserToSchedule* user = new UserToSchedule();
			user->m_userToSchedule = (NetworkNode*) record->GetUE();
			user->m_dataToTransmit = record->GetSchedulingRequest();
			user->m_listOfAllocatedRBs.clear();
			user->m_selectedMCS = 0;
			user->m_transmittedData = 0;
			user->m_channelContition = record->GetUplinkChannelStatusIndicator();
			user->m_averageSchedulingGrant = record->GetSchedulingGrants();
			GetUsersToSchedule ()->push_back(user);
        }
	}
}

void UplinkPacketScheduler::DoSchedule(void)
{
	SelectUsersToSchedule();
	//if (GetUsersToSchedule()->size() > 0)
	int usersWithData = 0;
	for (int x = 0 ; x < GetUsersToSchedule()->size() ; x ++)
	{
		if (GetUsersToSchedule()->at(x)->m_dataToTransmit > 0)
		{
			usersWithData += 1;
			break;
		}
	}
	if (usersWithData)
    {
		RBsAllocation();
		DoStopSchedule();
    }
	DeleteUsersToSchedule();
}

void UplinkPacketScheduler::RBsAllocation()
{
}

void
UplinkPacketScheduler::DoStopSchedule(void)
{
  //Finalize the allocation

  PdcchMapIdealControlMessage *pdcchMsg = new PdcchMapIdealControlMessage();
  UsersToSchedule* users = GetUsersToSchedule();

  for (UsersToSchedule::iterator it = users->begin(); it != users->end(); it++)
  {
	  UserToSchedule *user = (*it);

	  if (user->m_transmittedData > 0)
	  {

		  //create PDCCH messages
		  for (int rb = 0; rb < user->m_listOfAllocatedRBs.size(); rb++)
		  {
			  pdcchMsg->AddNewRecord (PdcchMapIdealControlMessage::UPLINK,
					                  user->m_listOfAllocatedRBs.at(rb),
									  user->m_userToSchedule,
									  user->m_selectedMCS);
		  }

		  //update users information
		  ENodeB *enb = (ENodeB*) GetMacEntity()->GetDevice();
		  ENodeB::UserEquipmentRecord* record = enb->GetUserEquipmentRecord(user->m_userToSchedule->GetNodeID());
		  record->m_schedulingRequest -= user->m_transmittedData;
		  if (record->m_schedulingRequest < 0)
		  {
			  record->m_schedulingRequest = 0;
		  }
		  record->UpdateSchedulingGrants(user->m_dataToTransmit);
	  }

  }

  if (pdcchMsg->GetMessage()->size() > 0)
  {
	  GetMacEntity()->GetDevice()->GetPhy()->SendIdealControlMessage(pdcchMsg);
  }

  delete pdcchMsg;
}