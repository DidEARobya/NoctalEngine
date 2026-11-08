#pragma once
#include <string>

namespace NoctalEngine
{
	enum class ShaderStage
	{
		VERTEX,
		PIXEL	// a.k.a. "fragment" shader in OpenGL
	};

	// A single compiled shader stage (vertex OR pixel), loaded from a file.
	//
	// Shaders are not bound directly any more. Modern APIs bake the shaders together with
	// blend state, vertex layout, etc. into one "Pipeline" object (see Pipeline.h), and that is
	// what gets bound. So a Shader is just the compiled code waiting to be put into a Pipeline.
	//
	// Uniforms (SetUniformFloat4 etc.) are gone too: per-object data now travels inside the
	// vertices (so thousands of objects can share one draw call) and per-frame data travels
	// in a ConstantBuffer.
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual ShaderStage GetStage() const = 0;
		virtual const std::string& GetName() const = 0;
		virtual bool IsValid() const = 0;
	};
}
