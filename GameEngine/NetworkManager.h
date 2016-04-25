#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#pragma once
#include "Singleton.h"
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include "GetTime.h"
#include "Rand.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "ReadyEvent.h"
#include <assert.h>
#include "Kbhit.h"
#include "RakSleep.h"
#include "SocketLayer.h"
#include "FullyConnectedMesh2.h"
#include "ConnectionGraph2.h"
#include <Gets.h>
#include "NetworkIDManager.h"
#include "PacketInclude.h"

class NetworkManager : public Singleton<NetworkManager>
{
	friend class Singleton<NetworkManager>;

public:
	~NetworkManager();

	bool InitHost();
	void InitClient();


	void ShutDown();
	
	void Connect();

	bool SignalEvent(int, bool);
	void AddToList(int eventID, RakNet::RakNetGUID guid);
	
	RakNet::Packet* GetNextPacket();

	RakNet::RakPeerInterface *peer;

	int GetNumOfConnections();
	RakNet::RakNetGUID GetGUIDAt(int i);
	RakNet::RakNetGUID GetMyGUID();

	RakNet::NetworkIDManager* GetIDManager();

	bool m_IsHost;

private:
	NetworkManager();

	RakNet::Packet *packet;

	RakNet::ReadyEvent* m_ReadyEventPlugin;
	RakNet::FullyConnectedMesh2* m_fcm2;
	RakNet::ConnectionGraph2* m_cg2;
	RakNet::NetworkIDManager* m_IDManager;

	bool m_IsConnected;
};

#endif