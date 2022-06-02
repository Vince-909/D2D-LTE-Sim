#include "SchedulingRequestIdealControlMessage.h"

SchedulingRequestIdealControlMessage::SchedulingRequestIdealControlMessage()
{
	SetMessageType(IdealControlMessage::SCHEDULING_REQUEST);
}

SchedulingRequestIdealControlMessage::~SchedulingRequestIdealControlMessage()
{
}

int SchedulingRequestIdealControlMessage::GetBufferStatusReport(void)
{
	return m_bufferStatusReport;
}

void SchedulingRequestIdealControlMessage::SetBufferStatusReport(int report)
{
	m_bufferStatusReport = report;
}