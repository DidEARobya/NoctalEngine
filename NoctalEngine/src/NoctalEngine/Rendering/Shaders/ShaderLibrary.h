#pragma once
#include "Shader.h"

namespace NoctalEngine
{
	class ShaderLibrary
	{
	public:
		std::shared_ptr<Shader> LoadShader(const std::string& filepath, bool sortAfterLoad = true);
		std::shared_ptr<Shader> LoadShader(const std::string& name, const std::string filePath, bool sortAfterLoad = true);

		std::shared_ptr<Shader> GetShader(const std::string& name);

		void SortShaders();
		
	private:
		bool Contains(std::string_view name);

	private:
		std::vector<std::pair<std::string, std::shared_ptr<Shader>>> m_Shaders;
	};
}