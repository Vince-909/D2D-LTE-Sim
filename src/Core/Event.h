#ifndef EVENT_H_
#define EVENT_H_

class Event 
{

public:
	
	Event();
	virtual ~Event();

	void SetTimeStamp(double);
	double GetTimeStamp(void) const;

	void SetUID(int);
	int GetUID(void) const;

	virtual void
	RunEvent(void) = 0;

private:
	
	double m_timeStamp;
	int m_uid;

};

#endif
