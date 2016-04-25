// shadertype = <glsl>
#version 400 core
struct Material
{
	sampler2D diffuse;
};

in vec3 FragPos;  
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;

void main()
{
    vec4 texColor = texture(material.diffuse, TexCoords);
    if(texColor.a < 0.1)
        discard;
	vec3 texture = vec3(texture(material.diffuse, TexCoords));
	color = vec4(texture.x, texture.y, texture.z, 1.0f);

}
