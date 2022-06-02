#include "CqiIdealControlMessage.h"

CqiIdealControlMessage::CqiIdealControlMessage()
{
  m_cqiFeedbacks = new CqiFeedbacks();
  isD2DUplinkChannelStatusIndicator = false;
  SetMessageType(IdealControlMessage::CQI_FEEDBACKS);
}

CqiIdealControlMessage::~CqiIdealControlMessage()
{
  m_cqiFeedbacks->clear();
  delete m_cqiFeedbacks;
}

void CqiIdealControlMessage::AddNewRecord(int subChannel, double cqi)
{
  CqiFeedback c;
  c.m_idSubChannel = subChannel;
  c.m_cqi = cqi;

  m_cqiFeedbacks->push_back(c);

}

CqiIdealControlMessage::CqiFeedbacks* CqiIdealControlMessage::GetMessage()
{
  return m_cqiFeedbacks;
}

bool CqiIdealControlMessage::IsD2DUplinkChannelStatusIndicator(void)
{
	return isD2DUplinkChannelStatusIndicator;
}

void CqiIdealControlMessage::SetisD2DUplinkChannelStatusIndicator(bool csi)
{
	isD2DUplinkChannelStatusIndicator = csi;
}