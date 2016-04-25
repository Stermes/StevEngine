#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#pragma once
#include "Renderer.h"

class SpriteRenderer :	public Renderer
{
public:
	class Texture2D texture;

	SpriteRenderer(class Shader &shader, class Texture2D &texture);

	virtual void Render() override;

	~SpriteRenderer();

	virtual Component* GetComp(const type_info& type) override;
	glm::mat4 model;
	
private:
	void InitRenderer();
};

#endif