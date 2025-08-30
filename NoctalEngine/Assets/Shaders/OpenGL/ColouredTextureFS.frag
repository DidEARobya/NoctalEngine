#version 420 core

layout(location = 0) out vec4 v_Colour;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Colour;

void main()
{
	v_Colour = texture(u_Texture, v_TexCoord) * u_Colour;
}	