#ifndef HOSTSTATE_H
#define HOSTSTATE_H

#pragma once
#include "GameState.h"
class HostState : public GameState
{
public:
	HostState();
	~HostState();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void ShutDown() override;
	virtual void HandlePackets() override;
	int NumOfReady;
private:
	SceneNode *ConnectionText;
	bool Exit;
};

#endif