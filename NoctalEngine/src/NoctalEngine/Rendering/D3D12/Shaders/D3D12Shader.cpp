#include "nepch.h"
#include "D3D12Shader.h"
#include "NoctalEngine/Rendering/RendererConfig.h"
#include <d3dcompiler.h>
#include <filesystem>

D3D12Shader::D3D12Shader(const std::string& filePath)
{
	const std::filesystem::path path(filePath);
	m_Name = path.stem().string();

	// ---- Work out the stage from the name ----
	const char* target = nullptr;

	if (m_Name.ends_with("VS"))
	{
		m_Stage = NoctalEngine::ShaderStage::VERTEX;
		target = "vs_5_1";	// Shader Model 5.1: the first with dynamically indexed texture arrays.
	}
	else if (m_Name.ends_with("PS"))
	{
		m_Stage = NoctalEngine::ShaderStage::PIXEL;
		target = "ps_5_1";
	}
	else
	{
		NE_ENGINE_ERROR("Shader '{}' must end in VS or PS so its stage is known", m_Name);
		return;
	}

	// ---- Compile ----
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Debug info + no optimisation: lets tools like PIX/RenderDoc step through the shader.
	flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

	// Pass the C++ texture slot count into the shader, so the two can never disagree.
	const std::string slotCount = std::to_string(NoctalEngine::RendererConfig::MaxTextureSlots);
	const D3D_SHADER_MACRO defines[] =
	{
		{ "MAX_TEXTURE_SLOTS", slotCount.c_str() },
		{ nullptr, nullptr }
	};

	ComPtr<ID3DBlob> errors;
	const HRESULT hr = D3DCompileFromFile(path.wstring().c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", target, flags, 0, &m_Bytecode, &errors);

	if (FAILED(hr))
	{
		NE_ENGINE_ERROR("Failed to compile shader '{}':\n{}", filePath, errors ? (const char*)errors->GetBufferPointer() : "file not found?");
		m_Bytecode.Reset();
		return;
	}

	// Warnings come back in 'errors' even on success.
	if (errors)
	{
		NE_ENGINE_WARN("Shader '{}' compiled with warnings:\n{}", m_Name, (const char*)errors->GetBufferPointer());
	}

	NE_ENGINE_INFO("Compiled shader {}", m_Name);
}

D3D12_SHADER_BYTECODE D3D12Shader::GetBytecode() const
{
	D3D12_SHADER_BYTECODE bytecode = {};

	if (m_Bytecode)
	{
		bytecode.pShaderBytecode = m_Bytecode->GetBufferPointer();
		bytecode.BytecodeLength = m_Bytecode->GetBufferSize();
	}

	return bytecode;
}
