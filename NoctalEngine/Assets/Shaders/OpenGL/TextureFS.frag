#version 330 core

layout(location = 0) out vec4 v_Colour;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	v_Colour = texture(u_Texture, v_TexCoord);
}	