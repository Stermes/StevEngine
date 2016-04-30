#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#pragma once
#include "GameState.h"
#include "NetworkManager.h"

class PlayState : public GameState
{
public:
	PlayState();
	~PlayState();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void FixedUpdate(float dt) override;
	virtual void ShutDown() override;
	virtual void HandlePackets() override;

private:
	class Camera* camera;
	SceneNode *Text;
	SceneNode *counter;
	void CreateRacer();
	void ClientCreate(RakNet::Packet *packet);
	void HostCreate(RakNet::Packet *packet);

	void Accel(RakNet::Packet *packet);
	void StopAccel(RakNet::Packet *packet);

	void Decel(RakNet::Packet *packet);

	float Timer;

	int Pos;
	bool called;
	int Winner;
};

#endif