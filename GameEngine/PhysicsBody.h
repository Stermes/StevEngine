#ifndef PHYSICSBODY_H
#define PHYSICSBODY_H

#pragma once
#include "Component.h"

class PhysicsBody :	public Component
{
public:
	PhysicsBody();
	~PhysicsBody();

	virtual void FixedUpdate(float deltaTime) override;
	
	glm::vec3 Velocity;
	float Gravity;
};

#endif