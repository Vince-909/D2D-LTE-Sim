#ifndef IDEALCONTROLMESSAGE_H_
#define IDEALCONTROLMESSAGE_H_

class NetworkNode;

class IdealControlMessage
{

public:
	
	enum MessageType
	{
		CQI_FEEDBACKS,
		ALLOCATION_MAP,
		SCHEDULING_REQUEST,
		POWER_CONTROL,
		QOE_REPORT,
		D2D
	};

	IdealControlMessage();
	~IdealControlMessage();

	NetworkNode* GetSource(void);
	void SetSource(NetworkNode*);

	NetworkNode* GetDestination(void);
	void SetDestination(NetworkNode*);

	MessageType GetMessageType(void);
	void SetMessageType(MessageType);

private:
	
	NetworkNode* m_source;
	NetworkNode* m_destination;
	MessageType m_type;

};

#endif