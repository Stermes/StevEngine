#ifndef JOINSTATE_H
#define JOINSTATE_H

#pragma once
#include "GameState.h"
class JoinState :
	public GameState
{
public:
	JoinState();
	~JoinState();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void ShutDown() override;
	virtual void HandlePackets() override;

private:
	SceneNode *ConnectionText;
};

#endif