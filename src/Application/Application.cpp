#include "Application.h"
#include <iostream>
#include "../Core/Simulator.h"

Application::Application()
{
	m_src = NULL;
	m_dest = NULL;
	m_bearer = NULL;
	m_appSink = NULL;
}

Application::~Application()
{
}

void Application::Destroy(void)
{
  m_src = NULL;
  m_dest = NULL;
  m_bearer = NULL;
  m_appSink = NULL;
}

double Application::GetStartTime(void)
{
	return m_startTime;
}

void Application::SetStartTime(double time)
{
  m_startTime = time;
  Simulator::Init()->Schedule(time, &Application::Start, this);
}

double Application::GetStopTime()
{
	return m_stopTime;
}

void Application::SetStopTime(double time)
{
  m_stopTime = time;
  Simulator::Init()->Schedule(time + 0.1, &Application::Stop, this);
}

void Application::Start()
{
	DoStart();
}

void Application::Stop()
{
}

RadioBearer* Application::GetRadioBearer(void)
{
	return m_bearer;
}

void Application::SetRadioBearer(RadioBearer* bearer)
{
	m_bearer = bearer;
}

UserEquipment* Application::GetApplicationSource(void)
{
	return m_src;
}

void Application::SetApplicationSource(UserEquipment* src)
{
	m_src = src;
}

UserEquipment* Application::GetApplicationDestination(void)
{
	return m_dest;
}

void Application::SetApplicationDestination(UserEquipment* dst)
{
	m_dest = dst;
}
	
Application::ApplicationType Application::GetApplicationType(void)
{
	return m_appType;
}

void Application::SetApplicationType(ApplicationType type)
{
	m_appType = type;
}
	
int Application::GetApplicationID(void)
{
	return idApp;
}

void Application::SetApplicationID(int id)
{
	idApp = id;
}
	
ApplicationSink* Application::GetApplicationSink(void)
{
	return m_appSink;
}

void Application::SetApplicationSink(ApplicationSink* sink)
{
	m_appSink = sink;
}

