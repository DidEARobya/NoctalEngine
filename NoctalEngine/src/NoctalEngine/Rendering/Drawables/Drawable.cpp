#include "nepch.h"
#include "Drawable.h"

//Drawable::Drawable()
//{
//	float vertices[3 * 7]
//	{
//		-0.5f, -0.5f, 0.0f,		1.0, 0.0f, 1.0f, 1.0f,
//		 0.5f, -0.5f, 0.0f,		0.0, 0.0f, 1.0f, 1.0f,
//		 0.0f,  0.5f, 0.0f,		1.0, 1.0f, 0.0f, 1.0f
//	};

//	NoctalEngine::BufferLayout layout = {
//		{ NoctalEngine::ShaderDataType::FLOAT_3, "a_Position"},
//		{ NoctalEngine::ShaderDataType::FLOAT_4, "a_Colour"}
//	};

//	AddBind(Renderer::Instance().CreateVertexBuffer(vertices, sizeof(vertices), layout));

//	uint32_t indices[3] = { 0, 1, 2 };
//	AddIndexBuffer(Renderer::Instance().CreateIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));
//}

//void Drawable::Draw() const
//{
//	for (auto& bindable : m_Binds)
//	{
//		bindable->Bind();
//	}

//	Renderer::Instance().DrawIndexed();
//}

//void Drawable::AddBind(std::unique_ptr<Bindable> bind)
//{
//	NE_ENGINE_ASSERT(typeid(*bind) != typeid(IndexBuffer), "Must use AddIndexBuffer to bind index buffer");
//	m_Binds.push_back(std::move(bind));
//}
//void Drawable::AddIndexBuffer(std::unique_ptr<class IndexBuffer> indexBuffer)
//{
//	NE_ENGINE_ASSERT(m_IndexBuffer == nullptr, "Attempted to add Index Buffer a second time");
//	m_IndexBuffer = indexBuffer.get();
//	m_Binds.push_back(std::move(indexBuffer));
//}