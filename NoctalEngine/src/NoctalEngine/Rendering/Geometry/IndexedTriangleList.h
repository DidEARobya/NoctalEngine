#pragma once
#include <vector>
#include <glm/glm.hpp>

template<class T>
class IndexedTriangleList
{
public:
	IndexedTriangleList() = default;
	IndexedTriangleList(std::vector<T> vertices, std::vector<uint32_t> indices) : m_Vertices(std::move(vertices)), m_Indices(std::move(indices))
	{
		NE_ENGINE_ASSERT(m_Vertices.size() > 2, "Invalid number of vertices");
		NE_ENGINE_ASSERT((m_Indices.size() % 3) == 0, "Invalid number of indices");
	}

	void Transform(glm::mat4 matrix)
	{
		for (auto& v : m_Vertices)
		{
			const glm::vec4 pos = glm::vec4(v.Pos, 1.0f);
			pos = matrix * pos;
			v.Pos = glm::vec3(pos);
		}
	}

	void SetNormalsIndependentFlat()
	{
		for (size_t i = 0; i < m_Indices.size(); i += 3)
		{
			auto& v0 = m_Vertices[m_Indices[i]];
			auto& v1 = m_Vertices[m_Indices[i + 1]];
			auto& v2 = m_Vertices[m_Indices[i + 2]];

			const glm::vec3& p0 = v0.Pos;
			const glm::vec3& p1 = v1.Pos;
			const glm::vec3& p2 = v2.Pos;

			glm::vec3 n = glm::normalize(glm::cross(p1 - p0, p2 - p0));

			v0.n = n;
			v1.n = n;
			v2.n = n;
		}
	}
public:
	std::vector<T> m_Vertices;
	std::vector<uint32_t> m_Indices;
};