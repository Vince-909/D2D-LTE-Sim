#ifndef VIDEOSTREAM_H_
#define VIDEOSTREAM_H_

#include "Application.h"
#include <vector>

class VideoStream : public Application
{

	std::vector<int> m_sizes;
	int m_currentSize;

public:

	VideoStream();
	virtual ~VideoStream();

	bool LoadFile(char*);

	void DoStart(void);
	void DoStop(void);

    void ScheduleTransmit(double);
    void Send(void);

	std::vector<int> GetSizes(void);

};

#endif