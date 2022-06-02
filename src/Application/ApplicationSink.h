#ifndef APPLICATIONSINK_H_
#define APPLICATIONSINK_H_

class Application;
class Packet;
class QoeManager;

class ApplicationSink
{

public:

	ApplicationSink();
	~ApplicationSink();

	Application* GetApplication(void);
	void SetApplication(Application*);

	QoeManager* GetQoeManager(void);
	void SetQoeManager(QoeManager*);

	void Receive(Packet*);

private:

	Application* m_app;
	QoeManager* m_qoeManager;

};

#endif