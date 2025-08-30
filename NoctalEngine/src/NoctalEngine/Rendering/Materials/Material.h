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

		void SetBaseTexture(std::shared_ptr<Texture> texture);
		//Setup doesn't work, come back when creating MaterialUniformBufferObject
		//void BindSubTexture(std::shared_ptr<Texture> texture);

		void SetShader(std::unique_ptr<Shader> shader);

		void SetColour(glm::vec4 colour);
		const glm::vec4 GetColour() const { return m_Uniforms.Colour; };

	private:
		struct MaterialData
		{
			glm::vec4 Colour;

		} m_Uniforms;

		std::shared_ptr<Texture> m_BaseTexture;
		//std::vector<std::shared_ptr<Texture>> m_SubTextures;
		std::unique_ptr<Shader> m_Shader;
	};
}