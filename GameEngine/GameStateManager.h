#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include <stack>
#include <map>
#include "Singleton.h"

#pragma once
class GameStateManager : public Singleton<GameStateManager>
{
	friend class Singleton<GameStateManager>;

public:

	// The amount of frames that have passed
	int frame = 0;

	// The amount of time that has passed since start up
	float time = 0;
	
	// Screen width and height
	int Width, Height;

	void Init();

	// Updates the top state in the stack
	void UpdateState(float dt);
	// Renders the top state in the stack
	void RenderState();

	// Pops and tells the top most state in the stack to shutdown
	void Pop();
	// Push a new state on top of the stack
	void PushState(class GameState *state);

	// Clears all states and sets the new state on top
	void ChangeState(class GameState *state);

	class GameState* CurrentState()
	{
		return States.top();
	}
protected:
	// Stack of the current game states
	std::stack<class GameState*> States;

	GameStateManager();
	// Constructor/Destructor

	float fixedUpdateTime = 0;
	float fixedTime = 0;

};

#endif