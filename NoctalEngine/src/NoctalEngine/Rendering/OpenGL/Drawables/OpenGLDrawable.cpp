#pragma once
#include "OpenGLDrawable.h"
#include "NoctalEngine/Rendering/VertexBuffer.h"
#include "NoctalEngine/Rendering/OpenGL/OpenGLVertexBuffer.h"
#include "NoctalEngine/Rendering/OpenGL/OpenGLShader.h"
#include "NoctalEngine/Rendering/OpenGL/OpenGLTransformCBuffer.h"
#include "NoctalEngine/Rendering/Renderer.h"
#include "glm/gtc/matrix_transform.hpp"

OpenGLDrawable::OpenGLDrawable(NoctalEngine::Geometry geometry) : m_Position(glm::vec3(0.0f)), m_Scale(glm::mat4(1.0f))
{
	glCreateVertexArrays(1, &m_RendererID);
	glBindVertexArray(m_RendererID);

	std::string vertexSource;
	std::string pixelSource;

	switch (geometry)
	{
	case NoctalEngine::Geometry::TRIANGLE:
		{
			float verticesTri[3 * 7]
			{
				-0.5f, -0.5f, 0.0f,		1.0, 0.0f, 1.0f, 1.0f,
				 0.5f, -0.5f, 0.0f,		0.0, 0.0f, 1.0f, 1.0f,
				 0.0f,  0.5f, 0.0f,		1.0, 1.0f, 0.0f, 1.0f
			};

			AddBind(std::unique_ptr<NoctalEngine::VertexBuffer>(NoctalEngine::Renderer::Instance().CreateVertexBuffer(verticesTri, sizeof(verticesTri), { { NoctalEngine::ShaderDataType::FLOAT_3, "a_Position" },
				{ NoctalEngine::ShaderDataType::FLOAT_4, "a_Colour" } })));

			uint32_t indicesTri[3] = { 0, 1, 2 };
			AddIndexBuffer(std::unique_ptr<NoctalEngine::IndexBuffer>(NoctalEngine::Renderer::Instance().CreateIndexBuffer(indicesTri, sizeof(indicesTri) / sizeof(uint32_t))));

			vertexSource = R"(
				#version 330 core

				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Colour;
				
				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;

				out vec3 v_Position;
				out vec4 v_Colour;

				void main()
				{
					v_Position = a_Position;
					v_Colour = a_Colour;
					gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
				}	

			)";

			pixelSource = R"(
				#version 330 core

				layout(location = 0) out vec4 colour;

				in vec3 v_Position;
				in vec4 v_Colour;

				void main()
				{
					colour = vec4(v_Position * 0.5 + 0.5, 1.0);
					colour = v_Colour;
				}	

			)";

			AddBind(std::make_unique<OpenGLShader>(vertexSource, pixelSource, *this));
			NE_ENGINE_INFO("Triangle Created");
			break;
		}


	case NoctalEngine::Geometry::SQUARE:
		{
			float verticesSquare[3 * 4]
			{
				-0.75f, -0.75f, 0.0f,
				 0.75f, -0.75f, 0.0f,
				 0.75f,  0.75f, 0.0f,
				-0.75f,  0.75f, 0.0f
			};

			AddBind(std::unique_ptr<NoctalEngine::VertexBuffer>(NoctalEngine::Renderer::Instance().CreateVertexBuffer(verticesSquare, sizeof(verticesSquare), {
				{ NoctalEngine::ShaderDataType::FLOAT_3, "a_Position"},
				})));

			uint32_t indicesSquare[6] = { 0, 1, 2, 2, 3, 0 };
			AddIndexBuffer(std::unique_ptr<NoctalEngine::IndexBuffer>(NoctalEngine::Renderer::Instance().CreateIndexBuffer(indicesSquare, sizeof(indicesSquare) / sizeof(uint32_t))));

			vertexSource = R"(
				#version 330 core

				layout(location = 0) in vec3 a_Position;
				
				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;

				out vec3 v_Position;

				void main()
				{
					v_Position = a_Position;
					gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
				}	

			)";

			pixelSource = R"(
				#version 330 core

				layout(location = 0) out vec4 colour;

				in vec3 v_Position;

				void main()
				{
					colour = vec4(0.2, 0.3, 0.8, 1.0);
				}	

			)";

			AddBind(std::make_unique<OpenGLShader>(vertexSource, pixelSource, *this));
			NE_ENGINE_INFO("Square Created");
			break;
		}

	default:
		NE_ENGINE_ASSERT(false, "Geometry case doesn't exist");
		break;
	}
	
	//Transform currently handled in Shader
	//AddBind(std::make_unique<OpenGLTransformCBuffer>());
}

OpenGLDrawable::~OpenGLDrawable()
{
	NE_ENGINE_INFO("Drawable Destroyed");
	glDeleteVertexArrays(1, &m_RendererID);
}

const glm::mat4& OpenGLDrawable::GetTransform() const
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * m_Scale;
	return transform;
}

void OpenGLDrawable::Draw() const
{
	glBindVertexArray(m_RendererID);

	for (auto& bindable : m_Binds)
	{
		bindable->Bind();
	}

	NoctalEngine::Renderer::Instance().DrawIndexed();
}

void OpenGLDrawable::AddBind(std::unique_ptr<Bindable> bind)
{
	NE_ENGINE_ASSERT(typeid(*bind) != typeid(NoctalEngine::IndexBuffer), "Must use AddIndexBuffer to bind index buffer");

	glBindVertexArray(m_RendererID);
	bind->Bind();

	if (bind->GetLayout() != nullptr)
	{
		uint32_t index = 0;
		const auto& layout = *bind->GetLayout();

		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.Count(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			index++;
		}
	}

	m_Binds.push_back(std::move(bind));
}

void OpenGLDrawable::AddIndexBuffer(std::unique_ptr<NoctalEngine::IndexBuffer> indexBuffer)
{
	NE_ENGINE_ASSERT(m_IndexBuffer == nullptr, "Attempted to add Index Buffer a second time");

	glBindVertexArray(m_RendererID);
	indexBuffer->Bind();

	m_IndexBuffer = indexBuffer.get();
	m_Binds.push_back(std::move(indexBuffer));
}

GLenum OpenGLDrawable::ShaderDataTypeToOpenGLBaseType(NoctalEngine::ShaderDataType type)
{
	switch (type)
	{
	case NoctalEngine::ShaderDataType::FLOAT:		    return GL_FLOAT;
	case NoctalEngine::ShaderDataType::FLOAT_2:			return GL_FLOAT;
	case NoctalEngine::ShaderDataType::FLOAT_3:			return GL_FLOAT;
	case NoctalEngine::ShaderDataType::FLOAT_4:			return GL_FLOAT;
	case NoctalEngine::ShaderDataType::MAT_3:		    return GL_FLOAT;
	case NoctalEngine::ShaderDataType::MAT_4:		    return GL_FLOAT;
	case NoctalEngine::ShaderDataType::INT:				return GL_INT;
	case NoctalEngine::ShaderDataType::INT_2:		    return GL_INT;
	case NoctalEngine::ShaderDataType::INT_3:		    return GL_INT;
	case NoctalEngine::ShaderDataType::INT_4:		    return GL_INT;
	case NoctalEngine::ShaderDataType::BOOL:		    return GL_BOOL;
	}

	NE_ENGINE_ASSERT(false, "Unknown ShaderDataType");
	return 0;
}