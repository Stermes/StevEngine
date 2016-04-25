#include "JoinState.h"
#include "NetworkManager.h"
#include "Button.h"
#include "TextRenderer.h"
#include "SceneLoad.h"

#include <sstream>
#include <typeinfo>



JoinState::JoinState() : GameState()
{
	NetworkManager::Instance().InitClient();
	NetworkManager::Instance().Connect();
}


JoinState::~JoinState()
{
}

void JoinState::Init()
{
	TextRenderer *ConnectionNum = new TextRenderer(m_Manager->Width, m_Manager->Height);
	ConnectionNum->Load("neuropolitical rg.ttf", 12);

	ConnectionText = new SceneNode();
	ConnectionText->AddComponent(ConnectionNum);
	ConnectionText->transform.position.x = 60;
	m_Root->AddChild(ConnectionText);

	SceneNode* button = new SceneNode();
	Button *menu = new Button(100, 50);
	menu->Text->Text = "Main Menu";
	menu->PressFunc = SceneLoad::Pop;
	button->AddComponent(menu);
	button->transform.position.z = 5;
	button->transform.position.x = m_Manager->Width / 2;
	button->transform.position.y = m_Manager->Height / 2 + 50;

	m_Root->AddChild(button);

	button = new SceneNode();
	Button *play = new Button(100, 50);
	play->Text->Text = "Ready";
	play->PressFunc = SceneLoad::Ready;
	button->AddComponent(play);
	button->transform.position.z = 5;
	button->transform.position.x = m_Manager->Width / 2;
	button->transform.position.y = m_Manager->Height / 2 - 50;
	m_Root->AddChild(button);
}

void JoinState::Update(float dt)
{
	
	// Prints the number of connections on the screen
	TextRenderer* tRender = (TextRenderer*)ConnectionText->GetComponent(typeid(TextRenderer));
	if (tRender)
	{
		std::stringstream ss;
		ss = std::stringstream();
		ss << NetworkManager::Instance().GetNumOfConnections();
		if (NetworkManager::Instance().GetNumOfConnections() == 1)
			tRender->Text = ss.str() + " is connected";
		else
			tRender->Text = ss.str() + " are connected";

		ss.clear();
	}

	HandlePackets();
	m_Root->Update(dt);
}

void JoinState::ShutDown()
{
	NetworkManager::Instance().ShutDown();
}

void JoinState::HandlePackets()
{
	for (RakNet::Packet *currentPacket = NetworkManager::Instance().GetNextPacket(); currentPacket; currentPacket = NetworkManager::Instance().GetNextPacket())
	{
		switch (currentPacket->data[0])
		{
		case ID_NEW_INCOMING_CONNECTION:
			printf("ID_NEW_INCOMING_CONNECTION\n");
			for (int i = 0; i < COUNT; i++)
			{
				NetworkManager::Instance().AddToList(i, currentPacket->guid);
			}
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			printf("ID_CONNECTION_REQUEST_ACCEPTED\n");

			for (int i = 0; i < COUNT; i++)
			{
				NetworkManager::Instance().AddToList(i, currentPacket->guid);
			}
			break;
		case ID_READY_EVENT_ALL_SET:
		{
			RakNet::BitStream bs(currentPacket->data, currentPacket->length, false);
			bs.IgnoreBytes(sizeof(RakNet::MessageID));
			int eventID;
			bs.Read(eventID);
			if (eventID == STARTGAME)
			{
				SceneLoad::LoadPlayState();
				printf("Game is Starting!.\n");
			}
		}
		break;
		case ID_DISCONNECTION_NOTIFICATION:
			// Connection lost normally
			printf("ID_DISCONNECTION_NOTIFICATION\n");
			break;
		case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
			printf("ID_REMOTE_DISCONNECTION_NOTIFICATION\n");
			break;
		case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
			printf("ID_REMOTE_CONNECTION_LOST\n");
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
			printf("ID_REMOTE_NEW_INCOMING_CONNECTION\n");
			break;
		case ID_CONNECTION_LOST:
			// Couldn't deliver a reliable packet - i.e. the other system was abnormally
			// terminated
			printf("ID_CONNECTION_LOST\n");
			break;

			break;
		case ID_CLIENT_CREATE_RACER:
			printf("Client\n");
			break;
		case ID_HOST_CREATE_RACER:
			printf("Host\n");
			break;
		}
	}
}