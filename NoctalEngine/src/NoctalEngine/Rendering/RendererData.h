#pragma once
#include "NoctalEngine.h"
#include "NoctalEngine/Rendering/Buffers/VertexArray.h"
#include "NoctalEngine/Rendering/Geometry/Geometry.h"

struct RendererData
{
public:
	static constexpr uint32_t MaxQuads = 500;
	static constexpr uint32_t MaxVerts = MaxQuads * 4;
	static constexpr uint32_t MaxIndices = MaxQuads * 6;

	RendererData() {};

	static RendererData& Instance() { return s_Instance; };

	static void AddVertexArray(NoctalEngine::Geometry geometry, std::unique_ptr<NoctalEngine::VertexArray> vertexArray)
	{
		NE_ENGINE_ASSERT(!s_Instance.m_VertexArrays.contains(geometry), "VertexArray already exists");
		NE_ENGINE_ASSERT(vertexArray, "Tried to initalise with invalid data");

		s_Instance.m_VertexArrays.emplace(geometry, std::move(vertexArray));
	}

	static NoctalEngine::VertexArray* GetVertexArray(NoctalEngine::Geometry geometry)
	{
		NE_ENGINE_ASSERT(s_Instance.m_VertexArrays.contains(geometry), "VertexArray doesn't exist for geometry");
		return s_Instance.m_VertexArrays[geometry].get();
	}

	static void AddToIndexCount(uint32_t count)
	{
		//NE_ENGINE_INFO(count);
		s_Instance.m_IndexCount += count;

		if (s_Instance.m_IndexCount >= MaxIndices)
		{
			Flush();
			StartBatch();
		}
	}

	static uint32_t GetIndexCount()
	{
		//NE_ENGINE_INFO(s_Instance.m_IndexCount);
		return s_Instance.m_IndexCount;
	}

	static void StartBatch()
	{
		s_Instance.m_IndexCount = 0;

		for (auto& array : s_Instance.m_VertexArrays)
		{
			array.second->StartBatch();
		}
	}

	static void Flush()
	{
		for (auto& array : s_Instance.m_VertexArrays)
		{
			array.second->Flush();
		}

		StartBatch();
	}
	//inline uint8_t BytesPerQuad() const { return 4 * QuadVertexSize; }


private:
	static RendererData s_Instance;
	std::unordered_map<NoctalEngine::Geometry, std::unique_ptr<NoctalEngine::VertexArray>> m_VertexArrays;
	uint32_t m_IndexCount = 0;
};


