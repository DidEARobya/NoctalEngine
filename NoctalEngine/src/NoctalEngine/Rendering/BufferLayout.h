#pragma once

namespace NoctalEngine
{
	enum class ShaderDataType
	{
		NONE = 0,
		FLOAT, 
		FLOAT_2,
		FLOAT_3,
		FLOAT_4,
		MAT_3, 
		MAT_4,
		INT,
		INT_2,
		INT_3,
		INT_4,
		BOOL
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::FLOAT:		    return 4;
			case ShaderDataType::FLOAT_2:		return 4 * 2;
			case ShaderDataType::FLOAT_3:		return 4 * 3;
			case ShaderDataType::FLOAT_4:		return 4 * 4;
			case ShaderDataType::MAT_3:		    return 4 * 3 * 3;
			case ShaderDataType::MAT_4:		    return 4 * 4 * 4;
			case ShaderDataType::INT:		    return 4;
			case ShaderDataType::INT_2:		    return 4 * 2;
			case ShaderDataType::INT_3:		    return 4 * 3;
			case ShaderDataType::INT_4:		    return 4 * 4;
			case ShaderDataType::BOOL:		    return 1;
		}

		NE_ENGINE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		bool Normalized;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{

		}

		uint32_t Count() const
		{
			switch (Type)
			{
				case ShaderDataType::FLOAT:		    return 1;
				case ShaderDataType::FLOAT_2:		return 2;
				case ShaderDataType::FLOAT_3:		return 3;
				case ShaderDataType::FLOAT_4:		return 4;
				case ShaderDataType::MAT_3:		    return 3 * 3;
				case ShaderDataType::MAT_4:		    return 4 * 4;
				case ShaderDataType::INT:		    return 1;
				case ShaderDataType::INT_2:		    return 2;
				case ShaderDataType::INT_3:		    return 3;
				case ShaderDataType::INT_4:		    return 4;
				case ShaderDataType::BOOL:		    return 1;
			}

			NE_ENGINE_ASSERT(false, "Unknown ShaderDataType");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements) 
		{
			CalculateOffsetsAndStride();
		};

		inline const std::vector<BufferElement>& GetElements() { return m_Elements; };
		inline uint32_t GetStride() const { return m_Stride; };

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;

			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;
	};
}