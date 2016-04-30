#include "GameState.h"
#include "GameStateManager.h"
#include "RenderingManager.h"
#include "PhysicsManager.h"
#include "SceneNode.h"


GameState::GameState() 
{
	m_Manager = &GameStateManager::Instance();
	renderManager = new RenderingManager();
	physicsManager = new PhysicsManager();
	m_Root = new SceneNode();
}

GameState::~GameState()
{
	delete m_Root;
	delete renderManager;
	delete physicsManager;
}

void GameState::Update(float dt)
{
	m_Root->Update(dt);
}

void GameState::FixedUpdate(float dt)
{
	m_Root->FixedUpdate(dt);
	physicsManager->CalculateCollisions();
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




