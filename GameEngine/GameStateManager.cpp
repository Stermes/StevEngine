#include "GameStateManager.h"
#include "InputManager.h"
#include "RenderingManager.h"
#include "GameState.h"

#define FIXEDFRAMES 60

GameStateManager::GameStateManager()
{
}

void GameStateManager::Init()
{
	fixedUpdateTime = 1.0f / FIXEDFRAMES;
}

void GameStateManager::UpdateState(float dt)
{
	frame++;
	time += dt;

	fixedTime += dt;

	if (States.empty())
	{
		return;
	}

	States.top()->Update(dt);
	if (fixedTime >= fixedUpdateTime)
	{
		States.top()->FixedUpdate(fixedUpdateTime);
		fixedTime -= fixedUpdateTime;
	}
}

void GameStateManager::RenderState()
{
	if (States.empty())
	{
		return;
	}
	States.top()->m_RenderingManager->Render();
}


void GameStateManager::Pop()
{
	if (States.empty())
	{
		return;
	}
	States.top()->ShutDown();
	delete States.top();
	States.pop();
}

void GameStateManager::PushState(GameState *state)
{
	States.push(state);
	States.top()->Init();
}

void GameStateManager::ChangeState(GameState *state)
{
	while (!States.empty())
	{
		States.top()->ShutDown();
		delete States.top();
		States.pop();
	}
	States.push(state);
	States.top()->Init();
}