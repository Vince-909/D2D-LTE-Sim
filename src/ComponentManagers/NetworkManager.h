#ifndef NETWORKMANAGER_H_
#define NETWORKMANAGER_H_

#include <vector>

class Cell;
class ENodeB;
class UserEquipment;
class NetworkNode;
class BandwidthManager;
class LteChannel;

class NetworkManager
{

	std::vector<Cell*> *m_cellContainer;
	std::vector<ENodeB*> *m_eNodeBContainer;
	std::vector<UserEquipment*> *m_userEquipmentContainer;

	NetworkManager();
	static NetworkManager *ptr;

public:

	~NetworkManager();

	static NetworkManager*
	Init (void)
	  {
		if (ptr==NULL)
	      {
		    ptr = new NetworkManager;
	   	  }
		return ptr;
	  }

	std::vector<Cell*>* GetCellContainer(void);
	std::vector<ENodeB*>* GetENodeBContainer(void);
	std::vector<UserEquipment*>* GetUserEquipmentContainer(void);

    Cell* CreateCell(int, double, double);

    ENodeB* CreateEnodeb(int, Cell*, BandwidthManager*, LteChannel*, LteChannel*, bool);

    UserEquipment* CreateUserEquipment(int, Cell*, double, double, double, double, ENodeB*);
	UserEquipment* CreateUserEquipment(int, Cell*, double, ENodeB*);

	Cell* GetCellByID(int);
	ENodeB* GetENodeBByID(int);
	ENodeB* GetENodeBByCellID(int);
	UserEquipment* GetUserEquipmentByID(int);
	std::vector<UserEquipment*>* GetRegisteredUEToENodeB(int);
	NetworkNode* GetNetworkNodeByID(int);

};


#endif