#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "Renderer.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"
#include <map>

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
	GLuint Advance;     // Horizontal offset to advance to next glyph
};

class TextRenderer : public Renderer
{
public:
	// Constructor
	TextRenderer(GLuint width, GLuint height);
	// The text that will be printed out
	std::string Text;

	// Pre-compiles a list of characters from the given font
	void Load(std::string font, GLuint fontSize);

	// Renders this nodes components and children
	virtual void Render() override;

	virtual Component* TextRenderer::GetComp(const type_info& type) override;

private:

	void InitText();
	// Holds a list of pre-compiled Characters
	std::map<GLchar, Character> Characters;
};

#endif 