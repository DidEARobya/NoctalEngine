#pragma once
#include "NoctalEngine/Rendering/Bindable.h"

namespace NoctalEngine
{
	// Small block of data shared by every vertex/pixel in a draw (e.g. the camera matrix).
	// D3D calls these "constant buffers", OpenGL calls them "uniform buffers".
	class ConstantBuffer : public Bindable
	{
	public:
		virtual ~ConstantBuffer() override = default;

		// Copy new data in. Like VertexBuffer, it's safe to call this more than once per frame.
		virtual void SetData(const void* data, uint32_t size) = 0;
	};
}
