#include "InfiniteBuffer.h"

InfiniteBuffer::InfiniteBuffer()
{
  SetApplicationType(Application::APPLICATION_TYPE_INFINITE_BUFFER);
}

InfiniteBuffer::~InfiniteBuffer()
{
  Destroy ();
}

void InfiniteBuffer::DoStart(void)
{
}

void InfiniteBuffer::DoStop(void)
{
}