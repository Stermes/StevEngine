#include "Renderer.h"


Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
}


Component* Renderer::GetComp(const type_info& type)
{
	if (type.name() == typeid(*this).name() || type.name() == typeid(this).name())
	{
		return this;
	}

	return NULL;
}