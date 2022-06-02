#ifndef MACENTITY_H_
#define MACENTITY_H_

class NetworkNode;
class AmcModule;

class MacEntity
{

public:

  MacEntity();
  virtual ~MacEntity();

  void Destroy(void);
  
  NetworkNode* GetDevice();
  void SetDevice(NetworkNode*);
  
  AmcModule* GetAmcModule(void);
  void SetAmcModule(AmcModule*);

private:

  NetworkNode* m_device;
  AmcModule* m_amcModule;

};

#endif