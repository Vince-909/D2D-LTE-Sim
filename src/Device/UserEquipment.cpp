#include "UserEquipment.h"
#include <iostream>

UserEquipment::UserEquipment()
	:NetworkNode()
{
	m_targetEnb = NULL;
	m_cqiManager = NULL;
}

UserEquipment::~UserEquipment()
{
}

ENodeB* UserEquipment::GetTargetEnb()
{
	return m_targetEnb;
}

void UserEquipment::SetTargetEnb(ENodeB* enb)
{
	m_targetEnb = enb;
}

void UserEquipment::SetCqiManager(CqiManager* cm)
{
	m_cqiManager = cm;
}

CqiManager* UserEquipment::GetCqiManager(void)
{
	return m_cqiManager;
}
