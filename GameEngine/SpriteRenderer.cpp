#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "RenderingManager.h"
#include "GameState.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLFW Include
#include <GLFW\glfw3.h>

#include <SOIL\SOIL.h>
#include "SceneNode.h"

SpriteRenderer::SpriteRenderer(Shader &shader, Texture2D &texture)
{
	this->shader = shader;
	this->texture = texture;
	//InitRenderer();
	//m_Manager->CurrentState()->renderManager->InsertRenderer(this);
	m_Manager->CurrentState()->renderManager->InsertRenderBatch(this, RenderingManager::RenderType::SPRITE);
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Render()
{
	// Prepare transformations
	this->shader.Use();
	
	shader.SetMatrix4("model", gameObject->GetModelMatrix());

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	//glDrawArrays(GL_TRIANGLES, 0, 6);

}


void SpriteRenderer::InitRenderer()
{
	//if (ResourceManager::GetVAO("Sprite"))
	//{
	//	VAO = ResourceManager::GetVAO("Sprite");
	//	return;
	//}

	GLfloat Cube[] = {
		// Positions           // Normals           // Texture Coords
		-(GLfloat)texture.Width / 2, -(GLfloat) texture.Height/2, -0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		(GLfloat)texture.Width / 2, -(GLfloat)texture.Height/2, -0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		(GLfloat)texture.Width / 2,  (GLfloat)texture.Height/2, -0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		(GLfloat)texture.Width / 2,  (GLfloat)texture.Height/2, -0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-(GLfloat)texture.Width / 2,  (GLfloat)texture.Height/2, -0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-(GLfloat)texture.Width / 2, -(GLfloat)texture.Height/2, -0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	};


	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Cube), Cube, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	glBindVertexArray(0);
	
	//ResourceManager::LoadVAO(VAO, "Sprite");
}


Component* SpriteRenderer::GetComp(const type_info& type)
{
	if (type.name() == typeid(*this).name() || type.name() == typeid(this).name())
	{
		return this;
	}

	return NULL;
}