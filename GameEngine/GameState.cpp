#include "GameState.h"
#include "GameStateManager.h"
#include "RenderingManager.h"
#include "SceneNode.h"


GameState::GameState() 
{
	m_Manager = &GameStateManager::Instance();
	m_RenderingManager = new RenderingManager();
	m_Root = new SceneNode();
}

GameState::~GameState()
{
	delete m_Root;
	delete m_RenderingManager;
}

void GameState::Pop()
{
	m_Manager->Pop();
}

void GameState::PushState(GameState *state)
{
	m_Manager->PushState(state);
}

void GameState::ChangeState(GameState *state)
{
	m_Manager->ChangeState(state);
}




