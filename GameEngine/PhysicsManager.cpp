#include "PhysicsManager.h"
#include "PhysicsBody.h"
#include "Collider.h"


PhysicsManager::PhysicsManager()
{
}


PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::CalculateCollisions()
{
	for (std::list<Collider*>::iterator it = Colliders.begin(); it != Colliders.end(); it++)
	{	
		for (std::list<Collider*>::iterator jt = it; jt != Colliders.end(); jt++)
		{
			Collider* first = *it;
			Collider* second = *jt;			
			if (it == jt || first->gameObject == second->gameObject || !(first->layer & second->layer))
			{
				continue;
			}
			
			printf("collision test");
		}
	}
}

void PhysicsManager::AddCollider(Collider* collider)
{
	Colliders.push_back(collider);
}
void PhysicsManager::RemoveCollider(Collider* collider)
{
	Colliders.remove(collider);
}