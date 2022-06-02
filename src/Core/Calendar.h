#ifndef CALENDAR_H_
#define CALENDAR_H_

#include "Event.h"
#include <list>

class Event;

class Calendar
{

public:

	Calendar();
	virtual ~Calendar();

    typedef std::list<Event*> Events;

    Events* GetEvents(void);
	void InsertEvent(Event *newEvent);
	bool IsEmpty(void);
	Event* GetEvent(void);
	void RemoveEvent(void);

private:

	Events* m_events;

};

#endif
