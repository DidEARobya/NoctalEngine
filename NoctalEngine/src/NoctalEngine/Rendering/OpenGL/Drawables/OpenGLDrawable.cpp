#include "OpenGLDrawable.h"
#include "NoctalEngine/Rendering/Buffers/VertexBuffer.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLVertexBuffer.h"
#include "NoctalEngine/Rendering/OpenGL/Shaders/OpenGLShaderProgram.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLTransformCBuffer.h"
#include "NoctalEngine/Rendering/OpenGL/Textures/OpenGLTexture2D.h"
#include "NoctalEngine/Rendering/Renderer.h"
#include "glm/gtc/matrix_transform.hpp"

OpenGLDrawable::OpenGLDrawable(NoctalEngine::Geometry geometry) : m_Position(glm::vec3(0.0f)), m_Scale(glm::mat4(1.0f))
{
	glCreateVertexArrays(1, &m_RendererID);
	glBindVertexArray(m_RendererID);

	AddMaterial(std::make_unique<NoctalEngine::Material>());
	NE_ENGINE_ASSERT(m_Material, "Failed to create Material");

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

			std::unique_ptr<OpenGLShaderProgram> shader = std::make_unique<OpenGLShaderProgram>(
				ASSET_DIR "Shaders/OpenGL/SolidColourVS.glsl",
				ASSET_DIR "Shaders/OpenGL/SolidColourFS.glsl",
				*this);

			if (shader->IsValid() == true)
			{
				m_Material->BindShader(std::move(shader));
			}
			else
			{
				NE_ENGINE_ERROR("Drawable Created, but Shader Failed to bind");
			}

			NE_ENGINE_INFO("Triangle Created");
			break;
		}
	case NoctalEngine::Geometry::SQUARE:
		{
			float verticesSquare[3 * 4]
			{
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.5f,  0.5f, 0.0f,
				-0.5f,  0.5f, 0.0f
			};

			AddBind(std::unique_ptr<NoctalEngine::VertexBuffer>(NoctalEngine::Renderer::Instance().CreateVertexBuffer(verticesSquare, sizeof(verticesSquare), {
				{ NoctalEngine::ShaderDataType::FLOAT_3, "a_Position"}
				})));

			uint32_t indicesSquare[6] = { 0, 1, 2, 2, 3, 0 };
			AddIndexBuffer(std::unique_ptr<NoctalEngine::IndexBuffer>(NoctalEngine::Renderer::Instance().CreateIndexBuffer(indicesSquare, sizeof(indicesSquare) / sizeof(uint32_t))));

			std::unique_ptr<OpenGLShaderProgram> shader = std::make_unique<OpenGLShaderProgram>(
				"SolidColourVS",
				"SolidColourFS",
				*this);

			if (shader->IsValid() == true)
			{
				m_Material->BindShader(std::move(shader));
			}
			else
			{
				NE_ENGINE_ERROR("Drawable Created, but Shader Failed to bind");
			}

			NE_ENGINE_INFO("Square Created");
			break;
		}
	case NoctalEngine::Geometry::TEX_SQUARE:
	{
		float verticesSquare[5 * 4]
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		AddBind(std::unique_ptr<NoctalEngine::VertexBuffer>(NoctalEngine::Renderer::Instance().CreateVertexBuffer(verticesSquare, sizeof(verticesSquare), {
			{ NoctalEngine::ShaderDataType::FLOAT_3, "a_Position"},
			{ NoctalEngine::ShaderDataType::FLOAT_2, "a_TexCoord"},
			})));

		uint32_t indicesSquare[6] = { 0, 1, 2, 2, 3, 0 };
		AddIndexBuffer(std::unique_ptr<NoctalEngine::IndexBuffer>(NoctalEngine::Renderer::Instance().CreateIndexBuffer(indicesSquare, sizeof(indicesSquare) / sizeof(uint32_t))));

		std::unique_ptr<OpenGLShaderProgram> shader = std::make_unique<OpenGLShaderProgram>(
			"TextureVS",
			"TextureFS.glsl",
			*this);

		if (shader->IsValid() == true)
		{
			m_Material->BindShader(std::move(shader));
		}
		else
		{
			NE_ENGINE_ERROR("Drawable Created, but Shader Failed to bind");
		}

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

void OpenGLDrawable::AddMaterial(std::unique_ptr<NoctalEngine::Material> material)
{
	NE_ENGINE_ASSERT(m_Material == nullptr, "Attempted to add Material a second time");

	m_Material = material.get();
	m_Binds.push_back(std::move(material));
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