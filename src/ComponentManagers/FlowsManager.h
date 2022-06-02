#ifndef FLOWSMANAGER_H_
#define FLOWSMANAGER_H_

#include <iostream>
#include "../Application/Application.h"
#include <vector>

class UserEquipment;
class ApplicationSink;
class RadioBearer;
class RadioBearerSink;

class FlowsManager {

	FlowsManager();
	static FlowsManager *ptr;
	std::vector<Application*>* m_applicationsContainer;

public:

	virtual ~FlowsManager();

	static FlowsManager*
	Init(void)
	  {
		if (ptr==NULL)
	      {
		    ptr = new FlowsManager;
	   	  }
		return ptr;
	  }

	Application* CreateApplication(int, UserEquipment*, UserEquipment*, Application::ApplicationType, double, double);
	ApplicationSink* CreateApplicationSink(void);
	RadioBearer* CreateRadioBearer(void);
	RadioBearerSink* CreateRadioBearerSink(void);
	std::vector<Application*>* GetApplicationsContainer(void);

};

#endif