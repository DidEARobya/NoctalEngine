#pragma once
#include <glm/glm.hpp>

namespace NoctalEngine
{
	class VertexLayout
	{
	public:
		enum ElementType
		{
			POS_2D,
			POS_3D,
			TEXCOORD,
		};

		template<ElementType> struct Map;

		template<> struct Map<POS_2D>
		{
			using SysType = glm::vec2;
			static constexpr const char* semantic = "a_Position";
		};

		template<> struct Map<POS_3D>
		{
			using SysType = glm::vec3;
			static constexpr const char* semantic = "Position";
		};

		template<> struct Map<TEXCOORD>
		{
			using SysType = glm::vec2;
			static constexpr const char* semantic = "a_TexCoord";
		};

		class Element
		{
		public:
			Element(ElementType type, size_t offset) : m_Type(type), m_Offset(offset) {}

			size_t GetOffsetAfter() const { return m_Offset + Size(); }
			size_t GetOffset() const { return m_Offset; }

			static constexpr size_t SizeOf(ElementType type);

			size_t Size() const
			{
				switch (m_Type)
				{
					case POS_2D:	
						return sizeof(glm::vec2);
					case POS_3D:	
						return sizeof(glm::vec3);
					case TEXCOORD:  
						return sizeof(glm::vec2);
					default: 
						return 0;
				}
			}

			uint32_t Count() const
			{
				switch (m_Type)
				{
					case POS_2D:	
						return 2;
					case POS_3D:	
						return 3;
					case TEXCOORD:  
						return 2;
					default: 
						return 0;
				}
			}

			ElementType GetType() const { return m_Type; }

		private:
			ElementType m_Type;
			size_t m_Offset;
		};

	public:
		VertexLayout& Append(ElementType type)
		{
			size_t offset = m_Elements.empty() ? 0 : m_Elements.back().GetOffsetAfter();
			m_Elements.emplace_back(type, offset);
			return *this;
		}

		template<ElementType Type>
		const Element& Resolve() const
		{
			for (auto& e : m_Elements)
			{
				if (e.GetType() == Type)
				{
					return e;
				}
			}
				
			NE_ENGINE_ASSERT(false, "Could not resolve element type");
			return m_Elements.front();
		}

		const Element& ResolveByIndex(size_t i) const { return m_Elements[i]; }
		size_t Size() const { return m_Elements.empty() ? 0 : m_Elements.back().GetOffsetAfter(); }
		size_t GetElementCount() const noexcept { return m_Elements.size(); }

		const std::vector<Element>& GetElements() const { return m_Elements; };

	private:
		std::vector<Element> m_Elements;
	};

	class Vertex
	{
		friend class VertexBufferData;
	public:
		template<VertexLayout::ElementType Type>
		auto& Attr()
		{
			auto pAttr = m_Data + m_Layout.Resolve<Type>().GetOffset();
			return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttr);
		}

		template<typename T>
		void SetAttributeByIndex(size_t i, T&& val)
		{
			const auto& element = m_Layout.ResolveByIndex(i);
			auto pAttribute = m_Data + element.GetOffset();

			switch (element.GetType())
			{
				case VertexLayout::POS_2D:
					SetAttribute<VertexLayout::POS_2D>(pAttribute, std::forward<T>(val));
					break;
				case VertexLayout::POS_3D:
					SetAttribute<VertexLayout::POS_3D>(pAttribute, std::forward<T>(val));
					break;
				case VertexLayout::TEXCOORD:
					SetAttribute<VertexLayout::TEXCOORD>(pAttribute, std::forward<T>(val));
					break;
				default:
					NE_ENGINE_ASSERT(false, "Bad element type");
			}
		}

	protected:
		Vertex(char* pData, const VertexLayout& layout) : m_Data(pData), m_Layout(layout) {}

	private:
		// enables parameter pack setting of multiple parameters by element index
		template<typename First, typename ...Rest>
		void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest)
		{
			SetAttributeByIndex(i, std::forward<First>(first));
			SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
		}
		// helper to reduce code duplication in SetAttributeByIndex
		template<VertexLayout::ElementType DestLayoutType, typename SrcType>
		void SetAttribute(char* pAttribute, SrcType&& val)
		{
			using Dest = typename VertexLayout::Map<DestLayoutType>::SysType;
			if constexpr (std::is_assignable<Dest, SrcType>::value)
			{
				*reinterpret_cast<Dest*>(pAttribute) = val;
			}
			else
			{
				NE_ENGINE_ASSERT(false, "Parameter attribute type mismatch");
			}
		}
	private:
		char* m_Data = nullptr;
		const VertexLayout& m_Layout;
	};

	class ConstVertex
	{
	public:
		ConstVertex(const Vertex& v) : m_Vertex(v) {}

		template<VertexLayout::ElementType Type>
		const auto& Attr() const { return const_cast<Vertex&>(m_Vertex).Attr<Type>(); }

	private:
		Vertex m_Vertex;
	};

	class VertexBufferData
	{
	public:
		VertexBufferData(VertexLayout layout);
		const char* GetData() const;
		const VertexLayout& GetLayout() const noexcept;
		size_t Size() const;
		size_t SizeBytes() const;

		template<typename ...Params>
		void EmplaceBack(Params&&... params)
		{
			NE_ENGINE_ASSERT(sizeof...(params) == m_Layout.GetElementCount(), "Param count doesn't match number of vertex elements");
			m_Buffer.resize(m_Buffer.size() + m_Layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}

		Vertex Back();
		Vertex Front();
		Vertex operator[](size_t i);
		ConstVertex Back() const;
		ConstVertex Front() const;
		ConstVertex operator[](size_t i) const;
	private:
		std::vector<char> m_Buffer;
		VertexLayout m_Layout;
	};
}