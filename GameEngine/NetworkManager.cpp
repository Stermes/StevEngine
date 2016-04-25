#include "NetworkManager.h"

#define MAX_CLIENTS 4
#define SERVER_PORT 60000


NetworkManager::NetworkManager()
{
	m_IsConnected = false;
}


NetworkManager::~NetworkManager()
{
}

bool NetworkManager::InitHost()
{
	peer = RakNet::RakPeerInterface::GetInstance();


	m_ReadyEventPlugin = new RakNet::ReadyEvent();

	m_fcm2 = new RakNet::FullyConnectedMesh2();
	m_cg2 = new RakNet::ConnectionGraph2();
	m_IDManager = new RakNet::NetworkIDManager;

	m_IsHost = true;

	peer->AttachPlugin(m_ReadyEventPlugin);
	peer->AttachPlugin(m_fcm2);
	peer->AttachPlugin(m_cg2);
	peer->SetMaximumIncomingConnections(8);
	m_fcm2->SetAutoparticipateConnections(true);
	m_fcm2->SetConnectOnNewRemoteConnection(true, "");
	m_cg2->SetAutoProcessNewConnections(true);

	RakNet::SocketDescriptor sd(SERVER_PORT, 0);

	if(RakNet::IRNS2_Berkley::IsPortInUse(sd.port, sd.hostAddress, sd.socketFamily, SOCK_DGRAM) == true)
	{
		return false;
	}
	RakNet::StartupResult sr = peer->Startup(MAX_CLIENTS, &sd, 1);
	RakAssert(sr == RakNet::RAKNET_STARTED);
	m_IsConnected = true;
	return true;
}

void NetworkManager::InitClient()
{
	peer = RakNet::RakPeerInterface::GetInstance();
	m_IsHost = false;
	m_ReadyEventPlugin = new RakNet::ReadyEvent();
	m_IDManager = new RakNet::NetworkIDManager;
	peer->AttachPlugin(m_ReadyEventPlugin);
	RakNet::SocketDescriptor sd(SERVER_PORT, 0);

	while (RakNet::IRNS2_Berkley::IsPortInUse(sd.port, sd.hostAddress, sd.socketFamily, SOCK_DGRAM) == true)
	{
		sd.port++;
	}
	RakNet::StartupResult sr = peer->Startup(MAX_CLIENTS, &sd, 1);
	RakAssert(sr == RakNet::RAKNET_STARTED); 
	m_IsConnected = true;
}

void NetworkManager::ShutDown()
{
	// Be nice and let the server know we quit.
	peer->Shutdown(300);

	// We're done with the network
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void NetworkManager::Connect()
{	
	char str[512];
	strcpy_s(str, "127.0.0.1");
	RakNet::ConnectionAttemptResult car = peer->Connect(str, 60000, 0, 0, 0);
	RakAssert(car == RakNet::CONNECTION_ATTEMPT_STARTED);
}


RakNet::Packet* NetworkManager::GetNextPacket()
{
	if (peer == NULL)
	{
		return NULL;
	}
	if (packet)
	{
		peer->DeallocatePacket(packet);
	}
	packet = peer->Receive();

	return packet;
}

void NetworkManager::AddToList(int eventID, RakNet::RakNetGUID guid)
{
	
	m_ReadyEventPlugin->AddToWaitList(eventID, guid);
}

bool NetworkManager::SignalEvent(int eventID, bool signal)
{
	return m_ReadyEventPlugin->SetEvent(eventID, signal);
}

int NetworkManager::GetNumOfConnections()
{
	if (peer == NULL)
	{
		return 0;
	}
	return peer->NumberOfConnections();
}

RakNet::RakNetGUID NetworkManager::GetGUIDAt(int i)
{
	return peer->GetGUIDFromIndex(i);
}

RakNet::RakNetGUID NetworkManager::GetMyGUID()
{
	return peer->GetMyGUID();
}

RakNet::NetworkIDManager* NetworkManager::GetIDManager()
{
	return m_IDManager;
}