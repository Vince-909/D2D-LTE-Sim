#include "D2DFairUplinkPacketScheduler.h"
#include "../Mac/MacEntity.h"
#include "../Device/NetworkNode.h"
#include "../Physical/LtePhy.h"
#include "../Spectrum/BandwidthManager.h"
#include "../Mac/AmcModule.h"
#include "../Utility/EesmEffectiveSinr.h"
#include "../D2DManager/D2DManager.h"
#include <iostream>

D2DFairUplinkPacketScheduler::D2DFairUplinkPacketScheduler()
{
  SetMacEntity(NULL);
  CreateUsersToSchedule();

  m_roundRobinId = 0;
}

D2DFairUplinkPacketScheduler::~D2DFairUplinkPacketScheduler()
{
  Destroy();
}

void D2DFairUplinkPacketScheduler::RBsAllocation()
{
	
	UsersToSchedule *users = GetUsersToSchedule();

	std::cout << "\nNEW ALLOX FOR: " << users->size() << std::endl;
	int nbOfRBs = GetMacEntity()->GetDevice()->GetPhy()->GetBandwidthManager()->GetUlSubChannels().size();
	
	//RBs allocation
	int nbPrbToAssign = 5;
	int stop_nbOfRBs = nbOfRBs;
	if ((nbOfRBs / users->size()) > nbPrbToAssign)
    {
	  nbPrbToAssign = ceil((double)(nbOfRBs / users->size()));
	  stop_nbOfRBs = nbPrbToAssign * users->size();
    }

	int s = 0;
	std::cout << "NB PRB: " << stop_nbOfRBs << std::endl;
	while (s < stop_nbOfRBs)
    {
		std::cout << "MPAINEI" << users->size() << std::endl;
		if (m_roundRobinId >= users->size()) m_roundRobinId = 0;
		while ((users->at(m_roundRobinId)->m_dataToTransmit == 0) || ((!users->at(m_roundRobinId)->skippedSched) && (!users->at(m_roundRobinId)->m_userToSchedule->GetD2DManager()->D2DinUse())))
		{
			std::cout << "hi: ID: " << m_roundRobinId << " SKIPPED: " << users->at(m_roundRobinId)->skippedSched << " DATA: " << users->at(m_roundRobinId)->m_dataToTransmit << std::endl;
			if (users->at(m_roundRobinId)->m_dataToTransmit != 0) users->at(m_roundRobinId)->skippedSched = true;
			m_roundRobinId++;
			if (m_roundRobinId >= users->size()) m_roundRobinId = 0;
		}
		std::cout << "MPAINEI" << std::endl;
		if (!users->at(m_roundRobinId)->m_userToSchedule->GetD2DManager()->D2DinUse()) users->at(m_roundRobinId)->skippedSched = false;
		if (m_roundRobinId >= users->size()) m_roundRobinId = 0; //restart again from the beginning
		std::cout << "RR ID: " << m_roundRobinId << std::endl;
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
		std::cout << "TBS: " << tbs << "MCS: " << mcs << std::endl;
		scheduledUser->m_transmittedData = tbs;
		scheduledUser->m_selectedMCS = mcs;
		std::cout << "GRAnted RES TO user: " << users->at(m_roundRobinId)->m_userToSchedule->GetNodeID() << std::endl;
		s = s + nbPrbToAssign;
		m_roundRobinId++;


    }
}