#pragma once
//To implement later
//#include "OpenGLDrawable.h"
//
//template<class T>
//class OpenGLDrawableBase : public OpenGLDrawable
//{
//protected:
//	bool IsStaticInitialised() const noexcept
//	{
//		return !s_StaticBinds.empty();
//	}
//	static void AddStaticBind(std::unique_ptr<Bindable> bind)
//	{
//		NE_ENGINE_ASSERT(typeid(*bind) != typeid(NoctalEngine::IndexBuffer), "Must use AddIndexBuffer to bind index buffer");
//		s_StaticBinds.push_back(std::move(bind));
//	}
//	void SetStaticIndexBuffer(std::unique_ptr<NoctalEngine::IndexBuffer> indexBuffer)
//	{
//		NE_ENGINE_ASSERT(m_IndexBuffer == nullptr, "Attempted to add Index Buffer a second time");
//		m_IndexBuffer = indexBuffer.get();
//		s_StaticBinds.push_back(std::move(indexBuffer));
//	}
//
//	void SetIndexFromStatic()
//	{
//		NE_ENGINE_ASSERT(m_IndexBuffer == nullptr, "Attempting to add index buffer a second time");
//
//		for (const auto& b : s_StaticBinds)
//		{
//			if (const auto p = dynamic_cast<NoctalEngine::IndexBuffer*>(b.get()))
//			{
//				m_IndexBuffer = p;
//				return;
//			}
//		}
//
//		NE_ENGINE_ASSERT(m_IndexBuffer != nullptr, "Failed to find index buffer in static binds");
//	}
//
//private:
//	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override
//	{
//		return s_StaticBinds;
//	}
//
//private:
//	static std::vector<std::unique_ptr<Bindable>> s_StaticBinds;
//};
//
//template<class T>
//std::vector<std::unique_ptr<Bindable>> OpenGLDrawableBase<T>::s_StaticBinds;