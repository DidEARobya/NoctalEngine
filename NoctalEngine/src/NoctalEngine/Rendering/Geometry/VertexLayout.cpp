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
		assert("Invalid element type" && false);
		return 0u;
	}

	// VertexBuffer
	VertexBufferData::VertexBufferData(VertexLayout layout)
		:
	layout(std::move(layout))
	{
	}
	const char* VertexBufferData::GetData() const
	{
		return buffer.data();
	}
	const VertexLayout& VertexBufferData::GetLayout() const noexcept
	{
		return layout;
	}
	size_t VertexBufferData::Size() const 
	{
		return buffer.size() / layout.Size();
	}
	size_t VertexBufferData::SizeBytes() const 
	{
		return buffer.size();
	}
	Vertex VertexBufferData::Back() 
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data() + buffer.size() - layout.Size(),layout };
	}
	Vertex VertexBufferData::Front() 
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data(),layout };
	}
	Vertex VertexBufferData::operator[](size_t i) 
	{
		assert(i < Size());
		return Vertex{ buffer.data() + layout.Size() * i,layout };
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