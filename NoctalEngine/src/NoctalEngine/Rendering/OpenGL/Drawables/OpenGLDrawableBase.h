#pragma once
#include "OpenGLDrawable.h"
#include "NoctalEngine/Rendering/RendererData.h"
#include "NoctalEngine/Rendering/Geometry/Geometry.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLVertexArray.h"

template<class T>
class OpenGLDrawableBase : public OpenGLDrawable
{
protected:
	bool IsStaticInitialised() const noexcept
	{
		return s_VertexArrayPtr != nullptr;
	}
	static void AddStaticBind(std::unique_ptr<Bindable> bind)
	{
		NE_ENGINE_ASSERT(typeid(*bind) != typeid(NoctalEngine::IndexBuffer), "Must use AddIndexBuffer to bind index buffer");
		s_StaticBinds.push_back(std::move(bind));
	}
	void SetStaticVertexArray(NoctalEngine::Geometry geometry, const NoctalEngine::VertexBufferData& bufferData, const std::vector<uint32_t>& indices)
	{
		NE_ENGINE_ASSERT(m_VertexArray == nullptr, "Attempted to add VertexArray a second time");
		RendererData::AddVertexArray(geometry, std::make_unique<OpenGLVertexArray>(bufferData, indices));
		s_VertexArrayPtr = RendererData::GetVertexArray(geometry);
		m_VertexArray = RendererData::GetVertexArray(geometry);
	}
	void SetStaticMaterial(std::unique_ptr<NoctalEngine::Material> material)
	{
		NE_ENGINE_ASSERT(m_Material == nullptr, "Attempted to add Material a second time");
		m_Material = material.get();
		s_StaticBinds.push_back(std::move(material));
	}

	void SetVertexArrayFromStatic(NoctalEngine::Geometry geometry)
	{
		NE_ENGINE_ASSERT(m_VertexArray == nullptr, "Attempting to add VertexArray a second time");
		m_VertexArray = RendererData::GetVertexArray(geometry);
		m_VertexArray->Bind();
		NE_ENGINE_ASSERT(m_VertexArray != nullptr, "Failed to find static VertexArray");
	}
	void SetMaterialFromStatic()
	{
		NE_ENGINE_ASSERT(m_Material == nullptr, "Static Material doesn't exist");

		for (const auto& b : s_StaticBinds)
		{
			if (const auto p = dynamic_cast<NoctalEngine::Material*>(b.get()))
			{
				m_Material = p;
				return;
			}
		}

		NE_ENGINE_ASSERT(m_Material != nullptr, "Failed to find Material in static binds");
	}

private:
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override
	{
		return s_StaticBinds;
	}

private:
	static NoctalEngine::VertexArray* s_VertexArrayPtr;
	static std::vector<std::unique_ptr<Bindable>> s_StaticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> OpenGLDrawableBase<T>::s_StaticBinds;
template<class T>
NoctalEngine::VertexArray* OpenGLDrawableBase<T>::s_VertexArrayPtr = nullptr;