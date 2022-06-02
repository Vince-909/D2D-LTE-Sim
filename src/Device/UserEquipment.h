#ifndef USEREQUIPMENT_H_
#define USEREQUIPMENT_H_

#include "NetworkNode.h"

class ENodeB;
class CqiManager;

class UserEquipment : public NetworkNode
{

public:

	UserEquipment();
	~UserEquipment();

	ENodeB* GetTargetEnb(void);
	void SetTargetEnb(ENodeB*);

	void SetCqiManager(CqiManager*);
	CqiManager* GetCqiManager(void);

	void Visualize(void);

private:

	ENodeB* m_targetEnb;
	CqiManager* m_cqiManager;

};

#endif
