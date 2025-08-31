#pragma once
#include "Drawable.h"

template<class T>
class DrawableBase : public Drawable
{
protected:
	bool IsStaticInitialised() const noexcept
	{
		return !_staticBinds.empty();
	}

	virtual static void AddStaticBind(std::unique_ptr<Bindable> bind) = 0;
	virtual void SetStaticIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer) = 0;

	void SetIndexFromStatic(NoctalEngine::IndexBuffer& indexBuffer) noexcept(!IS_DEBUG)
	{
		NE_ENGINE_ASSERT(indexBuffer == nullptr, "Attempting to add index buffer a second time");

		for (const auto& b : _staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
			{
				indexBuffer = p;
				return;
			}
		}

		NE_ENGINE_ASSERT(indexBuffer != nullptr, "Failed to find index buffer in static binds");
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
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::s_StaticBinds;