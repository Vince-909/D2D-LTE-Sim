#ifndef APPLICATION_H_
#define APPLICATION_H_

class RadioBearer;
class RadioBearerSink;
class UserEquipment;
class ApplicationSink;

class Application
{

public:

	enum ApplicationType
	{
		APPLICATION_TYPE_VOIP,
	    APPLICATION_TYPE_VIDEOSTREAM
	 };

	Application();
	~Application();

	void Destroy (void);

	double GetStartTime(void);
	void SetStartTime(double);

	double GetStopTime(void);
	void SetStopTime(double);

	void Start(void);
	void Stop(void);
	virtual void DoStart (void) = 0;
	virtual void DoStop (void) = 0;

	RadioBearer* GetRadioBearer(void);
	void SetRadioBearer(RadioBearer*);

	UserEquipment* GetApplicationSource(void);
	void SetApplicationSource(UserEquipment*);

	UserEquipment* GetApplicationDestination(void);
	void SetApplicationDestination(UserEquipment*);
	
	ApplicationType GetApplicationType(void);
	void SetApplicationType(ApplicationType);
	
	int GetApplicationID(void);
	void SetApplicationID(int);
	
	ApplicationSink* GetApplicationSink(void);
	void SetApplicationSink(ApplicationSink*);

private:
	
	int idApp;

	UserEquipment* m_src;
	UserEquipment* m_dest;

	ApplicationType m_appType;

	double m_startTime;
	double m_stopTime;

	RadioBearer* m_bearer;

	ApplicationSink* m_appSink;

};

#endif