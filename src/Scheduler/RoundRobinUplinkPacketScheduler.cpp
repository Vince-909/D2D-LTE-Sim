#include "RoundRobinUplinkPacketScheduler.h"
#include "../Mac/MacEntity.h"
#include "../Device/NetworkNode.h"
#include "../Physical/LtePhy.h"
#include "../Spectrum/BandwidthManager.h"
#include "../Mac/AmcModule.h"
#include "../Utility/EesmEffectiveSinr.h"
#include <iostream>

RoundRobinUplinkPacketScheduler::RoundRobinUplinkPacketScheduler()
{
  SetMacEntity(NULL);
  CreateUsersToSchedule();

  m_roundRobinId = 0;
}

RoundRobinUplinkPacketScheduler::~RoundRobinUplinkPacketScheduler()
{
  Destroy();
}

void RoundRobinUplinkPacketScheduler::RBsAllocation()
{
	
	UsersToSchedule *users = GetUsersToSchedule();

	int usersWithData = 0;
	for (int x = 0 ; x < GetUsersToSchedule()->size() ; x ++)
	{
		if (GetUsersToSchedule()->at(x)->m_dataToTransmit > 0)
		{
			usersWithData += 1;
		}
	}

	int nbOfRBs = GetMacEntity()->GetDevice()->GetPhy()->GetBandwidthManager()->GetUlSubChannels().size();
	
	//RBs allocation
	int nbPrbToAssign = 5;
	int stop_nbOfRBs = nbOfRBs;
	if ((nbOfRBs / usersWithData) > nbPrbToAssign)
    {
	  nbPrbToAssign = ceil((double)(nbOfRBs / usersWithData));
	  stop_nbOfRBs = nbPrbToAssign * usersWithData;
    }

	int s = 0;
	while (s < stop_nbOfRBs)
    {
		if (m_roundRobinId >= users->size()) m_roundRobinId = 0; //restart again from the beginning
		while (users->at(m_roundRobinId)->m_dataToTransmit == 0)
		{
			m_roundRobinId++;
			if (m_roundRobinId >= users->size()) m_roundRobinId = 0;
		}

		UserToSchedule* scheduledUser = users->at(m_roundRobinId);
		std::vector<double> sinrs;
		for (int i = 0; i < nbPrbToAssign; i++)
	    {
			double chCondition = scheduledUser->m_channelContition.at(s+i);
			double sinr = chCondition;
			sinrs.push_back(sinr);
			scheduledUser->m_listOfAllocatedRBs.push_back(s+i);
	    }
		double effectiveSinr =  GetEesmEffectiveSinr(sinrs);
		
		int mcs = GetMacEntity()->GetAmcModule()->GetMCSFromCQI(GetMacEntity()->GetAmcModule()->GetCQIFromSinr(effectiveSinr));
		int tbs = ((GetMacEntity()->GetAmcModule()->GetTBSizeFromMCS(mcs, nbPrbToAssign)) / 8);
		scheduledUser->m_transmittedData = tbs;
		scheduledUser->m_selectedMCS = mcs;
		s = s + nbPrbToAssign;
		m_roundRobinId++;


    }
}