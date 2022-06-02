#include "D2DIdealControlMessage.h"
#include "IdealControlMessage.h"
#include "../Device/UserEquipment.h"
#include <vector>

D2DIdealControlMessage::D2DIdealControlMessage()
{
	SetMessageType(IdealControlMessage::D2D);
}

D2DIdealControlMessage::~D2DIdealControlMessage()
{
}

D2DIdealControlMessage::D2DMessageType D2DIdealControlMessage::GetD2DMessageType(void)
{
	return m_d2dType;
}

void D2DIdealControlMessage::SetD2DMessageType(D2DIdealControlMessage::D2DMessageType type)
{
	m_d2dType = type;
}

D2DIdealControlMessage::TestData D2DIdealControlMessage::GetTestData(void)
{
	return m_testData;
}

void D2DIdealControlMessage::SetTestData(UserEquipment* source, std::vector<int> channelsTx, std::vector<int> mcsIndexTx, std::vector<double> values)
{
	m_testData.sourceUe = source;
	m_testData.channelsTx.clear();
	for (int i = 0 ; i < channelsTx.size() ; i ++) m_testData.channelsTx.push_back(channelsTx.at(i));
	m_testData.mcsIndexTx.clear();
	for (int i = 0 ; i < mcsIndexTx.size() ; i ++) m_testData.mcsIndexTx.push_back(mcsIndexTx.at(i));
	m_testData.txSignalValues.clear();
	for (int i = 0 ; i < values.size() ; i ++) m_testData.txSignalValues.push_back(values.at(i));
}

void D2DIdealControlMessage::SetTestData(double result)
{
	m_testData.result = result;
}

void D2DIdealControlMessage::SetTestData(UserEquipment* destUe)
{
	m_testData.destinationUe = destUe;
}

D2DIdealControlMessage::D2DRole D2DIdealControlMessage::GetD2DRole(void)
{
	return m_D2DRole;
}

void D2DIdealControlMessage::SetD2DRole(D2DIdealControlMessage::D2DRole role)
{
	m_D2DRole = role;
}

void D2DIdealControlMessage::SetChannels(std::vector<int>* ch)
{
	channelstoReceive = ch;
}

std::vector<int>* D2DIdealControlMessage::GetChannels(void)
{
	return channelstoReceive;
}

void D2DIdealControlMessage::SetMcs(std::vector<int>* mcs)
{
	mcsIndextoReceive = mcs;
}

std::vector<int>* D2DIdealControlMessage::GetMcs(void)
{
	return mcsIndextoReceive;
}