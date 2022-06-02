#include "PowerManager.h"
#include "../Physical/LtePhy.h"
#include <iostream>
#include "../Device/NetworkNode.h"
#include "../Icmp/PowerControlIdealControlMessage.h"
#include "../D2DManager/D2DManager.h"

#define ENB_T_POWER 43 //dbm
#define UE_T_POWER_CEL 23 //dbm
#define UE_T_POWER_D2D -19//dbm

PowerManager::PowerManager()
{
	m_phy = NULL;
}

void PowerManager::SetTxPower(void)
{
	NetworkNode::NodeType nodeType = GetPhy()->GetDevice()->GetNodeType();
	switch (nodeType)
	{
	case NetworkNode::TYPE_ENODEB:
		SetTxPowerForCellular(ENB_T_POWER);
		break;
	case NetworkNode::TYPE_UE:
		SetTxPowerForCellular(UE_T_POWER_CEL);
		SetTxPowerForD2D(UE_T_POWER_D2D);
		break;
	}
}

PowerManager::~PowerManager()
{
}

LtePhy* PowerManager::GetPhy(void)
{
	return m_phy;
}

void PowerManager::SetPhy(LtePhy* phy)
{
	m_phy = phy;
}

double PowerManager::GetTxPowerForD2D(void)
{
	return m_txPowerForD2D;
}

void PowerManager::SetTxPowerForD2D(double tp)
{
	m_txPowerForD2D = tp;
}

double PowerManager::GetTxPowerForCellular(void)
{
  return m_txPowerForCellular;
}

void PowerManager::SetTxPowerForCellular(double tp)
{
	if (tp > 23) m_txPowerForCellular = 23;
	else m_txPowerForCellular = tp;
}

void PowerManager::ReceivePowerControlIdealControlMessage(PowerControlIdealControlMessage* msg)
{
	if (!(GetPhy()->GetDevice()->GetD2DManager()->D2DinUse()))
	{
		if (msg->GetAction() == PowerControlIdealControlMessage::INCREASE_POWER)
			SetTxPowerForCellular(GetTxPowerForCellular() + 2);
		else SetTxPowerForCellular(GetTxPowerForCellular() - 2);
	}
	else
	{
		//do nothing, d2d transmission power is fixed
	}
	delete msg;
}