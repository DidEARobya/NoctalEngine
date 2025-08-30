#include "nepch.h"
#include "Material.h"
#include "NoctalEngine/Rendering/Shaders/Shader.h"
#include "NoctalEngine/Rendering/Textures/Texture.h"

namespace NoctalEngine
{
	Material::Material() : m_Uniforms({ 0.0f, 1.0f, 1.0f, 1.0f })
	{
	}
	void Material::Bind()
	{
		if (m_Shader == nullptr)
		{
			return;
		}

		m_Shader->Bind();

		uint32_t index = 0;
		for (const auto& texture : m_Textures)
		{
			texture->SetTextureIndex(index);
			texture->Bind();
			index++;
		}
	}

	void Material::BindTexture(std::shared_ptr<Texture> texture)
	{
		NE_ENGINE_ASSERT(m_Shader || texture, "Bind Shader before binding textures");
		m_Textures.push_back(texture);

		m_Shader->Bind();
		m_Shader->SetUniformInt("u_Texture", 0);
		m_Shader->SetUniformInt("u_UseTexture", 1);
	}

	void Material::BindShader(std::unique_ptr<Shader> shader)
	{
		NE_ENGINE_ASSERT(m_Shader == nullptr, "Attempted to add Shader a second time");
		m_Shader = std::move(shader);
		m_Shader->Bind();
		m_Shader->SetUniformFloat4("u_Colour", m_Uniforms.Colour);
	}
	void Material::SetColour(glm::vec4 colour)
	{
		if (m_Uniforms.Colour == colour)
		{
			return;
		}

		m_Shader->Bind();

		m_Uniforms.Colour = colour;
		m_Shader->SetUniformFloat4("u_Colour", m_Uniforms.Colour);
	}
}