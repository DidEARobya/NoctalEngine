#include "nepch.h"
#include "OpenGLDrawable.h"
#include "NoctalEngine/Rendering/Renderer.h"

OpenGLDrawable::~OpenGLDrawable()
{
	NE_ENGINE_INFO("Drawable Destroyed");
}

void OpenGLDrawable::Draw() const
{
	for (auto& bindable : m_Binds)
	{
		bindable->Bind();
	}

	for (auto& bindable : GetStaticBinds())
	{
		bindable->Bind();
	}

	ObjectData data({ GetTransform() });
	m_ObjectBuffer->UpdateObjectData(data);
	m_ObjectBuffer->Bind();

	NoctalEngine::Renderer::Instance().DrawIndexed();
}

void OpenGLDrawable::AddBind(std::unique_ptr<Bindable> bind)
{
	NE_ENGINE_ASSERT(typeid(*bind) != typeid(NoctalEngine::IndexBuffer), "Must use AddIndexBuffer to bind index buffer");
	m_Binds.push_back(std::move(bind));
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