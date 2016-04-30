#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#pragma once
#include <vector>
#include <list>

class PhysicsManager
{
public:	
	PhysicsManager();
	~PhysicsManager();
	
	void AddCollider(class Collider*);
	void RemoveCollider(class Collider*);

	void CalculateCollisions();

private:
	std::list<class Collider*> Colliders;
	std::list<class PhysicsBody*> PhysicsBodies;

};


#endif