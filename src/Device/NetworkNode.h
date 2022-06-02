#ifndef NETWORKNODE_H_
#define NETWORKNODE_H_

class Cell;
class Position;
class LtePhy;
class RrcEntity;
class MacEntity;
class PacketBurst;
class D2DManager;
class PowerManager;

class NetworkNode
{

public:

	NetworkNode();
	~NetworkNode();

	enum NodeType
	{
		TYPE_ENODEB,
		TYPE_UE,
	};

	NodeType GetNodeType(void);
	void SetNodeType(NodeType);

	int GetNodeID();
	void SetNodeID(int);

	Cell* GetCell();
	void SetCell(Cell*);

	Position* GetPosition();
	void SetPosition(double, double, double, double, double, double);
	void SetPosition(double, double);

	RrcEntity* GetRrcEntity(void);
	void SetRrcEntity(RrcEntity*);

	LtePhy* GetPhy(void);
	void SetPhy(LtePhy*);

	MacEntity* GetMacEntity(void);
	void SetMacEntity(MacEntity*);

	void SendPacketBurst(PacketBurst*, int = 0);
    void ReceivePacketBurst(PacketBurst*);

	D2DManager* GetD2DManager(void);
	void SetD2DManager(D2DManager*);

private:

	NodeType m_nodeType;
	int m_idNode;
	Cell* m_cell;
	Position* m_pos;

	MacEntity* m_mac;
	LtePhy* m_phy;
	RrcEntity* m_rrc;
	D2DManager* m_d2dManager;

};

#endif
