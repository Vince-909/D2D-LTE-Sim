#include "FlowsManager.h"
#include "../Application/VoIP.h"
#include "../Application/ApplicationSink.h"
#include "../Rrc/RadioBearer.h"
#include "../Rrc/RadioBearerSink.h"
#include "../Device/UserEquipment.h"
#include "../Device/ENodeB.h"
#include "../Rrc/RrcEntity.h"
#include <vector>
#include "../QualityOfExperience/QoeManager.h"
#include "../Application/VideoStream.h"

FlowsManager* FlowsManager::ptr = NULL;

FlowsManager::FlowsManager()
{
	m_applicationsContainer = new std::vector<Application*>;
}

FlowsManager::~FlowsManager()
{
}

std::vector<Application*>* FlowsManager::GetApplicationsContainer(void)
{
	return m_applicationsContainer;
}

Application* FlowsManager::CreateApplication(int idApp, UserEquipment* src, UserEquipment* dst,
											Application::ApplicationType type, double startTime, double duration)
{

  Application* app;

  if (type == Application::APPLICATION_TYPE_VOIP)
    {
	  app = (Application*) new VoIP();
    }

  if (type == Application::APPLICATION_TYPE_VIDEOSTREAM)
    {
	  app = (Application*) new VideoStream();
    }

  //create application sink
  ApplicationSink* apps = new ApplicationSink();

  //create a quality of experience manager
  QoeManager* qoe = new QoeManager();
  apps->SetQoeManager(qoe);
  qoe->SetApplicationSink(apps);

  //create radio bearer and radio bearer sink between transmitting ue and enb
  RadioBearer* urb = new RadioBearer();
  src->GetRrcEntity()->AddRadioBearer(urb);
  
  RadioBearerSink* urbs = new RadioBearerSink();
  src->GetTargetEnb()->GetRrcEntity()->AddRadioBearerSink(urbs);

  //create radio bearer and radio bearer sink between enb and receiving ue
  RadioBearer* drb = new RadioBearer();
  src->GetTargetEnb()->GetRrcEntity()->AddRadioBearer(drb);

  RadioBearerSink* drbs = new RadioBearerSink();
  dst->GetRrcEntity()->AddRadioBearerSink(drbs);

  app->SetApplicationID(idApp);
  app->SetApplicationSource(src);
  app->SetApplicationDestination(dst);
  app->SetStartTime(startTime);
  app->SetStopTime(startTime + duration);

  //link application and radio bearers
  app->SetApplicationSink(apps);
  apps->SetApplication(app);
  app->SetRadioBearer(urb);
  urb->SetApplication(app);
  urb->SetRadioBearerSink(urbs);
  urbs->SetRadioBearer(urb);
  drb->SetApplication(app);
  drb->SetRadioBearerSink(drbs);
  drbs->SetRadioBearer(drb);
  drbs->SetApplicationSink(apps);

  m_applicationsContainer->push_back(app);

  return app;

}

