#ifndef RADIOBEARERSINK_H_
#define RADIOBEARERSINK_H_

#include <vector>

class RadioBearer;
class Packet;
class ApplicationSink;

class RadioBearerSink
{

public:

	RadioBearerSink();
	~RadioBearerSink();

	RadioBearer* GetRadioBearer(void);
	void SetRadioBearer(RadioBearer*);

	ApplicationSink* GetApplicationSink(void);
	void SetApplicationSink(ApplicationSink*);

	std::vector<std::vector<Packet*>*>* GetFragments(void);
	bool Defragment(Packet**);
	void Receive(Packet*);

private:
	
	RadioBearer* m_bearer;
	ApplicationSink* m_appSink;
	std::vector<std::vector<Packet*>*>* m_fragments;

};

#endif