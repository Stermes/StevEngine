#ifndef SCENENODE_H
#define SCENENODE_H

#include <vector>
#include "NetworkIDObject.h"
#include "glm\glm.hpp"

#pragma once

struct Transform
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

class SceneNode : public RakNet::NetworkIDObject
{
public:
	// The parent of this node
	SceneNode *Parent;

	// Vector of children and components attached
	std::vector<SceneNode*> Children;
	std::vector<class Component*> Components;

	// Holds the nodes positions, rotation and scale
	Transform transform;
	Transform worldTransform;

	SceneNode();
	~SceneNode();

public:
	// The init happens at the start of the game before anything else is called
	virtual void Init();
	// Updates this nodes components and children
	virtual void Update(float deltaTime);
	// Updates this nodes components and children
	virtual void FixedUpdate(float deltaTime);

	// Adds a component to the vector of attached components
	void AddComponent(Component *comp);

	// Returns the first component found of the type
	Component* GetComponent(const type_info& type);

	// Returns an array of components found of the type
	std::vector<Component*> GetAllComponents(const type_info& type);

	// Adds a child to the vector of attached sceneNodes
	void AddChild(SceneNode *child);
		
	glm::mat4 GetModelMatrix()
	{
		return m_Model;
	}

	void SetIsMaster(bool isMaster)
	{
		m_IsMaster = isMaster;
	}

	bool GetIsMaster()
	{
		return m_IsMaster;
	}

protected:
	glm::mat4 m_Model;
	bool m_IsMaster;
};

#endif