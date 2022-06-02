#include "RRCEntity.h"
#include "RadioBearer.h"
#include "RadioBearerSink.h"
#include "../Application/Application.h"

RrcEntity::RrcEntity()
{
  m_bearers = new RadioBearersContainer();
  m_sink = new RadioBearersSinkContainer();
  m_device = NULL;
}

RrcEntity::~RrcEntity ()
{
  m_bearers->clear();
  delete m_bearers;

  m_sink->clear();
  delete m_sink;

  m_device = NULL;
}

void RrcEntity::SetDevice(NetworkNode* d)
{
  m_device = d;
}


NetworkNode* RrcEntity::GetDevice()
{
  return m_device;
}

RrcEntity::RadioBearersContainer* RrcEntity::GetRadioBearerContainer(void)
{
  return m_bearers;
}

RrcEntity::RadioBearersSinkContainer* RrcEntity::GetRadioBearerSinkContainer(void)
{
  return m_sink;
}


void RrcEntity::AddRadioBearer(RadioBearer* bearer)
{
  m_bearers->push_back (bearer);
}

void RrcEntity::DelRadioBearer(RadioBearer* bearer)
{
  RadioBearersContainer* newContainer = new RadioBearersContainer();
  RadioBearersContainer::iterator it;
  for (it = GetRadioBearerContainer ()->begin(); it != GetRadioBearerContainer ()->end(); it++)
	{
	  RadioBearer *b = (*it);
	  if (b->GetApplication()->GetApplicationID() != bearer->GetApplication()->GetApplicationID())
		{
		  newContainer->push_back(b);
		}
	}
  m_bearers->clear ();
  delete m_bearers;
  m_bearers = newContainer;
}

void RrcEntity::AddRadioBearerSink(RadioBearerSink* bearer)
{
  m_sink->push_back (bearer);
}

void
RrcEntity::DelRadioBearerSink(RadioBearerSink* bearer)
{
  RadioBearersSinkContainer* newContainer = new RadioBearersSinkContainer ();
  RadioBearersSinkContainer::iterator it;
  for (it = GetRadioBearerSinkContainer ()->begin(); it != GetRadioBearerSinkContainer ()->end(); it++)
	{
	  RadioBearerSink *b = (*it);
	  if (b->GetRadioBearer()->GetApplication()->GetApplicationID() != bearer->GetRadioBearer()->GetApplication()->GetApplicationID())
	    {
		  newContainer->push_back (b);
	    }
	}
  m_sink->clear ();
  delete m_sink;
  m_sink = newContainer;
}
