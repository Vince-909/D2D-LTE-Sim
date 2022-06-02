#include "FrameManager.h"
#include "../Core/Simulator.h"
#include <vector>
#include "../Device/ENodeB.h"
#include "NetworkManager.h"
#include "../Device/UserEquipment.h"
#include "../Position/Position.h"
#include "../Position/Mobility.h"

FrameManager* FrameManager::ptr=NULL;

FrameManager::FrameManager()
{
  m_nbFrames = 0;
  m_nbSubframes = 0;
  m_TTICounter = 0;
  Simulator::Init()->Schedule(0.0, &FrameManager::Start, this);
}

FrameManager::~FrameManager()
{
}

void FrameManager::UpdateNbFrames(void)
{
  m_nbFrames++;
}

int FrameManager::GetNbFrames(void) const
{
  return m_nbFrames;
}

void FrameManager::UpdateNbSubframes(void)
{
  m_nbSubframes++;
}

void FrameManager::ResetNbSubframes(void)
{
  m_nbSubframes = 0;
}

int FrameManager::GetNbSubframes(void) const
{
  return m_nbSubframes;
}

void FrameManager::UpdateTTIcounter(void)
{
  m_TTICounter++;
}

unsigned long FrameManager::GetTTICounter() const
{
  return m_TTICounter;
}

void FrameManager::Start(void)
{
  Simulator::Init()->Schedule(0.0, &FrameManager::StartFrame, this);
}

void FrameManager::StartFrame(void)
{

  UpdateNbFrames ();
  Simulator::Init()->Schedule(0.0, &FrameManager::StartSubframe, this);
}

void FrameManager::StopFrame(void)
{
  Simulator::Init()->Schedule(0.0, &FrameManager::StartFrame, this);
}

void
FrameManager::StartSubframe(void)
{
  UpdateTTIcounter ();
  UpdateNbSubframes ();

  UpdateUserPosition();

  ResourceAllocation();

  Simulator::Init()->Schedule(0.001, &FrameManager::StopSubframe, this);
}

void FrameManager::StopSubframe(void)
{
  if (GetNbSubframes () == 10)
    {
	  ResetNbSubframes ();
	  Simulator::Init()->Schedule(0.0, &FrameManager::StopFrame, this);
    }
  else
    {
	  Simulator::Init()->Schedule(0.0, &FrameManager::StartSubframe, this);
    }
}


NetworkManager* FrameManager::GetNetworkManager(void)
{
  return NetworkManager::Init();
}

void
FrameManager::UpdateUserPosition(void)
{
	std::vector<UserEquipment*> *records = GetNetworkManager()->GetUserEquipmentContainer();
	std::vector<UserEquipment*>::iterator iter;
	UserEquipment* record;
	for (iter = records->begin (); iter != records->end (); iter++)
	{
		record = *iter;
		record->GetPosition()->GetMobility()->Move();
	}
}


void FrameManager::ResourceAllocation(void)
{
	std::vector<ENodeB*> *records = GetNetworkManager()->GetENodeBContainer();
	std::vector<ENodeB*>::iterator iter;
	ENodeB *record;
	for (iter = records->begin(); iter != records->end(); iter++)
	{
		record = *iter;
		Simulator::Init()->Schedule(0.0, &ENodeB::ResourceBlocksAllocation,record);
	}
}
