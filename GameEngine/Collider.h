#ifndef COLLIDER_H
#define COLLIDER_H

#pragma once
#include "Component.h"

class Collider : public Component
{
public:
	int layer;

	Collider();
	~Collider();
};

#endif