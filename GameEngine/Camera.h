/*#ifndef CAMERA_H
#define CAMERA_H

#include "SceneNode.h"

//std includes
#include <vector>

// GL includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



class Camera : public SceneNode
{
public:

	glm::vec3 Target;
	glm::vec3 Up;

	Camera();
	~Camera();

	virtual void Render() override;
	virtual void Update(float deltaTime) override;
	virtual void MouseInput(int key, int action) override;

protected:

private:

};

#endif
*/

#pragma once

//std includes
#include <vector>

// GL includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement
// Used as abstraction to stay away from window-system specific input methods
enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.05f;
const GLfloat ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors, and Matrices
class Camera 
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Eular Angles
	GLfloat Yaw;
	GLfloat Pitch;
	// Camera Options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	//Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->UpdateCameraVectors();
	}

	// Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->Position = glm::vec3(posX, posY, posZ);
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->UpdateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	// Processes input received from any keyboard-like input system. 
	// Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(CameraMovement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->MovementSpeed * deltaTime;
		if (direction == FORWARD)
		{
			this->Position += this->Front * velocity;
		}
		if (direction == BACKWARD)
		{
			this->Position -= this->Front * velocity;
		}
		if (direction == LEFT)
		{
			this->Position -= this->Right * velocity;
		}
		if (direction == RIGHT)
		{
			this->Position += this->Right * velocity;
		}
		if (direction == UP)
		{
			this->Position += this->Up * velocity;
		}

		if (direction == DOWN)
		{
			this->Position -= this->Up * velocity;
		}
		this->UpdateCameraVectors();
	}

	// Processes input received from a mouse input system
	// Expects the offset value in both the x and y direction
	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true)
	{
		xOffset *= this->MouseSensitivity;
		yOffset *= this->MouseSensitivity;

		// add the offset to our yaw and pitch
		Yaw += xOffset;
		Pitch += yOffset;

		// Constrain our pitch to positive 89 and -89 
		if (Pitch > 70.0f)
		{
			Pitch = 70.0f;
		}
		if (Pitch < -70.0f)
		{
			Pitch = -70.0f;
		}

		// Update Front, Right, and Up Vectors using updated Eular Angles
		this->UpdateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event.
	// Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(GLfloat yOffset)
	{
		if (this->Zoom >= 44.0f && this->Zoom <= 45.0f)
		{
			this->Zoom -= yOffset * 0.25f;
		}

		if (this->Zoom <= 44.0f)
		{
			this->Zoom = 44.0f;
		}

		if (this->Zoom >= 45.0f)
		{
			this->Zoom = 45.0f;
		}
	}

private:
	//Calculates the front vector from the Camera's (updated) Eular Angles
	void UpdateCameraVectors()
	{

		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) *  cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw) *  cos(glm::radians(Pitch)));
		this->Front = glm::normalize(front);
		//Also recalculate the Right and Up vector
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	}

};