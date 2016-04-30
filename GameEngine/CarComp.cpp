#include "CarComp.h"
#include "InputManager.h"
// GLFW Include
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "NetworkManager.h"
#include "AudioManager.h"
#include <GLFW\glfw3.h>
#include <typeinfo>

CarComp::CarComp(int Num) : Component()
{
	CarNum = Num;
	m_Speed = 0.0f;
	m_MaxSpeed = 300.0f;
	AllreadyWinner = false;
	EngineSound = AudioManager::Instance().PlaySound2D("Cars/car.wav", true, true, true);
	//EngineSound->setIsPaused(true);
}

float time = 0.1f;

CarComp::~CarComp()
{
}

void CarComp::Update(float DeltaTime)
{
	//if (gameObject->transform.position.x > 350 && !AllreadyWinner)
	//{
	//	RakNet::BitStream oStream;

	//	oStream.Write((RakNet::MessageID)ID_WINNER);
	//	oStream.Write(CarNum);

	//	NetworkManager::Instance().peer->Send(&oStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	//	AllreadyWinner = true;
	//}

	gameObject->transform.position.x += m_Speed * DeltaTime;

	if (gameObject->GetIsMaster())
	{
		if (InputManager::Instance().KeyDown(GLFW_KEY_D))
		{
			Accelerate(DeltaTime);
		}
		else
		{
			StopAccelerate();
		}
		if (InputManager::Instance().KeyDown(GLFW_KEY_A))
		{
			Decelerate(DeltaTime);
		}
	}
}


void CarComp::Accelerate(float DeltaTime)
{
	time -= DeltaTime;

	if (gameObject->GetIsMaster())
	{
		m_Speed += 20.0f * DeltaTime;
		if (m_Speed > m_MaxSpeed)
		{
			m_Speed = m_MaxSpeed;
		}
		if (time <= 0)
		{
			RakNet::BitStream oStream;

			oStream.Write((RakNet::MessageID)ID_ACCELERATE);

			oStream.Write(gameObject->GetNetworkID());
			oStream.Write(m_Speed);

			NetworkManager::Instance().peer->Send(&oStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
			time = 0.1f;
		}
	}
	SpriteRenderer* renderer = (SpriteRenderer*)gameObject->GetComponent(typeid(SpriteRenderer));
	if (renderer)
	{
		std::string loc = "Move" + std::to_string(CarNum);
		renderer->texture = ResourceManager::GetTexture(loc.c_str());
	}
	
	if (EngineSound)
	{
		EngineSound->setIsPaused(false);
	}
}

void CarComp::StopAccelerate()
{
	if (InputManager::Instance().KeyUp(GLFW_KEY_D))
	{
		RakNet::BitStream oStream;

		oStream.Write((RakNet::MessageID)ID_STOP_ACCELERATE);
		oStream.Write(gameObject->GetNetworkID());

		NetworkManager::Instance().peer->Send(&oStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}

	SpriteRenderer* renderer = (SpriteRenderer*)gameObject->GetComponent(typeid(SpriteRenderer));
	if (renderer)
	{
		std::string loc = "Still" + std::to_string(CarNum);
		renderer->texture = ResourceManager::GetTexture(loc.c_str());
	}
	if (EngineSound)
	{
		EngineSound->setIsPaused(true);
	}
	
}

void CarComp::Decelerate(float DeltaTime)
{
	time -= DeltaTime;
	if (gameObject->GetIsMaster())
	{
		m_Speed -= 20.0f * DeltaTime;
		if (m_Speed < -m_MaxSpeed)
		{
			m_Speed = -m_MaxSpeed;
		}
		if (time <= 0)
		{
			RakNet::BitStream oStream;

			oStream.Write((RakNet::MessageID)ID_DECELERATE);

			oStream.Write(gameObject->GetNetworkID());
			oStream.Write(m_Speed);

			NetworkManager::Instance().peer->Send(&oStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
			time = 0.1f;
		}
	}
}
