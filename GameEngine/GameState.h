#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "ResourceManager.h"
#include "TextRenderer.h"
#include "SceneNode.h"
#include <vector>

// GLFW Include
#include <GLFW\glfw3.h>

#pragma once
class GameState
{
public:
	GameState();
	~GameState();

	// Initialization function of the current gameste
	virtual void Init() = 0;
	// Updates all objects within the game
	virtual void Update(float dt);
	virtual void FixedUpdate(float dt);
	// Shutsdown the current game scene
	virtual void ShutDown() = 0;

	virtual void HandlePackets() {};

	class RenderingManager *renderManager;
	class PhysicsManager *physicsManager;

protected:
	// The game state manager
	class GameStateManager *m_Manager;
	class SceneNode* m_Root;

	// Pops this state from the state manager
	void Pop();
	// Pushes a new state ontop of the state manager
	void PushState(GameState *state);
	// Stop this state and push a new one on top
	void ChangeState(GameState *state);

};

#endif
