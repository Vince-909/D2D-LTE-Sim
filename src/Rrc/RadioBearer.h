#ifndef RADIOBEARER_H_
#define RADIOBEARER_H_

class MacQueue;
class RadioBearerSink;
class Application;
class Packet;

class RadioBearer
{

public:

	RadioBearer();
	~RadioBearer();

	MacQueue* GetMacQueue(void);

	RadioBearerSink* GetRadioBearerSink(void);
	void SetRadioBearerSink(RadioBearerSink*);

	Application* GetApplication(void);
	void SetApplication(Application*);
	
	double GetAverageTransmissionRate(void);
	void UpdateAverageTransmissionRate(void);

	int GetTransmittedBytes(void);
	void UpdateTransmittedBytes(int);
	void ResetTransmittedBytes(void);

	double GetLastUpdate(void);
	void SetLastUpdate(void);
	
	void Enqueue(Packet*);

private:

	Application* m_app;
	MacQueue* m_macq;
	RadioBearerSink* m_rbSink;

	double m_averageTransmissionRate;
	int m_transmittedBytes;
	double m_lastUpdate;

};

#endif