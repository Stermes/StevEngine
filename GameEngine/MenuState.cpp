#include "MenuState.h"
#include "GameStateManager.h"
#include "InputManager.h"
#include "AudioManager.h"
#include <sstream>

#include "SceneNode.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "Button.h"
#include "TextRenderer.h"
#include <typeinfo>

#include "SceneLoad.h"

float lastTime = 0;

MenuState::MenuState() : GameState()
{
	//AudioManager::Instance().PlaySound2D("Cars/Rocky.mp3", true, false, false);
}

MenuState::~MenuState()
{
}

void MenuState::Init()
{

	TextRenderer *tRender = new TextRenderer(m_Manager->Width, m_Manager->Height);
	tRender->Load("neuropolitical rg.ttf", 24);

	Text = new SceneNode();
	Text->AddComponent(tRender);
	Text->transform.position.x = m_Manager->Width - 50;
	m_Root->AddChild(Text);

	ResourceManager::LoadShader("LightingVS.glsl", "SpriteFS.glsl", nullptr, "SpriteShader");
	ResourceManager::LoadTexture("red.jpg", GL_FALSE, "Red");

	camera = new Camera();
	camera->Position.z = 5;

	SceneNode* button = new SceneNode();
	Button *but = new Button(100, 50);

	but->Text->Text = "Host";
	but->PressFunc = SceneLoad::LoadHostMenu;
	button->AddComponent(but);
	button->transform.position.z = 5;
	button->transform.position.x = m_Manager->Width /2;
	button->transform.position.y = m_Manager->Height / 2 - 50;
	m_Root->AddChild(button);

	button = new SceneNode();
	Button *join = new Button(100, 50);

	join->Text->Text = "Join";
	join->PressFunc = SceneLoad::LoadJoinMenu;
	button->AddComponent(join);
	button->transform.position.z = 5;
	button->transform.position.x = m_Manager->Width / 2;
	button->transform.position.y = m_Manager->Height / 2 + 50;
	m_Root->AddChild(button);
}

void MenuState::Update(float dt)
{
	// view matrix
	glm::mat4 view;
	view = camera->GetViewMatrix();

	// projection matrix
	glm::mat4 projection = glm::ortho(
		(float)(-m_Manager->Width / 100),
		(float)(m_Manager->Width / 100),
		(float)(m_Manager->Height / 100),
		(float)(-m_Manager->Height / 100), 0.1f, 100.0f);

	// Set the value in our shader
	ResourceManager::GetShader("SpriteShader").SetMatrix4("view", view, GL_TRUE);
	ResourceManager::GetShader("SpriteShader").SetMatrix4("projection", projection);

	if (m_Manager->time - lastTime >= 1)
	{
		TextRenderer* tRender = (TextRenderer*)Text->GetComponent(typeid(TextRenderer));
		if (tRender)
		{
			std::stringstream ss;
			ss = std::stringstream();
			ss << m_Manager->frame;
			tRender->Text = ss.str();
			m_Manager->frame = 0;
			lastTime = m_Manager->time;
			ss.clear();
		}
	}

	m_Root->Update(dt);

}

void MenuState::FixedUpdate(float dt)
{
}


void MenuState::ShutDown()
{

}
