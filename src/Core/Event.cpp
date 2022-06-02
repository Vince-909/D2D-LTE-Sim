#include "Event.h"

Event::Event()
{}

Event::~Event()
{}

void
Event::SetTimeStamp (double time)
{
  m_timeStamp = time;
}

double
Event::GetTimeStamp (void) const
{
  return m_timeStamp;
}

void
Event::SetUID (int uid)
{
  m_uid = uid;
}

int
Event::GetUID (void) const
{
  return m_uid;
}

