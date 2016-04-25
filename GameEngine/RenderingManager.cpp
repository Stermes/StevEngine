#include "RenderingManager.h"
#include "Renderer.h"

RenderingManager::RenderingManager()
{
}


RenderingManager::~RenderingManager()
{

}

// Renders all the renderers in the render list
void RenderingManager::Render()
{
	int lastNum = -1;
	for (std::list<Renderer*>::iterator it = Renderers.begin(); it != Renderers.end(); it++)
	{
		Renderer* myRenderer = (Renderer*)*it;

		if (lastNum != myRenderer->VAO)
		{
			lastNum = myRenderer->VAO;
		}
		glBindVertexArray(myRenderer->VAO);
		myRenderer->Render();

	}
}

// Inserts into renderer into the renderer list
void RenderingManager::InsertRenderer(Renderer* renderer)
{
	int rendererPoints = renderer->VAO;

	if (Renderers.size() <= 0)
	{
		Renderers.push_back(renderer);
	}

	for (std::list<Renderer*>::iterator it = Renderers.begin(); it != Renderers.end(); it++)
	{
		Renderer* myRenderer = (Renderer*)*it;
		if (myRenderer->VAO < renderer->VAO)
		{
			continue;
		}

		if (myRenderer->VAO == renderer->VAO)
		{
			Renderers.insert(it, renderer);
			return;
		}

		if (myRenderer->VAO > renderer->VAO)
		{
			continue;
		}
	}

	Renderers.push_back(renderer);
}

// Removes this renderer from the rendering lsit
void RenderingManager::RemoveRenderer(Renderer* renderer)
{

}