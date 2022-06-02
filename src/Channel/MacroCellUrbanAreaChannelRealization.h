#ifndef MACROCELLURBANAREACHANNELREALIZATION_H_
#define MACROCELLURBANAREACHANNELREALIZATION_H_

#include "ChannelRealization.h"

class NetworkNode;

class MacroCellUrbanAreaChannelRealization : public ChannelRealization
{

public:

	MacroCellUrbanAreaChannelRealization (NetworkNode*, NetworkNode*);
	virtual ~MacroCellUrbanAreaChannelRealization();
	void SetPenetrationLoss(double);
	double GetPenetrationLoss(void);
	double GetPathLoss(void);
	void SetShadowing(double);
	double GetShadowing(void);
	virtual void UpdateModels(void);

	virtual std::vector<double> GetLoss();

private:

	double m_penetrationLoss;
	double m_pathLoss;
	double m_shadowing;

};

#endif