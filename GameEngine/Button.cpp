#include "Button.h"
#include "ResourceManager.h"
#include "RenderingManager.h"
#include "InputManager.h"
#include "TextRenderer.h"
#include "GameState.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLFW Include
#include <GLFW\glfw3.h>
#include <GL/glew.h>

#include <iostream>

Button::Button(float width, float height) : Renderer()
{
	this->shader =	ResourceManager::LoadShader("TextVS.glsl", "ButtonFS.glsl", nullptr, "Button");
	this->shader.SetMatrix4("proj", glm::ortho(0.0f, static_cast<GLfloat>(m_Manager->Width), static_cast<GLfloat>(m_Manager->Height), 0.0f), GL_TRUE);
	this->shader.SetInteger("text", 0);
	this->Text = new TextRenderer(m_Manager->Width, m_Manager->Height);
	this->Text->Load("neuropolitical rg.ttf", 0.12 * width);
	this->MaxWidth = width;
	this->MaxHeight = height;
	this->xPos = 0;
	this->yPos = 0;

	InitButton();
	state = ButtonState::NORMAL;
	m_Manager->CurrentState()->m_RenderingManager->InsertRenderer(this);
}

Button::~Button()
{
	delete Text;
}

void Button::Render()
{
	if (gameObject == NULL)
	{
		return;
	}
	if (!Text->gameObject)
	{
		Text->gameObject = gameObject;
	}

	if (oldTrans.scale != gameObject->transform.scale)
	{
		Width = MaxWidth * gameObject->transform.scale.x;
		Height = MaxHeight * gameObject->transform.scale.y;
		oldTrans.scale = gameObject->transform.scale;
	}

	if (oldTrans.position != gameObject->transform.position)
	{
		xPos = gameObject->transform.position.x - Width / 2;
		yPos = gameObject->transform.position.y - Height / 2;
		oldTrans.position = gameObject->transform.position;
	}

	// Prepare transformations
	this->shader.Use();

	this->shader.SetVector3f("textColor", 1, 1, 1);

	glActiveTexture(GL_TEXTURE0);
	switch (state)
	{
	case ButtonState::NORMAL:
		normalTexture.Bind();
		break;
	case ButtonState::HOVER:
		hoverTexture.Bind();
		break;
	case ButtonState::PRESSED:
		pressedTexture.Bind();
		break;
	}

	GLfloat vertices[6][4] = {
		{ xPos,			yPos + Height,  0.0, 1.0 },
		{ xPos + Width, yPos,			1.0, 0.0 },
		{ xPos,			yPos,			0.0, 0.0 },

		{ xPos,			yPos + Height,  0.0, 1.0 },
		{ xPos + Width, yPos + Height,  1.0, 1.0 },
		{ xPos + Width, yPos,			1.0, 0.0 }
	};

	// Update content of VBO memory
	glBindBuffer(GL_ARRAY_BUFFER, this->VAO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Button::Update(float deltaTime)
{

	if (state != ButtonState::PRESSED)
	{
		if (InputManager::Instance().MouseX >= xPos && InputManager::Instance().MouseX <= xPos + Width && InputManager::Instance().MouseY >= yPos && InputManager::Instance().MouseY <= yPos + Height)
		{
			state = ButtonState::HOVER;
		}
		else
		{
			state = ButtonState::NORMAL;
		}
	}
	else
	{
		m_PressedTime -= deltaTime;
		if (m_PressedTime <= 0)
		{
			state = ButtonState::NORMAL;
		}
	}

	
	if (InputManager::Instance().MousePress(GLFW_MOUSE_BUTTON_1))
	{
		if (state == ButtonState::HOVER)
		{
			if (PressFunc != NULL)
				(*PressFunc)();
			state = ButtonState::PRESSED;
			m_PressedTime = 0.2f;
		}
	}
}

void Button::InitButton()
{
	this->normalTexture = ResourceManager::LoadTexture("Normal.png", GL_TRUE, "NormButton");
	this->hoverTexture = ResourceManager::LoadTexture("Hover.png", GL_TRUE, "HoverButton");
	this->pressedTexture = ResourceManager::LoadTexture("Pressed.png", GL_TRUE, "PressedButton");
	if (ResourceManager::GetVAO("Button"))
	{
		VAO = ResourceManager::GetVAO("Button");
		return;
	}

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	ResourceManager::LoadVAO(VAO, "Button");


}

Component* Button::GetComp(const type_info& type)
{
	if (type.name() == typeid(*this).name() || type.name() == typeid(this).name())
	{
		return this;
	}
	
	return NULL;
}

