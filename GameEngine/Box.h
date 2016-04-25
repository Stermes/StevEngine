#pragma once
#include "GameObject.h"

class Box :	public GameObject
{
public:
	Box();
	~Box();

	virtual void Draw() override;
	virtual void Update(float deltaTime) override;
};

