#ifndef CQIMANAGER_H_
#define CQIMANAGER_H_

#include <vector>

class NetworkNode;

class CqiManager
{

public:

    enum CQIReportingMode
    {
        PERIODIC,
        APERIODIC
    };

	CqiManager();
	virtual ~CqiManager();

	void SetDevice(NetworkNode*);
	NetworkNode* GetDevice(void);

	void SetCqiReportingMode(CQIReportingMode);
	CQIReportingMode GetCqiReportingMode(void);

	void SetSendCqi(bool);
	bool GetSendCqi(void);

	void SetReportingInterval(int);
	int GetReportingInterval(void);

	void SetLastSent(void);
	long int GetLastSent(void);

	virtual void CreateCqiFeedbacks(std::vector<double>) = 0;

	bool NeedToSendFeedbacks(void);

private:

	CQIReportingMode m_reportingMode;

	bool m_sendCqi;

	int m_reportingInterval;
	long int m_lastSent;

	NetworkNode* m_device;

};

#endif