#version 330 core

layout(location = 0) out vec4 v_Colour;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Colour;
uniform bool u_UseTexture;

void main()
{
	v_Colour =  u_UseTexture ? texture(u_Texture, v_TexCoord) * u_Colour : u_Colour;
}	