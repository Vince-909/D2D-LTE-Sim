#ifndef FRAMEMANAGER_H_
#define FRAMEMANAGER_H_

#include <iostream>

class NetworkManager;

class FrameManager {

	int m_nbFrames;
	int m_nbSubframes;
	unsigned long m_TTICounter;

	FrameManager();
	static FrameManager *ptr;

public:

	virtual ~FrameManager();

	static FrameManager*
	Init (void)
	  {
		if (ptr==NULL)
	      {
		    ptr = new FrameManager;
	   	  }
		return ptr;
	  }

	void UpdateNbFrames(void);
	int GetNbFrames(void) const;
	void UpdateNbSubframes(void);
	void ResetNbSubframes(void);
	int GetNbSubframes(void) const;
	void UpdateTTIcounter(void);
	unsigned long GetTTICounter(void) const;

	void Start(void);
	void StartFrame(void);
	void StartSubframe(void);
	void StopFrame(void);
	void StopSubframe(void);

	NetworkManager* GetNetworkManager(void);

	void UpdateUserPosition(void);
	void ResourceAllocation(void);

};

#endif
