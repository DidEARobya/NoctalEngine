#pragma once
#include "OpenGLDrawable.h"
#include "NoctalEngine/Rendering/Buffers/VertexArray.h"
#include "NoctalEngine/Rendering/Renderer.h"

template<class T>
class OpenGLDrawableBase : public OpenGLDrawable
{
protected:
	bool IsStaticInitialised() const noexcept
	{
		return !s_StaticBinds.empty();
	}

	static void SubmitVertexArrayToRenderer(NoctalEngine::Geometry geometry, std::unique_ptr<NoctalEngine::VertexArray> vertexArray)
	{
		NoctalEngine::Renderer::Instance().SubmitVertexArray(geometry, std::move(vertexArray));
	}

	static void AddStaticBind(std::unique_ptr<Bindable> bind)
	{
		NE_ENGINE_ASSERT(typeid(*bind) != typeid(NoctalEngine::IndexBuffer), "Must use AddIndexBuffer to bind index buffer");
		s_StaticBinds.push_back(std::move(bind));
	}

	void SetStaticMaterial(std::unique_ptr<NoctalEngine::Material> material)
	{
		NE_ENGINE_ASSERT(m_Material == nullptr, "Attempted to add Material a second time");
		m_Material = material.get();
		s_StaticBinds.push_back(std::move(material));
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
	static std::vector<std::unique_ptr<Bindable>> s_StaticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> OpenGLDrawableBase<T>::s_StaticBinds;