#ifndef POWERCONTROLIDEALCONTROLMESSAGE_H_
#define POWERCONTROLIDEALCONTROLMESSAGE_H_

#include "IdealControlMessage.h"

class PowerControlIdealControlMessage : public IdealControlMessage
{

public:

	PowerControlIdealControlMessage();
	~PowerControlIdealControlMessage();

	enum Action
	{
		INCREASE_POWER,
		DECREASE_POWER
	};

	void SetAction(Action);
	Action GetAction(void);

private:

	Action m_action;

};


#endif
