#include "PdcchMapIdealControlMessage.h"

PdcchMapIdealControlMessage::PdcchMapIdealControlMessage()
{
  m_idealPdcchMessage =  new IdealPdcchMessage();
  SetMessageType(IdealControlMessage::ALLOCATION_MAP);
}


PdcchMapIdealControlMessage::~PdcchMapIdealControlMessage()
{
  m_idealPdcchMessage->clear();
  delete m_idealPdcchMessage;
}


void PdcchMapIdealControlMessage::AddNewRecord(Direction direction, int subChannel, NetworkNode* ue, double mcs)
{
  IdealPdcchRecord record;
  record.m_direction = direction;
  record.m_idSubChannel = subChannel;
  record.m_mcsIndex = mcs;
  record.m_ue = ue;
  m_idealPdcchMessage->push_back(record);
}


PdcchMapIdealControlMessage::IdealPdcchMessage* PdcchMapIdealControlMessage::GetMessage(void)
{
  return m_idealPdcchMessage;
}
