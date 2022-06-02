#ifndef PACKET_H_
#define PACKET_H_

class Packet
{

public:

	Packet();
	~Packet();

	int GetId(void);
	void SetId(int);

	int GetIdMac(void);
	void SetIdMac(int);

	double GetTimeStamp(void);
	void SetTimeStamp(double);

	int GetApplicationId(void);
	void SetApplicationId(int);

	int GetSize(void);
	void SetSize(int);

	int GetSource(void);
	void SetSource(int);

	int GetDestination(void);
	void SetDestination(int);

	int GetFragmentSize(void);
	void SetFragmentSize(int);

	Packet* Copy(void);

private:

	int m_id;
	int m_idSource;
	int m_idDestination;
	int m_idMacDestination;
	int m_idApplication;
	double m_timeStamp;
	int m_size;
	int m_fragmentSize;

};

#endif