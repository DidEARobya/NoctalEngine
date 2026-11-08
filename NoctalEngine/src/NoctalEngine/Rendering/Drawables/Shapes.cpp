#include "nepch.h"
#include "Shapes.h"
#include "NoctalEngine/Rendering/Geometry/Quad.h"
#include "NoctalEngine/Rendering/Geometry/Triangle.h"

namespace
{
	// The vertex type the Geometry helpers fill in (they expect .Pos and .TexCoord).
	struct LocalVertex
	{
		glm::vec3 Pos;
		glm::vec2 TexCoord;
	};

	// Copies an IndexedTriangleList into the fixed 4-corner ShapeData the batcher uses.
	ShapeData ToShapeData(const IndexedTriangleList<LocalVertex>& list)
	{
		NE_ENGINE_ASSERT(list.m_Vertices.size() >= 3 && list.m_Vertices.size() <= 4, "Batched shapes must have 3 or 4 vertices");

		ShapeData shape{};

		for (size_t i = 0; i < 4; i++)
		{
			// If the shape only has 3 corners, repeat the last one (makes a zero-area 2nd triangle).
			const LocalVertex& v = list.m_Vertices[std::min(i, list.m_Vertices.size() - 1)];
			shape.Positions[i] = glm::vec2(v.Pos);
			shape.TexCoords[i] = v.TexCoord;
		}

		return shape;
	}
}

ShapeData QuadDrawable::BuildShape()
{
	// Corners: bottom-left, bottom-right, top-right, top-left.
	// That order matches the batch index pattern (0,1,2, 2,3,0).
	return ToShapeData(NoctalEngine::Quad::MakeTextured<LocalVertex>());
}

ShapeData TriangleDrawable::BuildShape()
{
	return ToShapeData(NoctalEngine::Triangle::MakeTextured<LocalVertex>());
}
