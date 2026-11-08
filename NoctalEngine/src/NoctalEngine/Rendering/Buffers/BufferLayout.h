#pragma once

namespace NoctalEngine
{
	enum class ShaderDataType
	{
		NONE = 0,
		FLOAT,
		POS_2D,
		POS_3D,
		FLOAT_4,
		MAT_3,
		MAT_4,
		INT,
		INT_2,
		INT_3,
		INT_4,
		BOOL
	};

	inline uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::FLOAT:		    return 4;
			case ShaderDataType::POS_2D:		return 4 * 2;
			case ShaderDataType::POS_3D:		return 4 * 3;
			case ShaderDataType::FLOAT_4:		return 4 * 4;
			case ShaderDataType::MAT_3:		    return 4 * 3 * 3;
			case ShaderDataType::MAT_4:		    return 4 * 4 * 4;
			case ShaderDataType::INT:		    return 4;
			case ShaderDataType::INT_2:		    return 4 * 2;
			case ShaderDataType::INT_3:		    return 4 * 3;
			case ShaderDataType::INT_4:		    return 4 * 4;
			case ShaderDataType::BOOL:		    return 1;
			default: break;
		}

		NE_ENGINE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	// One attribute inside a vertex, e.g. "POSITION" as 3 floats.
	// 'Name' doubles as the HLSL semantic name in D3D (and the attribute name in OpenGL).
	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		bool Normalized;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) : Name(name), Type(type), Offset(0), Size(ShaderDataTypeSize(type)), Normalized(normalized)
		{

		}

		uint32_t Count() const
		{
			switch (Type)
			{
				case ShaderDataType::FLOAT:		    return 1;
				case ShaderDataType::POS_2D:		return 2;
				case ShaderDataType::POS_3D:		return 3;
				case ShaderDataType::FLOAT_4:		return 4;
				case ShaderDataType::MAT_3:		    return 3 * 3;
				case ShaderDataType::MAT_4:		    return 4 * 4;
				case ShaderDataType::INT:		    return 1;
				case ShaderDataType::INT_2:		    return 2;
				case ShaderDataType::INT_3:		    return 3;
				case ShaderDataType::INT_4:		    return 4;
				case ShaderDataType::BOOL:		    return 1;
				default: break;
			}

			NE_ENGINE_ASSERT(false, "Unknown ShaderDataType");
			return 0;
		}
	};

	// Describes the full vertex: an ordered list of elements, with offsets and total size (stride)
	// worked out automatically.
	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		};

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; };
		inline uint32_t GetStride() const { return m_Size; };

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_Size = 0;

			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Size += element.Size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Size = 0;
	};
}
