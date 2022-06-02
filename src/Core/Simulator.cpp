#include "Simulator.h"
#include "MakeEvent.h"
#include <math.h>
#include <fstream>
#include <list>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

Simulator* Simulator::ptr=NULL;

Simulator::Simulator ()
{
  m_stop = false;
  m_currentUid = 0;
  m_currentTs = 0;
  m_unscheduledEvents = 0;
  m_calendar = new Calendar;
  m_uid = 0;
}

Simulator::~Simulator ()
{
  while (!m_calendar->IsEmpty ())
    {
	  m_calendar->RemoveEvent ();
    }
  delete m_calendar;
}

double
Simulator::Now (void)
{
  return m_currentTs;
}


void 
Simulator::Run (void)
{

  m_stop = false;
  while (!m_calendar->IsEmpty () && !m_stop)
    {
      ProcessOneEvent ();
    }
}

void
Simulator::ProcessOneEvent(void)
{
  Event *next = m_calendar->GetEvent();

  --m_unscheduledEvents;

  m_currentTs = next->GetTimeStamp();
  m_currentUid = next->GetUID();

  next->RunEvent();

  m_calendar->RemoveEvent();
}

int
Simulator::GetUID (void)
{
  m_uid++;
  return (m_uid-1);
}

void 
Simulator::Stop (void)
{
  m_stop = true;
}

void 
Simulator::SetStop (double time)
{
  DoSchedule (time,
			  MakeEvent (&Simulator::Stop, this));
}


void
Simulator::DoSchedule (double time,
					   Event *event)
{

  double timeStamp = time + Now();
  event->SetTimeStamp(timeStamp);

  ++m_unscheduledEvents;

  m_calendar->InsertEvent(event);
}
