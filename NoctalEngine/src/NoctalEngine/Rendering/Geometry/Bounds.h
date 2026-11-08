#pragma once
#include <glm/glm.hpp>

namespace NoctalEngine
{
	// Axis-Aligned Bounding Box in 2D world space.
	// Used for culling: if an object's box doesn't overlap the camera's box,
	// it can't be on screen, so we skip it entirely (no vertices, no draw cost).
	struct AABB2D
	{
		glm::vec2 Min = glm::vec2(0.0f);
		glm::vec2 Max = glm::vec2(0.0f);

		bool Overlaps(const AABB2D& other) const
		{
			// Two boxes overlap unless one is completely to one side of the other.
			return Min.x <= other.Max.x && Max.x >= other.Min.x &&
				   Min.y <= other.Max.y && Max.y >= other.Min.y;
		}

		// Builds a box that tightly fits a set of points.
		template<typename PointArray>
		static AABB2D FromPoints(const PointArray& points)
		{
			AABB2D box;
			box.Min = glm::vec2(points[0]);
			box.Max = glm::vec2(points[0]);

			for (const auto& p : points)
			{
				box.Min = glm::min(box.Min, glm::vec2(p));
				box.Max = glm::max(box.Max, glm::vec2(p));
			}

			return box;
		}
	};
}
