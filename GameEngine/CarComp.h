#ifndef CARCOMP_H
#define CARCOMP_H

#pragma once
#include "Component.h"
#include "irrKlang\irrKlang.h"
class CarComp :	public Component
{
public:
	CarComp(int num);
	~CarComp();

	void Accelerate(float deltaTime);
	void StopAccelerate();
	void Decelerate(float deltaTime);

	void SetSpeed(float speed)
	{
		m_Speed = speed;
	}

	virtual void Update(float deltaTime) override;

	int CarNum;

private:
	float m_Speed;
	float m_MaxSpeed;
	irrklang::ISound *EngineSound;

	bool AllreadyWinner;
};

#endif