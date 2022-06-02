#ifndef TRANSMITTEDSIGNAL_H_
#define TRANSMITTEDSIGNAL_H_

#include <vector>

class TransmittedSignal
{

public:

	TransmittedSignal();
	virtual ~TransmittedSignal();

	void SetValues(std::vector<double> values);
	std::vector<double> Getvalues(void);

	TransmittedSignal* Copy(void);

private:

	std::vector<double> m_values;

};

#endif