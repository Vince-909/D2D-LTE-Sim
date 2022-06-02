#include "TransmittedSignal.h"
#include <vector>

TransmittedSignal::TransmittedSignal()
{
}

TransmittedSignal::~TransmittedSignal()
{
}

void TransmittedSignal::SetValues(std::vector<double> values)
{
  m_values = values;
}

std::vector<double> TransmittedSignal::Getvalues(void)
{
  return m_values;
}

TransmittedSignal* TransmittedSignal::Copy(void)
{
  TransmittedSignal* txSignal = new TransmittedSignal();
  txSignal->SetValues(Getvalues());

  return txSignal;
}