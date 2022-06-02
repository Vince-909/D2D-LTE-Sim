#ifndef VOIP_H_
#define VOIP_H_

#include "Application.h"

class VoIP : public Application
{

	double m_interval;
	int m_size;
	bool m_stateON;
	double m_stateDuration;
	double m_endState;

public:

	VoIP();
	virtual ~VoIP();

	void DoStart(void);
	void DoStop(void);

    void ScheduleTransmit(double);
    void Send(void);

    int GetSize(void) const;

};

#endif