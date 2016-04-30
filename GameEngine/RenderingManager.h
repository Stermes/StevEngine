#ifndef RENDERINGMANAGER_H
#define RENDERINGMANAGER_H

#pragma once
#include "Singleton.h"

#include <GL/glew.h>

#include <map>
#include <list>
#include <vector>

class RenderingManager
{
	friend class Renderer;
public:

	RenderingManager();
	~RenderingManager();

	// Renders all the renderers in the render list
	void Render();

	// Inserts into renderer into the renderer list
	void InsertRenderer(class Renderer*);
	// Removes this renderer from the rendering lsit
	void RemoveRenderer(class Renderer*);



	GLuint VBO;
	GLuint VAO;

	enum RenderType
	{
		SPRITE = 0,
		TEXT = 1,
		GUI = 2,
	};

	int offset;
	// This will store all the vertices that we need to upload
	std::vector <GLfloat> vertices;
	std::vector<class Texture2D> Tex;
	std::vector<class Renderer*> Renders;

	// New Rendering technique
	void InsertRenderBatch(class Renderer*, RenderType Type);

	void CreateNewBuffer();
private:
	// List of all renderers
	std::list<class Renderer*> Renderers;

};

#endif