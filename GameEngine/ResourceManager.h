#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string.h>

#include "GL/glew.h"

#include "Texture.h"
#include "Shader.h"

class ResourceManager
{
public:
	// Resource storage
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;
	static std::map<std::string, GLuint> VAO;
	
	// Loads and gnereates a shader program from file loading vertex, fragmetn and geometry shader's source code
	static Shader LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
	// Retrieves a stored sader
	static Shader GetShader(std::string name);
	// Loads (and generates) a texture from file
	static Texture2D LoadTexture(const GLchar *file, GLboolean alpha, std::string name);
	// Retrieves a stored texture
	static Texture2D GetTexture(std::string name);
	// Loads the vertices into the map of vertices
	static GLuint LoadVAO(GLuint vao, std::string name);
	// Returns the vertices from the map of vertices
	static GLuint GetVAO(std::string name);
	// Properly de-allocates all loaded resources
	static void Clear();
	// Removes a shader from the map with the specified name
	static void RemoveShader(std::string name);
	// Removes a texture from the map with the specified name
	static void RemoveTexture(std::string name);

private:
	ResourceManager();

	// loads and generates a shader from file
	static Shader LoadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
	// loads a single texture from file
	static Texture2D LoadTextureFromFile(const GLchar *file, GLboolean alpha);

};

#endif

