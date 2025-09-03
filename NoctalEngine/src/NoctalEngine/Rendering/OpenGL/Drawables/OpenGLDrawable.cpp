#include "nepch.h"
#include "OpenGLDrawable.h"
#include "NoctalEngine/Rendering/Buffers/IndexBuffer.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLVertexArray.h"
#include "NoctalEngine/Rendering/Renderer.h"
#include "NoctalEngine/Rendering/RendererData.h"

OpenGLDrawable::~OpenGLDrawable()
{
	NE_ENGINE_INFO("Drawable Destroyed");
	glDeleteVertexArrays(1, &m_RendererID);
}

void OpenGLDrawable::Draw() const
{
	for (auto& bindable : m_Binds)
	{
		bindable->Bind();
	}

	//To implement later
	for (auto& bindable : GetStaticBinds())
	{
		bindable->Bind();
	}

	m_VertexArray->IncrementStoredVertexData(GetTransform());
	RendererData::AddToIndexCount(m_VertexArray->GetIndices());

	//ObjectData data({ GetTransform() });
	//m_ObjectBuffer->UpdateObjectData(data);
	//m_ObjectBuffer->Bind();
}

void OpenGLDrawable::AddBind(std::unique_ptr<Bindable> bind)
{
	NE_ENGINE_ASSERT(typeid(*bind) != typeid(NoctalEngine::IndexBuffer), "Must use SetIndexBuffer to bind IndexBuffer");
	NE_ENGINE_ASSERT(typeid(*bind) != typeid(NoctalEngine::VertexArray), "Must use SetVertexArray to bind VertexArray");
	m_Binds.push_back(std::move(bind));
}

void OpenGLDrawable::SetVertexArray(std::unique_ptr<NoctalEngine::VertexArray> vertexArray)
{
	NE_ENGINE_ASSERT(m_VertexArray == nullptr, "Attempted to add VertexArray a second time");
	m_VertexArray = vertexArray.get();
}

void OpenGLDrawable::SetMaterial(std::unique_ptr<NoctalEngine::Material> material)
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
	case NoctalEngine::ShaderDataType::POS_2D:			return GL_FLOAT;
	case NoctalEngine::ShaderDataType::POS_3D:			return GL_FLOAT;
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