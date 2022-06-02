#ifndef CHANNELREALIZATION_H_
#define CHANNELREALIZATION_H_

#include <vector>

class NetworkNode;

class ChannelRealization
{

public:

	ChannelRealization();
	virtual ~ChannelRealization();
	void Destroy();

	void SetSourceNode(NetworkNode*);
	NetworkNode* GetSourceNode(void);
	void SetDestinationNode(NetworkNode*);
	NetworkNode* GetDestinationNode(void);

	void SetLastUpdate(void);
	double GetLastUpdate(void);
	void SetSamplingPeriod(double);
	double GetSamplingPeriod(void);

	bool NeedForUpdate(void);
	virtual void UpdateModels(void) = 0;

	virtual std::vector<double> GetLoss() = 0;

private:

	NetworkNode* m_src;
	NetworkNode* m_dst;

    double m_lastUpdate;
	double m_samplingPeriod;

};

#endif