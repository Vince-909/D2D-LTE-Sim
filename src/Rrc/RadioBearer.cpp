#include "RadioBearer.h"
#include <iostream>
#include "MacQueue.h"
#include "../Core/Simulator.h"
#include "RadioBearerSink.h"

RadioBearer::RadioBearer()
{
	m_app = NULL;
	m_rbSink = NULL;
	m_macq = new MacQueue();
	m_averageTransmissionRate = 100000; //start value = 1kbps
	ResetTransmittedBytes();
}

RadioBearer::~RadioBearer()
{
	delete m_macq;
}

	
MacQueue* RadioBearer::GetMacQueue(void)
{
	return m_macq;
}

void RadioBearer::SetRadioBearerSink(RadioBearerSink* rbs)
{
	m_rbSink = rbs;
}

RadioBearerSink* RadioBearer::GetRadioBearerSink(void)
{
	return m_rbSink;
}

void RadioBearer::SetApplication(Application* app)
{
	m_app = app;
}

Application* RadioBearer::GetApplication(void)
{
	return m_app;
}

void RadioBearer::Enqueue(Packet* p)
{
	m_macq->AddPacket(p);
}

int RadioBearer::GetTransmittedBytes(void)
{
	return m_transmittedBytes;
}

double RadioBearer::GetLastUpdate(void)
{
	return m_lastUpdate;
}

void RadioBearer::ResetTransmittedBytes(void)
{
	m_transmittedBytes = 0;
	SetLastUpdate();
}

void RadioBearer::SetLastUpdate(void)
{
	m_lastUpdate = Simulator::Init()->Now();
}

void RadioBearer::UpdateAverageTransmissionRate(void)
{
  /*
   * Update Transmission Data Rate with
   * a Moving Average
   * R'(t+1) = (0.8 * R'(t)) + (0.2 * r(t))
   */
  double rate;
  if (GetTransmittedBytes() == 0) rate = 0;
  else rate = (GetTransmittedBytes() * 8)/(Simulator::Init()->Now() - GetLastUpdate());

  double beta = 0.2;

  m_averageTransmissionRate =
      ((1 - beta) * m_averageTransmissionRate) + (beta * rate);

  if (m_averageTransmissionRate < 1)
  {
	  m_averageTransmissionRate = 1;
  }
 
  ResetTransmittedBytes();
}

double RadioBearer::GetAverageTransmissionRate(void)
{
	return m_averageTransmissionRate;
}

void RadioBearer::UpdateTransmittedBytes(int bytes)
{
	m_transmittedBytes += bytes;
}