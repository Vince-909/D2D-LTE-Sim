#ifndef QOEMANAGER_H_
#define QOEMANAGER_H_

class ApplicationSink;

class QoeManager
{

public:

	QoeManager();
	~QoeManager();

	ApplicationSink* GetApplicationSink(void);
	void SetApplicationSink(ApplicationSink*);

	double CalculateMos(double, double);

	void UpdateSentPackets(void);
	void UpdateReceivedPackets(void);
	void UpdateDelay(double);

	void ResetSentPackets(void);
	void ResetReceivedPackets(void);
	void ResetDelay(void);
	
	int GetTotalSentPackets(void);
	int GetTotalReceivedPackets(void);
	double GetTotalDelay(void);

	void ReportQoe(void);

	double GetReportingInterval(void);

private:

	ApplicationSink* m_appSink;
	int m_sentPackets;
	int m_totalSentPackets;
	int m_receivedPackets;
	int m_totalReceivedPackets;
	double m_delay;
	double m_totalDelay;
	double m_lastMos;
	double m_reportingIntervalCel;
	double m_reportingIntervalD2D;

};

#endif