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
			TRANSFORM
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
			static constexpr const char* semantic = "a_Position";
		};

		template<> struct Map<TEXCOORD>
		{
			using SysType = glm::vec2;
			static constexpr const char* semantic = "a_TexCoord";
		};
		template<> struct Map<TRANSFORM>
		{
			using SysType = glm::mat4;
			static constexpr const char* semantic = "a_Transform";
		};
		class Element
		{
		public:
			Element(ElementType type, size_t offset)
				: type(type), offset(offset) {
			}

			size_t GetOffsetAfter() const { return offset + Size(); }
			size_t GetOffset() const { return offset; }

			static constexpr size_t SizeOf(ElementType type);

			size_t Size() const
			{
				switch (type)
				{
				case POS_2D:	return sizeof(glm::vec2);
				case POS_3D:	return sizeof(glm::vec3);
				case TEXCOORD:  return sizeof(glm::vec2);
				case TRANSFORM: return sizeof(glm::mat4);
				default: return 0;
				}
			}

			uint32_t Count() const
			{
				switch (type)
				{
				case POS_2D:	return 2;
				case POS_3D:	return 3;
				case TEXCOORD:  return 2;
				case TRANSFORM: return 16;
				default: return 0;
				}
			}

			ElementType GetType() const { return type; }

		private:
			ElementType type;
			size_t offset;
		};

	public:
		VertexLayout& Append(ElementType type)
		{
			size_t offset = elements.empty() ? 0 : elements.back().GetOffsetAfter();
			elements.emplace_back(type, offset);
			return *this;
		}

		template<ElementType Type>
		const Element& Resolve() const
		{
			for (auto& e : elements)
				if (e.GetType() == Type) return e;
			assert(false && "Could not resolve element type");
			return elements.front();
		}

		const Element& ResolveByIndex(size_t i) const { return elements[i]; }
		size_t Size() const { return elements.empty() ? 0 : elements.back().GetOffsetAfter(); }
		size_t GetElementCount() const noexcept { return elements.size(); }

	public:
		std::vector<Element> elements;
	};

	class Vertex
	{
		friend class VertexBufferData;
	public:
		template<VertexLayout::ElementType Type>
		auto& Attr()
		{
			auto pAttr = pData + layout.Resolve<Type>().GetOffset();
			return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttr);
		}

		template<typename T>
		void SetAttributeByIndex(size_t i, T&& val)
		{
			const auto& element = layout.ResolveByIndex(i);
			auto pAttribute = pData + element.GetOffset();
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
			case VertexLayout::TRANSFORM:
				SetAttribute<VertexLayout::TRANSFORM>(pAttribute, std::forward<T>(val));
				break;
			default:
				assert("Bad element type" && false);
			}
		}

	public:
		Vertex(char* pData, const VertexLayout& layout) : pData(pData), layout(layout) {}

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
				assert("Parameter attribute type mismatch" && false);
			}
		}
	private:
		char* pData = nullptr;
		const VertexLayout& layout;
	};


	class ConstVertex
	{
	public:
		ConstVertex(const Vertex& v) : vertex(v) {}

		template<VertexLayout::ElementType Type>
		const auto& Attr() const { return const_cast<Vertex&>(vertex).Attr<Type>(); }

	private:
		Vertex vertex;
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
			assert(sizeof...(params) == layout.GetElementCount() && "Param count doesn't match number of vertex elements");
			buffer.resize(buffer.size() + layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}

		Vertex Back();
		Vertex Front();
		Vertex operator[](size_t i);
		ConstVertex Back() const;
		ConstVertex Front() const;
		ConstVertex operator[](size_t i) const;
	private:
		std::vector<char> buffer;
		VertexLayout layout;
	};
}