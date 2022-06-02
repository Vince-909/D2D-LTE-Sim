#include "MacEntity.h"
#include <iostream>
#include "AmcModule.h"

MacEntity::MacEntity ()
{
}

MacEntity::~MacEntity ()
{
  delete m_amcModule;
  m_device = NULL;
}

void MacEntity::Destroy()
{
  delete m_amcModule;
  m_device = NULL;
}

void MacEntity::SetDevice(NetworkNode* d)
{
  m_device = d;
}

NetworkNode* MacEntity::GetDevice()
{
  return m_device;
}

void MacEntity::SetAmcModule(AmcModule* amc)
{
  m_amcModule = amc;
}

AmcModule* MacEntity::GetAmcModule()
{
  return m_amcModule;
}
