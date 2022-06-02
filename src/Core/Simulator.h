#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Event.h"
#include "Calendar.h"
#include "MakeEvent.h"

#include <stdint.h>
#include <string>
#include <iostream>


/*
 * Simulator
 * Control the scheduling of simulation events.
 */

class Simulator 
{
private:
	Simulator ();
	static Simulator *ptr;

	Calendar *m_calendar;
	bool m_stop;
	int m_currentUid;
	double m_currentTs;
	int m_unscheduledEvents;

	int m_uid;

	void ProcessOneEvent (void);

public:
	virtual ~Simulator ();

	static Simulator*
	Init (void)
	  {
		if (ptr==NULL)
	      {
		    ptr = new Simulator;
	   	  }
		return ptr;
	  }

	double Now (void);

	void Run (void);
	void Stop (void);
	void SetStop (double time);

	int GetUID (void);

	void
	DoSchedule (double time, Event *event);


	/*
	 * Schedule methods are called to insert a new method
	 * into the calendar scheduler
	 */
	template <typename MEM, typename OBJ>
	void
	Schedule (double time, MEM mem_ptr, OBJ obj);

	template <typename MEM, typename OBJ, typename T1>
	void
	Schedule (double time, MEM mem_ptr, OBJ obj, T1 a1);

	template <typename MEM, typename OBJ, typename T1, typename T2>
	void
	Schedule (double time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2);

	template <typename MEM, typename OBJ, typename T1, typename T2, typename T3>
	void
	Schedule (double time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3);

	template <typename MEM, typename OBJ, typename T1, typename T2, typename T3, typename T4>
	void
	Schedule (double time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4);

	void
	Schedule (double time, void (*f) (void));

	template <typename U1>
	void
	Schedule (double time, void (*f) (U1));

	template <typename U1, typename T1>
	void
	Schedule (double time, void (*f) (U1), T1 a1);

};


template <typename MEM, typename OBJ>
void
Simulator::Schedule (double time, MEM mem_ptr, OBJ obj)
{
  DoSchedule (time, MakeEvent (mem_ptr, obj));
}

template <typename MEM, typename OBJ, typename T1>
void
Simulator::Schedule (double time, MEM mem_ptr, OBJ obj, T1 a1)
{
  DoSchedule (time, MakeEvent(mem_ptr, obj, a1));
}

template <typename MEM, typename OBJ, typename T1, typename T2>
void
Simulator::Schedule (double time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2)
{
  DoSchedule (time, MakeEvent(mem_ptr, obj, a1, a2));
}

template <typename MEM, typename OBJ, typename T1, typename T2, typename T3>
void
Simulator::Schedule (double time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3)
{
  DoSchedule (time, MakeEvent(mem_ptr, obj, a1, a2, a3));
}

template <typename MEM, typename OBJ, typename T1, typename T2, typename T3, typename T4>
void
Simulator::Schedule (double time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4)
{
  DoSchedule (time, MakeEvent(mem_ptr, obj, a1, a2, a3, a4));
}

template <typename U1, typename T1>
void
Simulator::Schedule (double time, void (*f) (U1), T1 a1)
{
  DoSchedule (time, MakeEvent(f, a1));
}




#endif


