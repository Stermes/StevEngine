#include "PlayState.h"
#include "SpriteRenderer.h"
#include "Camera.h"
#include "CarComp.h"
#include "PhysicsBody.h"
#include "NetworkIDManager.h"
#include <iostream>
#include "MenuState.h"


PlayState::PlayState() : GameState()
{
}

PlayState::~PlayState()
{
}

void PlayState::Init()
{
	Pos = 0;
	called = false;
	ResourceManager::LoadShader("LightingVS.glsl", "SpriteFS.glsl", nullptr, "SpriteShader");
	ResourceManager::LoadTexture("Cars/DragRacerPlayer1Moving.png", GL_FALSE, "Move1");
	ResourceManager::LoadTexture("Cars/DragRacerPlayer2Moving.png", GL_FALSE, "Move2");
	ResourceManager::LoadTexture("Cars/DragRacerPlayer3Moving.png", GL_FALSE, "Move3");
	ResourceManager::LoadTexture("Cars/DragRacerPlayer4Moving.png", GL_FALSE, "Move4");
	ResourceManager::LoadTexture("Cars/DragRacerPlayer1.png", GL_FALSE, "Still1");
	ResourceManager::LoadTexture("Cars/DragRacerPlayer2.png", GL_FALSE, "Still2");
	ResourceManager::LoadTexture("Cars/DragRacerPlayer3.png", GL_FALSE, "Still3");
	ResourceManager::LoadTexture("Cars/DragRacerPlayer4.png", GL_FALSE, "Still4");

	ResourceManager::LoadTexture("Cars/Track.png", GL_FALSE, "Track");

	SceneNode *track = new SceneNode();
	track->AddComponent(new SpriteRenderer(ResourceManager::GetShader("SpriteShader"), ResourceManager::GetTexture("Track")));
	track->transform.position.z = -1;
	track->transform.scale = glm::vec3(1.7f, 1.0f, 1);
	m_Root->AddChild(track);

	TextRenderer *tRender = new TextRenderer(m_Manager->Width, m_Manager->Height);
	tRender->Load("neuropolitical rg.ttf", 24);

	Text = new SceneNode();
	Text->AddComponent(tRender);
	Text->transform.position.z = 1;
	Text->transform.position.x = m_Manager->Width / 2;
	
	m_Root->AddChild(Text);

	camera = new Camera();
	camera->Position.z = 5;
	Winner = 0;

}

void PlayState::Update(float dt)
{
	if (!called)
	{
		if (NetworkManager::Instance().m_IsHost)
		{
			HostCreate(NULL);
			called = true;
		}
	}
	// view matrix
	glm::mat4 view;
	view = camera->GetViewMatrix();

	// projection matrix
	glm::mat4 projection = glm::ortho(
		(float)(-m_Manager->Width / 2),
		(float)(m_Manager->Width / 2),
		(float)(m_Manager->Height / 2),
		(float)(-m_Manager->Height / 2), 0.1f, 100.0f);

	// Set the value in our shader
	ResourceManager::GetShader("SpriteShader").SetMatrix4("view", view, GL_TRUE);
	ResourceManager::GetShader("SpriteShader").SetMatrix4("projection", projection);

	HandlePackets();

	m_Root->Update(dt);

	if (Winner > 0)
	{
		Timer += dt;
		if (Timer >= 2)
		{
			ChangeState(new MenuState());
		}
	}
}

void PlayState::FixedUpdate(float dt)
{
	m_Root->FixedUpdate(dt);
}
void PlayState::ShutDown()
{

}

void PlayState::HandlePackets()
{
	for (RakNet::Packet *currentPacket = NetworkManager::Instance().GetNextPacket(); currentPacket; currentPacket = NetworkManager::Instance().GetNextPacket())
	{
		switch (currentPacket->data[0])
		{
		case ID_DISCONNECTION_NOTIFICATION:
			// Connection lost normally
			printf("ID_DISCONNECTION_NOTIFICATION\n");
			break;
		case ID_CONNECTION_LOST:
			// Couldn't deliver a reliable packet - i.e. the other system was abnormally
			// terminated
			printf("ID_CONNECTION_LOST\n");
			break;
		case ID_CLIENT_CREATE_RACER:
			ClientCreate(currentPacket);
			break;
		case ID_HOST_CREATE_RACER:
			HostCreate(currentPacket);
			break;
		case ID_STOP_ACCELERATE:
			StopAccel(currentPacket);
			break;
		case ID_ACCELERATE:
			Accel(currentPacket);
			break;
		case ID_DECELERATE:
			Decel(currentPacket);
			break;
		case ID_WINNER:

			RakNet::BitStream iStream(currentPacket->data, currentPacket->length, false);

			iStream.IgnoreBytes(sizeof(RakNet::MessageID));
			if (Winner == 0)
			{
				iStream.Read(Winner);

				TextRenderer* tRender = (TextRenderer*)Text->GetComponent(typeid(TextRenderer));
				if (tRender)
				{
					std::string player = "Winner " + std::to_string(Winner);
					tRender->Text = player.c_str();
				}
			}
			break;
		
		}
	}
}

// Creates a new racer
void PlayState::CreateRacer()
{
	RakNet::BitStream oStream;
	oStream.Write((RakNet::MessageID)ID_HOST_CREATE_RACER);
	RakNet::NetworkID netID;
	oStream.Write(netID);
	NetworkManager::Instance().peer->Send(&oStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void PlayState::HostCreate(RakNet::Packet *packet)
{
	if (!NetworkManager::Instance().m_IsHost)
	{
		return;
	}

	SceneNode* newNode = new SceneNode();
	std::string loc = "Still" + std::to_string(Pos + 1);
	newNode->AddComponent(new SpriteRenderer(ResourceManager::GetShader("SpriteShader"), ResourceManager::GetTexture(loc.c_str())));
	newNode->transform.position.y = 200 - 100 * Pos;
	newNode->transform.position.x = -300;
	newNode->transform.scale.x = 0.5f;
	newNode->transform.scale.y = 0.5f;
	newNode->AddComponent(new CarComp(Pos + 1));
	newNode->AddComponent(new PhysicsBody());
	newNode->SetNetworkIDManager(NetworkManager::Instance().GetIDManager());
	m_Root->AddChild(newNode);

	RakNet::BitStream oStream;
	oStream.Write((RakNet::MessageID)ID_CLIENT_CREATE_RACER);
	oStream.Write(newNode->GetNetworkID());
	oStream.Write(Pos);
	if (packet)
	{
		oStream.Write(packet->guid);
		newNode->SetIsMaster(false);
	}
	else
	{
		TextRenderer* tRender = (TextRenderer*)Text->GetComponent(typeid(TextRenderer));
		if (tRender)
		{
			std::string player = "You are player " + std::to_string(Pos + 1);
			tRender->Text = player.c_str();
		}
		oStream.Write(NetworkManager::Instance().GetMyGUID());
		newNode->SetIsMaster(true);

	}
	NetworkManager::Instance().peer->Send(&oStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

	Pos++;
}


void PlayState::ClientCreate(RakNet::Packet *packet)
{
	if (NetworkManager::Instance().m_IsHost)
	{
		return;
	}

	RakNet::BitStream iStream(packet->data, packet->length, false);

	iStream.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::NetworkID netID;
	RakNet::RakNetGUID guiID;
	iStream.Read(netID);
	iStream.Read(Pos);
	iStream.Read(guiID);


	SceneNode* newNode = new SceneNode();

	std::string loc = "Still" + std::to_string(Pos + 1);
	newNode->AddComponent(new SpriteRenderer(ResourceManager::GetShader("SpriteShader"), ResourceManager::GetTexture(loc.c_str())));
	newNode->transform.position.y = 200 - 100 * Pos;
	newNode->transform.position.x = -300;
	newNode->transform.scale.x = 0.5f;
	newNode->transform.scale.y = 0.5f;
	newNode->AddComponent(new CarComp(Pos + 1));
	newNode->SetNetworkIDManager(NetworkManager::Instance().GetIDManager());
	newNode->SetNetworkID(netID);
	m_Root->AddChild(newNode);


	if (guiID == NetworkManager::Instance().GetMyGUID())
	{
		TextRenderer* tRender = (TextRenderer*)Text->GetComponent(typeid(TextRenderer));
		if (tRender)
		{
			std::string player = "You are player " + std::to_string(Pos + 1);
			tRender->Text = player.c_str();
		}
		newNode->SetIsMaster(true);
	}
	else
	{
		newNode->SetIsMaster(false);
	}
	if (!called)
	{
		CreateRacer();
		called = true;
	}
	//NetworkManager::Instance().peer->Send(&oStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void PlayState::StopAccel(RakNet::Packet *packet)
{
	if (packet->guid == NetworkManager::Instance().GetMyGUID())
	{
		return;
	}

	RakNet::BitStream iStream(packet->data, packet->length, false);
	iStream.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::NetworkID netID;
	iStream.Read(netID);
	
	SceneNode* node = NetworkManager::Instance().GetIDManager()->GET_OBJECT_FROM_ID<SceneNode*>(netID);

	if (node)
	{
		if (!node->GetIsMaster())
		{
			CarComp* car = (CarComp*)node->GetComponent(typeid(CarComp));
			if (car)
			{
				car->StopAccelerate();
			}

			if (NetworkManager::Instance().m_IsHost)
			{
				RakNet::BitStream oStream;

				oStream.Write((RakNet::MessageID)ID_STOP_ACCELERATE);

				oStream.Write(netID);


				NetworkManager::Instance().peer->Send(&oStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
			}
		}
	}
}

void PlayState::Accel(RakNet::Packet *packet)
{
	if (packet->guid == NetworkManager::Instance().GetMyGUID())
	{
		return;
	}

	RakNet::BitStream iStream(packet->data, packet->length, false);
	iStream.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::NetworkID netID;
	iStream.Read(netID);
	float speed;
	iStream.Read(speed);

	std::cout << netID << std::endl;

	SceneNode* node = NetworkManager::Instance().GetIDManager()->GET_OBJECT_FROM_ID<SceneNode*>(netID);

	if (node)
	{
		if (!node->GetIsMaster())
		{
			CarComp* car = (CarComp*)node->GetComponent(typeid(CarComp));
			if (car)
			{
				car->Accelerate(0);
				car->SetSpeed(speed);
			}
			if (NetworkManager::Instance().m_IsHost)
			{
				RakNet::BitStream oStream;

				oStream.Write((RakNet::MessageID)ID_ACCELERATE);

				oStream.Write(netID);
				oStream.Write(speed);

				NetworkManager::Instance().peer->Send(&oStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
			}
		}
	}
}

void PlayState::Decel(RakNet::Packet *packet)
{

	if (packet->guid == NetworkManager::Instance().GetMyGUID())
	{
		return;
	}

	RakNet::BitStream iStream(packet->data, packet->length, false);
	iStream.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::NetworkID netID;
	iStream.Read(netID);
	float speed;
	iStream.Read(speed);

	std::cout << netID << std::endl;

	SceneNode* node = NetworkManager::Instance().GetIDManager()->GET_OBJECT_FROM_ID<SceneNode*>(netID);

	if (node)
	{
		if (!node->GetIsMaster())
		{
			CarComp* car = (CarComp*)node->GetComponent(typeid(CarComp));
			if (car)
			{
				car->Decelerate(0);
				car->SetSpeed(speed);
			}
			if (NetworkManager::Instance().m_IsHost)
			{
				RakNet::BitStream oStream;

				oStream.Write((RakNet::MessageID)ID_DECELERATE);

				oStream.Write(netID);
				oStream.Write(speed);

				NetworkManager::Instance().peer->Send(&oStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
			}
		}
	}
}