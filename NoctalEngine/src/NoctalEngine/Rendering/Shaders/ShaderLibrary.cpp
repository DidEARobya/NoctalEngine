#include "nepch.h"
#include "ShaderLibrary.h"
#include "Shader.h"
#include "NoctalEngine/Rendering/Renderer.h"
#include <filesystem>

namespace NoctalEngine
{
	std::shared_ptr<Shader> ShaderLibrary::LoadShader(const std::string& filePath, bool sortAfterLoad)
	{
		std::filesystem::path path(filePath);
		std::string name = path.stem().string();

		if (Contains(name) == true)
		{
			return GetShader(name);
		}

		std::shared_ptr<Shader> shader = Renderer::Instance().CreateShader(filePath);

		if (shader->IsValid() == false)
		{
			NE_ENGINE_ERROR("ShaderLibrary failed to load: {}", filePath);
			return nullptr;
		}

		m_Shaders.push_back(std::make_pair(name, shader));

		if (sortAfterLoad == true)
		{
			SortShaders();
		}

		return shader;
	}
	std::shared_ptr<Shader> ShaderLibrary::LoadShader(const std::string& name, const std::string filePath, bool sortAfterLoad)
	{
		if (Contains(name) == true)
		{
			return GetShader(name);
		}

		std::filesystem::path path(filePath);
		std::string fileName = path.stem().string();

		if (Contains(fileName) == true)
		{
			auto it = std::lower_bound(m_Shaders.begin(), m_Shaders.end(), fileName,
			[](const auto& pair, std::string_view key)
			{
				return pair.first < key;
			});

			if (it != m_Shaders.end() && it->first == fileName)
			{
				it->first = name;

				SortShaders();

				return it->second;
			}

			NE_ENGINE_ASSERT(false, "SharedLibrary contains key with fileName, but the shader was not found");
			return nullptr;
		}

		std::shared_ptr<Shader> shader = Renderer::Instance().CreateShader(filePath);

		if (shader->IsValid() == false)
		{
			NE_ENGINE_ERROR("ShaderLibrary failed to load: {}", filePath);
			return nullptr;
		}

		m_Shaders.push_back(std::make_pair(name, shader));

		if (sortAfterLoad == true)
		{
			SortShaders();
		}

		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::GetShader(const std::string& name)
	{
		auto it = std::lower_bound(m_Shaders.begin(), m_Shaders.end(), name,
		[](const auto& pair, std::string_view key)
		{
			return pair.first < key;
		});

		if (it != m_Shaders.end() && it->first == name)
		{
			return it->second;
		}

		NE_ENGINE_ERROR("ShaderLibrary failed to find: {}", name);
		return nullptr;
	}

	void ShaderLibrary::SortShaders()
	{
		std::sort(m_Shaders.begin(), m_Shaders.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
	}
	bool ShaderLibrary::Contains(std::string_view name)
	{
		auto it = std::lower_bound(m_Shaders.begin(), m_Shaders.end(), name,
		[](const auto& pair, std::string_view key) 
		{
			return pair.first < key;
		});

		return it != m_Shaders.end() && it->first == name;
	}
}