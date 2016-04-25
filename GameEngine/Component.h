#ifndef COMPONENT_H
#define COMPONENT_H

#include "SceneNode.h"
#include "GameStateManager.h"


class Component
{
public:
	SceneNode *gameObject;

	Component();
	~Component();

public:
	virtual void Render() {};
	virtual void Update(float deltaTime) {};

	virtual Component* GetComp(const type_info& type);

protected:
	GameStateManager *m_Manager;
};

#endif