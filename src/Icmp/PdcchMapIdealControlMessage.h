#ifndef PDCCHMAPIDEALCONTROLMESSAGE_H_
#define PDCCHMAPIDEALCONTROLMESSAGE_H_

#include "IdealControlMessage.h"
#include <list>

class PdcchMapIdealControlMessage : public IdealControlMessage
{

public:

  PdcchMapIdealControlMessage();
  virtual ~PdcchMapIdealControlMessage();

  enum Direction
  {
	  DOWNLINK,
	  UPLINK
  };

  struct IdealPdcchRecord
  {
    Direction m_direction;
    int m_idSubChannel;
    NetworkNode* m_ue;
    double m_mcsIndex;
  };

  typedef std::list<struct IdealPdcchRecord>  IdealPdcchMessage;

  void AddNewRecord(Direction, int, NetworkNode*, double);

  IdealPdcchMessage* GetMessage();

private:

  IdealPdcchMessage* m_idealPdcchMessage;

};

#endif