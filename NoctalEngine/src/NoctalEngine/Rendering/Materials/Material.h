#pragma once
#include "NoctalEngine/Rendering/Bindable.h"
#include <glm/glm.hpp>

namespace NoctalEngine
{
	class Shader;

	class Material : public Bindable
	{
	public:
		Material();

		virtual ~Material() override = default;
		virtual void Bind() override;

		virtual const NoctalEngine::BufferLayout* GetLayout() const { return nullptr; };

		void BindShader(std::unique_ptr<Shader> bind);

	public:
		struct MaterialData
		{
			glm::vec4 Colour;

		} Uniforms;
	private:
		std::unique_ptr<Shader> m_Shader;
	};
}