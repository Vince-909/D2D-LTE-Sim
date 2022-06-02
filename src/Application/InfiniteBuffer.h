#ifndef INFINITEBUFFER_H_
#define INFINITEBUFFER_H_

#include "Application.h"

class InfiniteBuffer : public Application

{
public:

	InfiniteBuffer();
	virtual ~InfiniteBuffer();

	virtual void DoStart(void);
	virtual void DoStop(void);

};

#endif