#ifndef RRCENTITY_H
#define RRCENTITY_H

#include <vector>

class NetworkNode;
class RadioBearer;
class RadioBearerSink;

class RrcEntity
{

public:

  RrcEntity();
  virtual ~RrcEntity();

  void SetDevice (NetworkNode* d);
  NetworkNode* GetDevice ();

  typedef std::vector<RadioBearer* > RadioBearersContainer;
  typedef std::vector<RadioBearerSink* > RadioBearersSinkContainer;


  RadioBearersContainer* GetRadioBearerContainer(void);
  RadioBearersSinkContainer* GetRadioBearerSinkContainer(void);


  void AddRadioBearer(RadioBearer*);
  void DelRadioBearer(RadioBearer*);


  void AddRadioBearerSink(RadioBearerSink*);
  void DelRadioBearerSink(RadioBearerSink*);

private:

  RadioBearersContainer* m_bearers;
  RadioBearersSinkContainer* m_sink;
  NetworkNode* m_device;

};

#endif