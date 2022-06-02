#include "ENodeB.h"
#include "UserEquipment.h"
#include <iostream>
#include "../Mac/EnbMacEntity.h"
#include "../Scheduler/ProportionalFairDownlinkPacketScheduler.h"
#include "../Scheduler/RoundRobinUplinkPacketScheduler.h"
#include <vector>
#include "../Spectrum/BandwidthManager.h"
#include "../Physical/LtePhy.h"

ENodeB::ENodeB()
{
	m_userEquipmentRecords = new UserEquipmentRecords;
}

ENodeB::~ENodeB()
{
}

void ENodeB::ResourceBlocksAllocation()
{
	UplinkResourceBlockAllocation();
	DownlinkResourceBlockAllocation();
}

void ENodeB::DownlinkResourceBlockAllocation(void)
{
	if (GetDLScheduler() != NULL && GetNbOfUserEquipmentRecords() > 0)
	{
		GetDLScheduler()->Schedule();
	}
}

void ENodeB::UplinkResourceBlockAllocation(void)
{
	if (GetULScheduler() != NULL && GetNbOfUserEquipmentRecords() > 0)
	{
		GetULScheduler()->Schedule();
	}
}

ENodeB::UserEquipmentRecords* ENodeB::GetUserEquipmentRecords(void)
{
	return m_userEquipmentRecords;
}

ENodeB::UserEquipmentRecord* ENodeB::GetUserEquipmentRecord(int idUE)
{
	UserEquipmentRecords* records = GetUserEquipmentRecords();
	UserEquipmentRecord* record;
	UserEquipmentRecords::iterator iter;

	for (iter = records->begin(); iter != records->end(); iter++)
	{
		record = *iter;
		if (record->GetUE()->GetNodeID() == idUE)
		{
			return record;
		}
	}
	return false;
}

UserEquipment* ENodeB::UserEquipmentRecord::GetUE(void)
{
	return m_UE;
}

void ENodeB::UserEquipmentRecord::SetSchedulingRequest(int r)
{
	m_schedulingRequest = r;
}

ENodeB::UserEquipmentRecord::UserEquipmentRecord()
{
	m_UE = NULL;
	//Create initial CQI values:
	m_cqiFeedback.clear();
	m_uplinkChannelStatusIndicator.clear();
	m_schedulingRequest = 0;
	m_qoe = 2.5;
	m_averageSchedulingGrants = 1;
}

void ENodeB::UserEquipmentRecord::SetCQI(std::vector<int> cqi)
{
  m_cqiFeedback = cqi;
}

ENodeB::UserEquipmentRecord::~UserEquipmentRecord()
{
  m_cqiFeedback.clear();
  m_uplinkChannelStatusIndicator.clear();
}

void ENodeB::RegisterUserEquipment(UserEquipment *UE)
{
	UserEquipmentRecord* record = new UserEquipmentRecord(UE);
	GetUserEquipmentRecords()->push_back(record);
}

ENodeB::UserEquipmentRecord::UserEquipmentRecord(UserEquipment *UE)
{
  m_UE = UE;
  BandwidthManager* s = m_UE->GetPhy()->GetBandwidthManager();

  m_qoe = 2.5;

  int nbRbs = s->GetDlSubChannels().size();
  m_cqiFeedback.clear();
  for (int i = 0; i < nbRbs; i++ )
  {
	  m_cqiFeedback.push_back(10);
  }

  nbRbs = s->GetUlSubChannels().size();
  m_uplinkChannelStatusIndicator.clear();
  for (int i = 0; i < nbRbs; i++ )
    {
		m_uplinkChannelStatusIndicator.push_back(10.);
    }

  m_schedulingRequest = 0;
  m_averageSchedulingGrants = 1;

}

PacketScheduler* ENodeB::GetDLScheduler(void)
{
	EnbMacEntity *mac = (EnbMacEntity*) this->GetMacEntity();
	return mac->GetDownlinkPacketScheduler();
}

int ENodeB::UserEquipmentRecord::GetSchedulingGrants(void)
{
  return m_averageSchedulingGrants;
}

void ENodeB::UserEquipmentRecord::UpdateSchedulingGrants(int b)
{
  m_averageSchedulingGrants = (int)( (0.9 * m_averageSchedulingGrants)	+ (0.1 * b));
}

int ENodeB::UserEquipmentRecord::GetSchedulingRequest(void)
{
  return m_schedulingRequest;
}

std::vector<int> ENodeB::UserEquipmentRecord::GetCQI(void)
{
	return m_cqiFeedback;
}

double ENodeB::UserEquipmentRecord::GetQoe(void)
{
	return m_qoe;
}

void ENodeB::UserEquipmentRecord::SetQoe(double mos)
{
	m_qoe = mos;
}

void ENodeB::SetDLScheduler(ENodeB::DLSchedulerType type)
{
	EnbMacEntity* mac = (EnbMacEntity*) GetMacEntity();
	PacketScheduler* scheduler;
	switch (type)
    {
		case ENodeB::DLScheduler_TYPE_PROPORTIONAL_FAIR:
    		scheduler = new  ProportionalFairDownlinkPacketScheduler();
    		scheduler->SetMacEntity(mac);
    		mac->SetDownlinkPacketScheduler(scheduler);
	    break;
    }
}

PacketScheduler* ENodeB::GetULScheduler(void)
{
	EnbMacEntity *mac = (EnbMacEntity*) GetMacEntity();
	return mac->GetUplinkPacketScheduler();
}

void ENodeB::SetULScheduler(ULSchedulerType type)
{
	EnbMacEntity *mac = (EnbMacEntity*) GetMacEntity();
	PacketScheduler *scheduler;
	switch (type)
	{
		case ENodeB::ULScheduler_TYPE_ROUNDROBIN:
			scheduler = new RoundRobinUplinkPacketScheduler();
			scheduler->SetMacEntity(mac);
			mac->SetUplinkPacketScheduler(scheduler);
	    break;
	}
}

int ENodeB::GetNbOfUserEquipmentRecords(void)
{
	return GetUserEquipmentRecords()->size();
}

void ENodeB::UserEquipmentRecord::SetUplinkChannelStatusIndicator(std::vector<double> vet)
{
	for (int i = 0 ; i < m_uplinkChannelStatusIndicator.size() ; i ++)
	{
		if (vet.at(i) != 0) m_uplinkChannelStatusIndicator.at(i) = vet.at(i);
	}
}

std::vector<double> ENodeB::UserEquipmentRecord::GetUplinkChannelStatusIndicator(void)
{
	return m_uplinkChannelStatusIndicator;
}

bool ENodeB::CheckD2DAvailability(void)
{
	return useD2DCommunications;
}

void ENodeB::SetD2DAvailability(bool useD2D)
{
	useD2DCommunications = useD2D;
}