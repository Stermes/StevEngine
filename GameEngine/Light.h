#pragma once
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct DirectionalLight
{
public:
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct SpotLight
{
public:
	glm::vec3 position;
	glm::vec3 direction;
	float cutOff;
	float outerCutOff;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct PointLight
{
public:
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	PointLight()
	{
		// Point light 1
		position = glm::vec3(0, 0, 0);
		ambient = glm::vec3(0.05f, 0.05f, 0.05f);
		diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
		specular = glm::vec3(1.0f, 1.0f, 1.0f);
		constant = 1.0f;
		linear = 0.09;
		quadratic = 0.032;

	}

	void SendToShader(Shader &shader, int num)
	{
		std::string loc = "pointLights[" + std::to_string(num) + "].position";
		shader.SetVector3f(loc.c_str(), position);
		loc = "pointLights[" + std::to_string(num) + "].ambient";
		shader.SetVector3f(loc.c_str(), ambient);
		loc = "pointLights[" + std::to_string(num) + "].diffuse";
		shader.SetVector3f(loc.c_str(), diffuse);
		loc = "pointLights[" + std::to_string(num) + "].specular";
		shader.SetVector3f(loc.c_str(), specular);
		loc = "pointLights[" + std::to_string(num) + "].constant";
		shader.SetFloat(loc.c_str(), constant);
		loc = "pointLights[" + std::to_string(num) + "].linear";
		shader.SetFloat(loc.c_str(), linear);
		loc = "pointLights[" + std::to_string(num) + "].quadratic";
		shader.SetFloat(loc.c_str(), quadratic);
	}
};

