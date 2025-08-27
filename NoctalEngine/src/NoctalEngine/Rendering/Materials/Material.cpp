#pragma once
#include "Material.h"
#include "NoctalEngine/Rendering/Shaders/Shader.h"

namespace NoctalEngine
{
	Material::Material() : Uniforms({ glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) })
	{
	}
	void Material::Bind()
	{
		if (m_Shader == nullptr)
		{
			return;
		}

		m_Shader->Bind();
		m_Shader->SetUniformFloat4("u_Colour", Uniforms.Colour);
	}

	void Material::BindShader(std::unique_ptr<Shader> bind)
	{
		NE_ENGINE_ASSERT(m_Shader == nullptr, "Attempted to add Shader a second time");
		m_Shader = std::move(bind);
	}
}