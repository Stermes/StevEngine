#ifndef RENDERER_H
#define RENDERER_H

#pragma once
#include "Component.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"

class Renderer : public Component
{
public:
	Renderer() : Component() {};
	~Renderer();


	virtual void Render() = 0;
	virtual Component* GetComp(const type_info& type) override;

	Shader shader;
	GLuint VBO;
	GLuint VAO;

};

#endif;