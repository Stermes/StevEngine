#include "RenderingManager.h"
#include "Texture.h"
#include "SpriteRenderer.h"
#include "Renderer.h"

RenderingManager::RenderingManager()
{
	CreateNewBuffer();
}


RenderingManager::~RenderingManager()
{

}

// Renders all the renderers in the render list
void RenderingManager::Render()
{
	glBindVertexArray(VAO);

	for (int i = 0; i < Renders.size(); i++)
	{
		Renders[i]->Render();

		glDrawArrays(GL_TRIANGLES, i * 6, 6);
	}

	glBindVertexArray(0);
	int lastNum = -1;
	for (std::list<Renderer*>::iterator it = Renderers.begin(); it != Renderers.end(); it++)
	{
		Renderer* myRenderer = (Renderer*)*it;

		if (lastNum != myRenderer->VAO)
		{
			glBindVertexArray(myRenderer->VAO);
			lastNum = myRenderer->VAO;
		}
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

// Inserts into renderer into the renderer list
void RenderingManager::InsertRenderBatch(Renderer* renderer, RenderingManager::RenderType type)
{

	SpriteRenderer* sRender = (SpriteRenderer*)renderer;

	if (!sRender)
	{
		return;
	}
	Renders.push_back(sRender);

	GLfloat Sprite[] =
	{
		// Positions												          // Texture Coords
		-(GLfloat)sRender->texture.Width / 2, -(GLfloat)sRender->texture.Height / 2, 0.0f, 0.0f,
		(GLfloat)sRender->texture.Width / 2, -(GLfloat)sRender->texture.Height / 2,  1.0f, 0.0f,
		(GLfloat)sRender->texture.Width / 2,  (GLfloat)sRender->texture.Height / 2,   1.0f, 1.0f,
		(GLfloat)sRender->texture.Width / 2,  (GLfloat)sRender->texture.Height / 2,  1.0f, 1.0f,
		-(GLfloat)sRender->texture.Width / 2,  (GLfloat)sRender->texture.Height / 2, 0.0f, 1.0f,
		-(GLfloat)sRender->texture.Width / 2, -(GLfloat)sRender->texture.Height / 2, 0.0f, 0.0f,
	};

	for (int i = 0; i < 24; i++)
	{
		vertices.push_back(Sprite[i]);
	}



	// Bind our VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Orphan the buffer (for speed)
	glBufferData(GL_ARRAY_BUFFER, vertices.size(), nullptr, GL_DYNAMIC_DRAW);
	// Upload the data
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size(), vertices.data());

	// Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderingManager::CreateNewBuffer()
{
	if (VAO == 0)
	{
		glGenVertexArrays(1, &this->VAO);
	}
	glBindVertexArray(VAO);

	if (VBO == 0)
	{
		glGenBuffers(1, &VBO);
	}


	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);

	// Position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

	// normal attribut

	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
}

// Removes this renderer from the rendering lsit
void RenderingManager::RemoveRenderer(Renderer* renderer)
{

}