#pragma once
#include "OpenGLDrawable.h"

template<class T>
class OpenGLDrawableBase : public OpenGLDrawable
{
protected:
	bool IsStaticInitialised() const noexcept
	{
		return !s_StaticBinds.empty();
	}
	static void SetRendererID(int32_t id)
	{
		NE_ENGINE_ASSERT(s_RendererID == 0, "Attempted to add Material a second time");
		s_RendererID = id;
	}

	static void AddStaticBind(std::unique_ptr<Bindable> bind)
	{
		NE_ENGINE_ASSERT(typeid(*bind) != typeid(NoctalEngine::IndexBuffer), "Must use AddIndexBuffer to bind index buffer");
		s_StaticBinds.push_back(std::move(bind));
	}
	void SetStaticIndexBuffer(std::unique_ptr<NoctalEngine::IndexBuffer> indexBuffer)
	{
		NE_ENGINE_ASSERT(m_IndexBuffer == nullptr, "Attempted to add Index Buffer a second time");
		m_IndexBuffer = indexBuffer.get();
		s_StaticBinds.push_back(std::move(indexBuffer));
	}

	void SetStaticMaterial(std::unique_ptr<NoctalEngine::Material> material)
	{
		NE_ENGINE_ASSERT(m_Material == nullptr, "Attempted to add Material a second time");
		m_Material = material.get();
		s_StaticBinds.push_back(std::move(material));
	}

	void SetIndexFromStatic()
	{
		NE_ENGINE_ASSERT(m_IndexBuffer == nullptr, "Attempting to add index buffer a second time");

		for (const auto& b : s_StaticBinds)
		{
			if (const auto p = dynamic_cast<NoctalEngine::IndexBuffer*>(b.get()))
			{
				m_IndexBuffer = p;
				return;
			}
		}

		NE_ENGINE_ASSERT(m_IndexBuffer != nullptr, "Failed to find Index Buffer in static binds");
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

	void SetRendererIDFromStatic()
	{
		NE_ENGINE_ASSERT(s_RendererID != 0, "Static RendererID doesn't exist");
		m_RendererID = s_RendererID;
	}

private:
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override
	{
		return s_StaticBinds;
	}

private:
	static uint32_t s_RendererID;
	static std::vector<std::unique_ptr<Bindable>> s_StaticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> OpenGLDrawableBase<T>::s_StaticBinds;

template<class T>
uint32_t OpenGLDrawableBase<T>::s_RendererID = 0;