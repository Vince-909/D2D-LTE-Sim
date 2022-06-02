#ifndef QOEIDEALCONTROLMESSAGE_H_
#define QOEIDEALCONTROLMESSAGE_H_

#include "IdealControlMessage.h"

class QoeIdealControlMessage : public IdealControlMessage
{
	
public:

	QoeIdealControlMessage();
	~QoeIdealControlMessage();

	double GetMos(void);
	void SetMos(double);

private:

	double m_mos;

};

#endif