#ifndef BANDWIDTHMANAGER_H_
#define BANDWIDTHMANAGER_H_

#include <vector>

class BandwidthManager
{

public:

	BandwidthManager(double, double, int, int);
	virtual ~BandwidthManager();

	std::vector<double> GetDlSubChannels(void);

	std::vector<double> GetUlSubChannels(void);

	int GetOperativeSubBand(void);

	double GetUlBandwidth(void);
	double GetDlBandwidth(void);
	int GetUlOffsetBw(void);
	int GetDlOffsetBw(void);

private:

	std::vector<double> m_dlSubChannels;
	std::vector<double> m_ulSubChannels;

	int m_operativeSubBand;

	double m_ulBandwidth;
	double m_dlBandwidth;

	int m_ulOffsetBw;
	int m_dlOffsetBw;

};

#endif