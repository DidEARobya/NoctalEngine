#pragma once
#include "NoctalEngine/Rendering/Bindable.h"
#include <glm/glm.hpp>

namespace NoctalEngine
{
	class Shader;
	class Texture;

	class Material : public Bindable
	{
	public:
		Material();

		virtual ~Material() override = default;
		virtual void Bind() override;

		void BindTexture(std::shared_ptr<Texture> texture);
		void BindShader(std::unique_ptr<Shader> shader);

	public:
		struct MaterialData
		{
			glm::vec4 Colour;

		} Uniforms;
	private:
		std::vector<std::shared_ptr<Texture>> m_Textures;
		std::unique_ptr<Shader> m_Shader;
	};
}