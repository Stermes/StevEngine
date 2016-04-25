#ifndef RENDERINGMANAGER_H
#define RENDERINGMANAGER_H

#pragma once
#include "Singleton.h"

#include <GL/glew.h>

#include <map>
#include <list>

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

private:
	// List of all renderers
	std::list<class Renderer*> Renderers;

};

#endif