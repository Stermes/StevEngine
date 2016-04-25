#include <iostream>

// GLEW Include
#include <GL\glew.h>

// GLFW Include
#include <GLFW\glfw3.h>

// other includes
#include "GameStateManager.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "MenuState.h"

// Function prototypes
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

GLuint WIDTH = 800, HEIGHT = 500;

GameStateManager *game = &GameStateManager::Instance();

int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "StevEngine", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make the window our main context on current thread
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 0);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	game->Width = WIDTH;
	game->Height = HEIGHT;
	game->Init();

	game->PushState(new MenuState());

	// Change in time between frames
	GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
	GLfloat lastFrame = 0.0f;  	// Time of last frame

	// Game Loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate the change in frames
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check call events
		glfwPollEvents();
		
		game->UpdateState(deltaTime);

		// Rendering and etc
		glClearColor(0.30f, 0.40f, 0.50f, 1.0f);
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game->RenderState();

		InputManager::Instance().EndFrame();
		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true,
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (action == GLFW_PRESS)
	{
		InputManager::Instance().keys[key].Down = true;
		InputManager::Instance().keys[key].Press = true;
		InputManager::Instance().keys[key].Release = false;
	}
	else if (action == GLFW_RELEASE)
	{
		InputManager::Instance().keys[key].Down = false;
		InputManager::Instance().keys[key].Press = false;
		InputManager::Instance().keys[key].Release = true;
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	InputManager::Instance().MouseX = xpos;
	InputManager::Instance().MouseY = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		InputManager::Instance().MouseClick[button].Down = true;
		InputManager::Instance().MouseClick[button].Press = true;
		InputManager::Instance().MouseClick[button].Release = false;
	}
	else if (action == GLFW_RELEASE)
	{
		InputManager::Instance().MouseClick[button].Down = false;
		InputManager::Instance().MouseClick[button].Press = false;
		InputManager::Instance().MouseClick[button].Release = true;
	}
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{

}
