#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "TextRenderer.h"
#include "ResourceManager.h"
#include "RenderingManager.h"
#include "GameState.h"


TextRenderer::TextRenderer(GLuint width, GLuint height) : Renderer()
{
	// Load and configure shader
	this->shader = ResourceManager::LoadShader("TextVS.glsl", "TextFS.glsl", nullptr, "Text");
	this->shader.SetMatrix4("proj", glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f), GL_TRUE);
	this->shader.SetInteger("text", 0);
	this->Text = "Text";

	InitText();
	m_Manager->CurrentState()->m_RenderingManager->InsertRenderer(this);
}

void TextRenderer::Load(std::string font, GLuint fontSize)
{
	// First clear the previously loaded Characters
	this->Characters.clear();
	// Then initialize and load the FreeType library
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) // All functions return a value different than 0 whenever an error occurred
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, font.c_str(), 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, fontSize);
	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Then for the first 128 ASCII characters, pre-load/compile their characters and store them
	for (GLubyte c = 0; c < 128; c++) // lol see what I did there 
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
			);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::Render()
{
	if (gameObject == NULL)
	{
		return;
	}
	// Activate corresponding render state	
	this->shader.Use();
	this->shader.SetVector3f("textColor", 1, 1, 1);
	glActiveTexture(GL_TEXTURE0);

	GLfloat x = gameObject->transform.position.x;
	int y = gameObject->transform.position.y;

	// Iterate through all characters
	std::string::const_iterator c;
	GLfloat start = 0;
	for (c = Text.begin(); c != Text.end(); c++)
	{
		Character ch = Characters[*c];
		start += ch.Size.x * gameObject->transform.scale.x;
	}
	x -= start / 2;

	for (c = Text.begin(); c != Text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + (ch.Bearing.x * gameObject->transform.scale.x);
		GLfloat ypos = y + (this->Characters['H'].Bearing.y - ch.Bearing.y) * gameObject->transform.scale.y;

		GLfloat w = ch.Size.x * gameObject->transform.scale.x;
		GLfloat h = ch.Size.y * gameObject->transform.scale.y;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 },
			{ xpos,     ypos,       0.0, 0.0 },

			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 }
		};
		
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, this->VAO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph
		x += (ch.Advance >> 6) * gameObject->transform.scale.x; // Bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::InitText()
{
	if (ResourceManager::GetVAO("Text"))
	{
		VAO = ResourceManager::GetVAO("Text");
		return;
	}

	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	ResourceManager::LoadVAO(VAO, "Text");
}


Component* TextRenderer::GetComp(const type_info& type)
{
	if (type.name() == typeid(*this).name() || type.name() == typeid(this).name())
	{
		return this;
	}

	return NULL;
}