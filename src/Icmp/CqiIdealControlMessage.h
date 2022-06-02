#ifndef CQIIDEALCONTROLMESSAGE_H_
#define CQIIDEALCONTROLMESSAGE_H_

#include "IdealControlMessage.h"
#include <list>
#include <vector>

class CqiIdealControlMessage : public IdealControlMessage
{

public:

	CqiIdealControlMessage(void);
	virtual ~CqiIdealControlMessage(void);

	struct CqiFeedback
	{
		int m_idSubChannel;
		double m_cqi;
	};

	typedef std::list<struct CqiFeedback> CqiFeedbacks;

	void AddNewRecord(int, double);
	CqiFeedbacks* GetMessage(void);

	void SetisD2DUplinkChannelStatusIndicator(bool);
	bool IsD2DUplinkChannelStatusIndicator(void);

	std::vector<double> measuredSinr;

private:

  CqiFeedbacks *m_cqiFeedbacks;
  bool isD2DUplinkChannelStatusIndicator;

};

#endif