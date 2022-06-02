#include "QoeManager.h"
#include "Emodel.h"
#include "../Icmp/QoeIdealControlMessage.h"
#include "../Application/Application.h"
#include <iostream>
#include "../Device/UserEquipment.h"
#include "../Physical/LtePhy.h"
#include "../Core/Simulator.h"
#include "../Application/ApplicationSink.h"
#include "../D2DManager/D2DManager.h"

QoeManager::QoeManager()
{
	m_appSink = NULL;
	m_sentPackets = 0;
	m_totalSentPackets = 0;
	m_receivedPackets = 0;
	m_totalReceivedPackets = 0;
	m_delay = 0;
	m_totalDelay = 0;
	m_lastMos = 2.5;
	m_reportingIntervalCel = 10;
	m_reportingIntervalD2D = 2;
}

QoeManager::~QoeManager()
{
}

ApplicationSink* QoeManager::GetApplicationSink(void)
{
	return m_appSink;
}

void QoeManager::SetApplicationSink(ApplicationSink* apps)
{
	m_appSink = apps;
}

double QoeManager::CalculateMos(double delay, double packetLoss)
{
	double mos = MOS(1000*delay, packetLoss);
	return mos < 1 ? 1 : floor(mos*100)/100.0; 
}

void QoeManager::UpdateSentPackets(void)
{
	m_sentPackets ++;
	m_totalSentPackets ++;
}

void QoeManager::UpdateReceivedPackets(void)
{
	m_receivedPackets ++;
	m_totalReceivedPackets ++;
}

void QoeManager::UpdateDelay(double delay)
{
	m_delay += delay;
	m_totalDelay += delay;
}

void QoeManager::ResetSentPackets(void)
{
	m_sentPackets = 0;
}

void QoeManager::ResetReceivedPackets(void)
{
	m_receivedPackets = 0;
}

void QoeManager::ResetDelay(void)
{
	m_delay = 0;
}

int QoeManager::GetTotalSentPackets(void)
{
	return m_totalSentPackets;
}

int QoeManager::GetTotalReceivedPackets(void)
{
	return m_totalReceivedPackets;
}

double QoeManager::GetTotalDelay(void)
{
	return m_totalDelay;
}

void QoeManager::ReportQoe(void)
{
	QoeIdealControlMessage* msg = new QoeIdealControlMessage;
	msg->SetSource((NetworkNode*) GetApplicationSink()->GetApplication()->GetApplicationDestination());
	msg->SetDestination((NetworkNode*) GetApplicationSink()->GetApplication()->GetApplicationDestination()->GetTargetEnb());
	double mos;
	if (m_sentPackets == 0) mos = m_lastMos; //no mos can be calculated if no voice has been transmitted
	else
	{
		if (m_receivedPackets == 0) mos = CalculateMos(0, 1);
		else mos = CalculateMos(m_delay/m_receivedPackets, 1 - ((double) m_receivedPackets/m_sentPackets));
		//std::cout << "mos: " << m_delay/m_receivedPackets << " " << (double) m_receivedPackets/m_sentPackets << std::endl;
		m_lastMos = mos;
	}
	msg->SetMos(mos);
	msg->GetSource()->GetPhy()->SendIdealControlMessage(msg);
	//reset packets for next reporting interval
	ResetSentPackets();
	ResetReceivedPackets();
	ResetDelay();
	
	//schedule next report
	Simulator::Init()->Schedule(GetReportingInterval(), &QoeManager::ReportQoe, this);
}

double QoeManager::GetReportingInterval(void)
{
	if (GetApplicationSink()->GetApplication()->GetApplicationDestination()->GetD2DManager()->D2DinUse())
		return m_reportingIntervalD2D;
	else
		return m_reportingIntervalCel;
}
