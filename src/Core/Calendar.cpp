#include "Calendar.h"
#include <iostream>

Calendar::Calendar()
{
  m_events = new Events;
}

Calendar::~Calendar()
{
  delete m_events;
}

Calendar::Events*
Calendar::GetEvents (void)
{
  return m_events;
}

void
Calendar::InsertEvent (Event *newEvent)
{
  Events *events = GetEvents ();
  Event *event;
  Events::iterator iter;

  if (IsEmpty ())
    {
	  events->push_front(newEvent);
	  return;
    }

  for (iter = events->begin(); iter != events->end(); iter++)
	{
	  event = *iter;
	  if(newEvent->GetTimeStamp() < event->GetTimeStamp())
	    {
		  m_events->insert(iter, newEvent);
		  return;
	    }
	}

  m_events->push_back(newEvent);;
}

bool
Calendar::IsEmpty (void)
{
  return GetEvents ()->empty();
}

Event*
Calendar::GetEvent (void)
{
  if (IsEmpty ())
	return NULL;

  Event *event = GetEvents ()->front ();
  return event;
}

void
Calendar::RemoveEvent (void)
{
  if (!IsEmpty ())
    {
	  Event *event = GetEvents ()->front();
	  GetEvents ()->pop_front ();
	  delete event;
    }
}


