#include "FullbandCqiManager.h"
#include "../Device/UserEquipment.h"
#include "../Device/ENodeB.h"
#include "../Mac/AmcModule.h"
#include "../Mac/MacEntity.h"
#include <vector>
#include "../Icmp/CqiIdealControlMessage.h"
#include "../Physical/LtePhy.h"
#include "../Spectrum/BandwidthManager.h"
#include <iostream>

FullbandCqiManager::FullbandCqiManager()
{}

FullbandCqiManager::~FullbandCqiManager()
{
}

void FullbandCqiManager::CreateCqiFeedbacks(std::vector<double> sinr)
{

  UserEquipment* thisNode = (UserEquipment*) GetDevice();
  ENodeB* targetNode = thisNode->GetTargetEnb();

  AmcModule *amc = GetDevice()->GetMacEntity()->GetAmcModule();
  std::vector<int> cqi;
  for (int k = 0 ; k < sinr.size() ; k ++)
  {
	  if (sinr.at(k) == 0) cqi.push_back(0);
	  else cqi.push_back(amc->CreateCqiFeedback(sinr.at(k)));
  }

  CqiIdealControlMessage *msg = new CqiIdealControlMessage();
  msg->SetSource(thisNode);
  msg->SetDestination(targetNode);

  int nbSubChannels = cqi.size();
  std::vector<double> dlSubChannels = thisNode->GetPhy()->GetBandwidthManager()->GetDlSubChannels();

  for (int i = 0; i < nbSubChannels; i++) msg->AddNewRecord(dlSubChannels.at(i), cqi.at(i));


  SetLastSent();

  thisNode->GetPhy()->SendIdealControlMessage(msg);
}
