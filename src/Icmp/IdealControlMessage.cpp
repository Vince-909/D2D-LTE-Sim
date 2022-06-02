#include "IdealControlMessage.h"
#include <iostream>

IdealControlMessage::IdealControlMessage()
{
	m_source = NULL;
	m_destination = NULL;
}

IdealControlMessage::~IdealControlMessage()
{
}

NetworkNode* IdealControlMessage::GetSource(void)
{
	return m_source;
}

void IdealControlMessage::SetSource(NetworkNode* node)
{
	m_source = node;
}

NetworkNode* IdealControlMessage::GetDestination(void)
{
	return m_destination;
}

void IdealControlMessage::SetDestination(NetworkNode* node)
{
	m_destination = node;
}

IdealControlMessage::MessageType IdealControlMessage::GetMessageType(void)
{
	return m_type;
}

void IdealControlMessage::SetMessageType(MessageType type)
{
	m_type = type;
}