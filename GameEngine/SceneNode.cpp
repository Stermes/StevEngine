#include "SceneNode.h"
#include "Component.h"
#include <algorithm>
#include <typeinfo>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SceneNode::SceneNode()
{
	transform.position = glm::vec3(0.0f);
	transform.rotation = glm::vec3(0.0f);
	transform.scale = glm::vec3(1.0f);
	m_IsMaster = true;
}

SceneNode::~SceneNode()
{
	//Render each component that is attached to this object
	for (int i = 0; i < Components.size(); i++)
	{
		delete Components[i];
	}
	Components.clear();

	// Render each child attached to this object
	for (int i = 0; i < Children.size(); i++)
	{
		delete Children[i];
	}
	Children.clear();
}

void SceneNode::Init()
{
	//Update each component that is attached to this object
	for (int i = 0; i < Components.size(); i++)
	{
		Components[i]->Init();
	}

	// Update each child attached to this object
	for (int i = 0; i < Children.size(); i++)
	{
		Children[i]->Init();
	}
}

void SceneNode::Update(float deltaTime)
{
	m_Model = glm::mat4();
	if (Parent)
	{
		m_Model = Parent->m_Model;
	}

	m_Model = glm::translate(m_Model, glm::vec3(transform.position.x, -transform.position.y, transform.position.z));
	m_Model = glm::rotate(m_Model, transform.rotation.x, glm::vec3(1, 0, 0));
	m_Model = glm::rotate(m_Model, transform.rotation.y, glm::vec3(0, 1, 0));
	m_Model = glm::rotate(m_Model, transform.rotation.z, glm::vec3(0, 0, 1));
	m_Model = glm::scale(m_Model, transform.scale); // Last scale

	//Update each component that is attached to this object
	for (int i = 0; i < Components.size(); i++)
	{
		Components[i]->Update(deltaTime);
	}

	// Update each child attached to this object
	for (int i = 0; i < Children.size(); i++)
	{
		Children[i]->Update(deltaTime);
	}
}

void SceneNode::FixedUpdate(float deltaTime)
{
	//Update each component that is attached to this object
	for (int i = 0; i < Components.size(); i++)
	{
		Components[i]->FixedUpdate(deltaTime);
	}

	// Update each child attached to this object
	for (int i = 0; i < Children.size(); i++)
	{
		Children[i]->FixedUpdate(deltaTime);
	}
}


void SceneNode::AddComponent(Component* comp)
{
	// Set the components owner gameobjec to this
	comp->gameObject = this;
	Components.push_back(comp);
}

Component* SceneNode::GetComponent(const type_info& type)
{
	for each (Component* comp in Components)
	{
		if (comp->GetComp(type))
		{
			return comp;
		}
	}
	return NULL;
}


std::vector<Component*> SceneNode::GetAllComponents(const type_info& type)
{
	std::vector<Component*> foundComps;

	// Iterate through all attached components, if one is a component of the type we are looking for, then add it to the vector
	for each (Component* comp in Components)
	{
		if (comp->GetComp(type))
		{
			foundComps.push_back(comp);
		}
	}
	return foundComps;
}


void SceneNode::AddChild(SceneNode* child)
{
	// Set the child's parent to this 
	child->Parent = this;
	Children.push_back(child);
}
