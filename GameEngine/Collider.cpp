#include "Collider.h"
#include "GameState.h"
#include "PhysicsManager.h"



Collider::Collider() : Component(), layer(1)
{
	m_Manager->CurrentState()->physicsManager->AddCollider(this);
}


Collider::~Collider()
{
}
