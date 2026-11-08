#pragma once
#include "NoctalEngine/Rendering/Pipeline/Pipeline.h"
#include "NoctalEngine/Rendering/D3D12/D3D12Common.h"

// A D3D12 Pipeline State Object (PSO): shaders, vertex layout, blending, rasteriser and
// render target format, all validated and compiled together up front. Binding it is then
// a single cheap call, which is a big part of why D3D12 is fast.
class D3D12Pipeline : public NoctalEngine::Pipeline
{
public:
	D3D12Pipeline(const NoctalEngine::PipelineSpecification& specification);
	virtual ~D3D12Pipeline() override;

	virtual void Bind() override;

	virtual const NoctalEngine::PipelineSpecification& GetSpecification() const override { return m_Specification; }
	virtual bool IsValid() const override { return m_PipelineState != nullptr; }

private:
	static DXGI_FORMAT ShaderDataTypeToDXGIFormat(NoctalEngine::ShaderDataType type);

private:
	NoctalEngine::PipelineSpecification m_Specification;
	ComPtr<ID3D12PipelineState> m_PipelineState;
};
