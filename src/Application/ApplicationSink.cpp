#include "ApplicationSink.h"
#include <iostream>
#include "../Packet/Packet.h"
#include "../Core/Simulator.h"
#include "Application.h"
#include "../Device/UserEquipment.h"
#include "../QualityOfExperience/Emodel.h"
#include "../Icmp/QoeIdealControlMessage.h"
#include "../Device/ENodeB.h"
#include "../Physical/LtePhy.h"
#include "../QualityOfExperience/QoeManager.h"
#include <iomanip>

ApplicationSink::ApplicationSink()
{
	m_app = NULL;
}

ApplicationSink::~ApplicationSink()
{
}

Application* ApplicationSink::GetApplication(void)
{
	return m_app;
}

void ApplicationSink::SetApplication(Application* app)
{
	m_app = app;
}

QoeManager* ApplicationSink::GetQoeManager(void)
{
	return m_qoeManager;
}

void ApplicationSink::SetQoeManager(QoeManager* manager)
{
	m_qoeManager = manager;
}

void ApplicationSink::Receive(Packet* packet)
{
	double now = Simulator::Init()->Now();
	double delay = now - packet->GetTimeStamp();
	
	GetQoeManager()->UpdateReceivedPackets();
	GetQoeManager()->UpdateDelay(delay);
	
	std::cout << GetApplication()->GetApplicationID() << " REC " << packet->GetId() << " "
		<< std::fixed << std::setprecision(3) << now << " " << delay << std::endl;

	delete packet;
}

