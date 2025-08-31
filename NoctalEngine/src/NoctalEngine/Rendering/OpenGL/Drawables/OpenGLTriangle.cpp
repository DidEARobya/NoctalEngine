#include "OpenGLTriangle.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLVertexBuffer.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLIndexBuffer.h"
#include "NoctalEngine/Rendering/OpenGL/Shaders/OpenGLShaderProgram.h"
#include "NoctalEngine/Rendering/OpenGL/Textures/OpenGLTexture2D.h"

OpenGLTriangle::OpenGLTriangle(glm::vec2 position, glm::vec2 scale) : OpenGLBaseObject(position, scale)
{	//To Implement later
	//if (IsStaticInitialised() == false)
	//{
	//	
	//}
	//else
	//{
	//	SetIndexFromStatic();
	//}

	glCreateVertexArrays(1, &m_RendererID);
	glBindVertexArray(m_RendererID);

	AddMaterial(std::make_unique<NoctalEngine::Material>());
	NE_ENGINE_ASSERT(m_Material, "Failed to create Material");

	float vertices[3 * 5]
	{
		-0.5f, -0.5f, 0.0f,		0.0, 0.0f,
		 0.5f, -0.5f, 0.0f,		1.0, 0.0f,
		 0.0f,  0.5f, 0.0f,		0.5, 1.0f
	};

	AddBind(std::unique_ptr<NoctalEngine::VertexBuffer>(new OpenGLVertexBuffer(vertices, sizeof(vertices), {
		{ NoctalEngine::ShaderDataType::FLOAT_3, "a_Position"},
		{ NoctalEngine::ShaderDataType::FLOAT_2, "a_TexCoord"},
		})));

	uint32_t indices[3] = { 0, 1, 2 };
	SetIndexBuffer(std::unique_ptr<NoctalEngine::IndexBuffer>(new OpenGLIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t))));

	std::unique_ptr<OpenGLShaderProgram> shader = std::make_unique<OpenGLShaderProgram>(
		"ColouredTextureVS",
		"ColouredTextureFS",
		*this);

	if (shader->IsValid() == true)
	{
		m_Material->SetShader(std::move(shader));
		m_Material->SetBaseTexture(std::make_shared<OpenGLTexture2D>());
	}
	else
	{
		NE_ENGINE_ERROR("Drawable Created, but Shader Failed to bind");
	}

	m_ObjectBuffer = std::make_unique<OpenGLObjectUniformBufferObject>();
	NE_ENGINE_INFO("Triangle Created");
}
