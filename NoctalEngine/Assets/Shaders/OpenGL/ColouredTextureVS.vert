#version 420 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in mat4 a_Transform;
			
layout(std140, binding = 0) uniform FrameUBO 
{
    mat4 u_ViewProjection;
};

//layout(std140, binding = 2) uniform ObjectUBO
//{
//    mat4 u_Transform;
//};

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * a_Transform * vec4(a_Position, 1.0);
}	
