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

		if (m_BaseTexture != nullptr)
		{
			m_BaseTexture->SetTextureIndex(0);
			m_BaseTexture->Bind();
		}

		//uint32_t index = 1;

		//for (const auto& texture : m_SubTextures)
		//{
		//	texture->SetTextureIndex(index);
		//	texture->Bind();
		//	index++;
		//}
	}

	void Material::SetBaseTexture(std::shared_ptr<Texture> texture)
	{
		NE_ENGINE_ASSERT(m_Shader, "Bind Shader before binding textures");

		if (texture == nullptr)
		{
			NE_ENGINE_ERROR("Tried to bind invalid Texture");
			return;
		}

		m_BaseTexture.reset();
		m_BaseTexture = texture;

		m_Shader->Bind();
		m_Shader->SetUniformInt("u_Texture", 0);
	}

	//Setup doesn't work, come back when creating MaterialUniformBufferObject
	//void Material::BindSubTexture(std::shared_ptr<Texture> texture)
	//{
	//	NE_ENGINE_ASSERT(m_Shader, "Bind Shader before binding textures");
	//	NE_ENGINE_ASSERT(texture, "Tried to bind invalid Texture");

	//	m_SubTextures.push_back(texture);

	//	m_Shader->Bind();
	//	m_Shader->SetUniformInt("u_Texture", 0);
	//	m_Shader->SetUniformInt("u_UseTexture", 1);
	//}

	void Material::SetShader(std::unique_ptr<Shader> shader)
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