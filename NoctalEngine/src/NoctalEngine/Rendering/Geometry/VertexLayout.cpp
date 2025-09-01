#include "VertexLayout.h"

namespace NoctalEngine
{
	constexpr size_t VertexLayout::Element::SizeOf(ElementType type)
	{
		switch (type)
		{
			case POS_2D:
				return sizeof(Map<POS_2D>::SysType);
			case POS_3D:
				return sizeof(Map<POS_3D>::SysType);
			case TEXCOORD:
				return sizeof(Map<TEXCOORD>::SysType);
		}

		NE_ENGINE_ASSERT(false, "Invalid element type");
		return 0u;
	}

	// VertexBuffer
	VertexBufferData::VertexBufferData(VertexLayout layout) : m_Layout(std::move(layout)) {}

	const char* VertexBufferData::GetData() const
	{
		return m_Buffer.data();
	}

	const VertexLayout& VertexBufferData::GetLayout() const noexcept
	{
		return m_Layout;
	}

	size_t VertexBufferData::Size() const 
	{
		return m_Buffer.size() / m_Layout.Size();
	}

	size_t VertexBufferData::SizeBytes() const 
	{
		return m_Buffer.size();
	}

	Vertex VertexBufferData::Back() 
	{
		NE_ENGINE_ASSERT(m_Buffer.size() != 0u, "Size is 0");
		return Vertex{ m_Buffer.data() + m_Buffer.size() - m_Layout.Size(), m_Layout };
	}

	Vertex VertexBufferData::Front() 
	{
		NE_ENGINE_ASSERT(m_Buffer.size() != 0u, "Size is 0");
		return Vertex{ m_Buffer.data(),m_Layout };
	}
	Vertex VertexBufferData::operator[](size_t i) 
	{
		NE_ENGINE_ASSERT(i < Size(), "i less than size");
		return Vertex{ m_Buffer.data() + m_Layout.Size() * i,m_Layout };
	}
	ConstVertex VertexBufferData::Back() const 
	{
		return const_cast<VertexBufferData*>(this)->Back();
	}
	ConstVertex VertexBufferData::Front() const 
	{
		return const_cast<VertexBufferData*>(this)->Front();
	}
	ConstVertex VertexBufferData::operator[](size_t i) const
	{
		return const_cast<VertexBufferData&>(*this)[i];
	}
}