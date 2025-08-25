#pragma once
#include "NoctalEngine/Rendering/Bindable.h"
#include "NoctalEngine/Rendering/IndexBuffer.h"

class Drawable
{
public:
	virtual ~Drawable() = default;

	//virtual void Update(float deltaTime) noexcept = 0;

	virtual void Draw() const = 0;

	virtual void AddBind(std::unique_ptr<Bindable> bind) = 0;
	virtual void AddIndexBuffer(std::unique_ptr<NoctalEngine::IndexBuffer> indexBuffer) = 0;
};


	//virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;
	
	//const class IndexBuffer* m_IndexBuffer = nullptr;
	//std::vector<std::unique_ptr<Bindable>> m_Binds;