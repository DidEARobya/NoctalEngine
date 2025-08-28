#version 330 core

layout(location = 0) in vec3 a_Position;
				
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform vec4 u_Colour;

out vec4 v_Colour;

void main()
{
	v_Colour = u_Colour;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}	