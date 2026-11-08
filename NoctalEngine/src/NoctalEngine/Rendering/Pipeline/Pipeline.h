#pragma once
#include "NoctalEngine/Rendering/Bindable.h"
#include "NoctalEngine/Rendering/Buffers/BufferLayout.h"

namespace NoctalEngine
{
	class Shader;

	// Everything the GPU needs to know about "how to draw", gathered in one place.
	struct PipelineSpecification
	{
		std::shared_ptr<Shader> VertexShader;
		std::shared_ptr<Shader> PixelShader;

		// Must match the vertex struct the shader expects.
		BufferLayout Layout;

		// Standard "over" blending: result = src * srcAlpha + dst * (1 - srcAlpha).
		bool AlphaBlending = true;

		std::string DebugName = "Pipeline";
	};

	// A Pipeline is the compiled, ready-to-use combination of shaders + vertex layout + render states.
	// - D3D12: a Pipeline State Object (PSO) plus root signature.
	// - OpenGL: would be a linked shader program plus a vertex array object and glEnable/glBlendFunc calls.
	// Building one is expensive, so do it once at start-up; binding one is cheap.
	class Pipeline : public Bindable
	{
	public:
		virtual ~Pipeline() override = default;

		virtual const PipelineSpecification& GetSpecification() const = 0;
		virtual bool IsValid() const = 0;
	};
}
