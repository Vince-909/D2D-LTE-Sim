#include "PowerControlIdealControlMessage.h"
#include "IdealControlMessage.h"

PowerControlIdealControlMessage::PowerControlIdealControlMessage()
{
	SetMessageType(IdealControlMessage::POWER_CONTROL);
}

PowerControlIdealControlMessage::~PowerControlIdealControlMessage()
{
}

PowerControlIdealControlMessage::Action PowerControlIdealControlMessage::GetAction(void)
{
	return m_action;
}

void PowerControlIdealControlMessage::SetAction(PowerControlIdealControlMessage::Action a)
{
	m_action = a;
}
