#pragma once
#include "NoctalEngine/Rendering/Shaders/Shader.h"
#include "NoctalEngine/Rendering/D3D12/D3D12Common.h"

// One HLSL shader stage compiled at load time with D3DCompile.
//
// Naming convention (same as the old OpenGL shaders): the file name ends in VS or PS,
// e.g. "Batch2DVS.hlsl" is a vertex shader, "Batch2DPS.hlsl" a pixel shader.
// The entry point function must be called main().
class D3D12Shader : public NoctalEngine::Shader
{
public:
	D3D12Shader(const std::string& filePath);
	virtual ~D3D12Shader() override = default;

	virtual NoctalEngine::ShaderStage GetStage() const override { return m_Stage; }
	virtual const std::string& GetName() const override { return m_Name; }
	virtual bool IsValid() const override { return m_Bytecode != nullptr; }

	// The compiled shader, ready to go into a pipeline state object.
	D3D12_SHADER_BYTECODE GetBytecode() const;

private:
	std::string m_Name;
	NoctalEngine::ShaderStage m_Stage = NoctalEngine::ShaderStage::VERTEX;
	ComPtr<ID3DBlob> m_Bytecode;
};
