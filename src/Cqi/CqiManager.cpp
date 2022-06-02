#include "CqiManager.h"
#include "../Core/Simulator.h"

CqiManager::CqiManager()
{
  m_device  = 0;
}

CqiManager::~CqiManager()
{
  m_device = 0;
}

void CqiManager::SetDevice(NetworkNode* d)
{
  m_device = d;
}

NetworkNode* CqiManager::GetDevice(void)
{
  return m_device;
}

void CqiManager::SetCqiReportingMode(CqiManager::CQIReportingMode m)
{
  m_reportingMode = m;
  if (m_reportingMode == CqiManager::APERIODIC)
  {
	  m_sendCqi = false;
  }
  else
  {
	  m_sendCqi = true;
  }
}

CqiManager::CQIReportingMode CqiManager::GetCqiReportingMode(void)
{
  return m_reportingMode;
}

void CqiManager::SetSendCqi(bool b)
{
  m_sendCqi = b;
}

bool CqiManager::GetSendCqi(void)
{
  return m_sendCqi;
}


void CqiManager::SetReportingInterval(int i)
{
  m_reportingInterval = i;
}

int CqiManager::GetReportingInterval(void)
{
  return m_reportingInterval;
}

void CqiManager::SetLastSent()
{
  m_lastSent = long(Simulator::Init()->Now() * 1000); //TTI
}

long int CqiManager::GetLastSent(void)
{
  return m_lastSent;
}

bool CqiManager::NeedToSendFeedbacks(void)
{
  if (GetSendCqi() && GetReportingInterval() == 1)
  {
	  return true;
  }

  else if (GetSendCqi() && ((Simulator::Init()->Now()*1000) - GetLastSent()) >= GetReportingInterval())
  {
      return true;
  }
  else
    {
	  return false;
    }
}
