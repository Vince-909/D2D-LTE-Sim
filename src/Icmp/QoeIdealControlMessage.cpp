#include "QoeIdealControlMessage.h"
#include "IdealControlMessage.h"

QoeIdealControlMessage::QoeIdealControlMessage()
{
	SetMessageType(IdealControlMessage::QOE_REPORT);
}

QoeIdealControlMessage::~QoeIdealControlMessage()
{
}

double QoeIdealControlMessage::GetMos(void)
{
	return m_mos;
}

void QoeIdealControlMessage::SetMos(double mos)
{
	m_mos = mos;
}