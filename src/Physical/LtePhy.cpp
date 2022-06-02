#include "LtePhy.h"
#include <iostream>
#include "../Spectrum/TransmittedSignal.h"
#include "ErrorModel.h"
#include "../PowerControl/PowerManager.h"

LtePhy::LtePhy()
{
  m_device = NULL;
  m_dlChannel = NULL;
  m_ulChannel = NULL;
  m_bandwidthManager = NULL;
  m_txSignal = NULL;
  m_powerManager = NULL;
}

LtePhy::~LtePhy()
{
  m_device = NULL;
  m_dlChannel = NULL;
  m_ulChannel = NULL;
  m_bandwidthManager = NULL;
  delete m_txSignal;
 }

void LtePhy::SetDevice(NetworkNode* device)
{
	m_device = device;
}

NetworkNode* LtePhy::GetDevice(void)
{
	return m_device;
}

void LtePhy::Destroy(void)
{
  m_device = NULL;
  m_dlChannel = NULL;
  m_ulChannel = NULL;
  m_bandwidthManager = NULL;
  delete m_txSignal;
  delete m_errorModel;
}

void LtePhy::SetBandwidthManager(BandwidthManager* s)
{
	m_bandwidthManager = s;
	DoSetBandwidthManager();
}

BandwidthManager* LtePhy::GetBandwidthManager(void)
{
  return m_bandwidthManager;
}

void LtePhy::SetTxSignal(TransmittedSignal* txSignal)
{
  m_txSignal = txSignal;
}

void LtePhy::SetErrorModel(ErrorModel* e)
{
  m_errorModel = e;
}

ErrorModel* LtePhy::GetErrorModel(void)
{
  return m_errorModel;
}

LteChannel* LtePhy::GetDlChannel(void)
{
	return m_dlChannel;
}

LteChannel* LtePhy::GetUlChannel(void)
{
	return m_ulChannel;
}

void LtePhy::SetDlChannel(LteChannel* ch)
{
	m_dlChannel = ch;
}

void LtePhy::SetUlChannel(LteChannel* ch)
{
	m_ulChannel = ch;
}

TransmittedSignal* LtePhy::GetTxSignal(void)
{
	return m_txSignal;
}

PowerManager* LtePhy::GetPowerManager(void)
{
	return m_powerManager;
}

void LtePhy::SetPowerManager(PowerManager* manager)
{
	m_powerManager = manager;
}