#include "PhysicsBody.h"
#include "PhysicsManager.h"
#include "GameState.h"

PhysicsBody::PhysicsBody() : Component(), Gravity(-980)
{

}


PhysicsBody::~PhysicsBody()
{
}

void PhysicsBody::FixedUpdate(float dt)
{
	Velocity += glm::vec3(0, 1, 0) * Gravity * dt * dt;
	gameObject->transform.position += Velocity * dt;
}
