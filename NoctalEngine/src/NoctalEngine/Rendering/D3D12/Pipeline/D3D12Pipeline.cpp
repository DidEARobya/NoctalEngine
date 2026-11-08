#include "nepch.h"
#include "D3D12Pipeline.h"
#include "NoctalEngine/Rendering/D3D12/D3D12Renderer.h"
#include "NoctalEngine/Rendering/D3D12/Shaders/D3D12Shader.h"

D3D12Pipeline::D3D12Pipeline(const NoctalEngine::PipelineSpecification& specification) : m_Specification(specification)
{
	D3D12Renderer& renderer = D3D12Renderer::Get();

	// The shaders came from this backend, so they are D3D12Shaders underneath.
	auto* vertexShader = dynamic_cast<D3D12Shader*>(m_Specification.VertexShader.get());
	auto* pixelShader = dynamic_cast<D3D12Shader*>(m_Specification.PixelShader.get());

	if (vertexShader == nullptr || vertexShader->IsValid() == false || pixelShader == nullptr || pixelShader->IsValid() == false)
	{
		NE_ENGINE_ERROR("Pipeline '{}' needs a valid vertex and pixel shader", m_Specification.DebugName);
		return;
	}

	// ---- Input layout ----
	// Describes each vertex attribute to the input assembler. Built from the API-agnostic
	// BufferLayout. The semantic names point into m_Specification, which lives as long as we do.
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElements;

	for (const NoctalEngine::BufferElement& element : m_Specification.Layout)
	{
		D3D12_INPUT_ELEMENT_DESC desc = {};
		desc.SemanticName = element.Name.c_str();
		desc.SemanticIndex = 0;
		desc.Format = ShaderDataTypeToDXGIFormat(element.Type);
		desc.InputSlot = 0;
		desc.AlignedByteOffset = element.Offset;
		desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;
		inputElements.push_back(desc);
	}

	// ---- Blending ----
	D3D12_BLEND_DESC blend = {};
	blend.AlphaToCoverageEnable = FALSE;
	blend.IndependentBlendEnable = FALSE;

	D3D12_RENDER_TARGET_BLEND_DESC& target = blend.RenderTarget[0];
	target.BlendEnable = m_Specification.AlphaBlending ? TRUE : FALSE;
	target.LogicOpEnable = FALSE;
	target.SrcBlend = D3D12_BLEND_SRC_ALPHA;			// colour = src * srcAlpha
	target.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//        + dst * (1 - srcAlpha)
	target.BlendOp = D3D12_BLEND_OP_ADD;
	target.SrcBlendAlpha = D3D12_BLEND_ONE;
	target.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
	target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	target.LogicOp = D3D12_LOGIC_OP_NOOP;
	target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// ---- Rasteriser ----
	D3D12_RASTERIZER_DESC rasterizer = {};
	rasterizer.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizer.CullMode = D3D12_CULL_MODE_NONE;			// 2D: draw both sides, winding order doesn't matter.
	rasterizer.FrontCounterClockwise = FALSE;
	rasterizer.DepthBias = 0;
	rasterizer.DepthBiasClamp = 0.0f;
	rasterizer.SlopeScaledDepthBias = 0.0f;
	// glm::ortho produces OpenGL-style depth (-1..1) but D3D clips depth outside 0..1.
	// We don't use depth in 2D (layers handle ordering), so just turn depth clipping off.
	rasterizer.DepthClipEnable = FALSE;
	rasterizer.MultisampleEnable = FALSE;
	rasterizer.AntialiasedLineEnable = FALSE;
	rasterizer.ForcedSampleCount = 0;
	rasterizer.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	// ---- Depth/stencil: off. Draw order comes from sorting by layer. ----
	D3D12_DEPTH_STENCIL_DESC depthStencil = {};
	depthStencil.DepthEnable = FALSE;
	depthStencil.StencilEnable = FALSE;

	// ---- Put it all together ----
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	desc.pRootSignature = renderer.GetRootSignature();
	desc.VS = vertexShader->GetBytecode();
	desc.PS = pixelShader->GetBytecode();
	desc.BlendState = blend;
	desc.SampleMask = UINT_MAX;
	desc.RasterizerState = rasterizer;
	desc.DepthStencilState = depthStencil;
	desc.InputLayout = { inputElements.data(), (UINT)inputElements.size() };
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.NumRenderTargets = 1;
	desc.RTVFormats[0] = D3D12Config::BackBufferFormat;
	desc.DSVFormat = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc = { 1, 0 };

	NE_D3D_CHECK(renderer.GetDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_PipelineState)));

	if (m_PipelineState)
	{
		m_PipelineState->SetName(D3D12Helpers::ToWide(m_Specification.DebugName).c_str());
	}
}

D3D12Pipeline::~D3D12Pipeline()
{
	if (D3D12Renderer::IsActive())
	{
		D3D12Renderer::Get().DeferRelease(m_PipelineState);
	}
}

void D3D12Pipeline::Bind()
{
	ID3D12GraphicsCommandList* commandList = D3D12Renderer::Get().GetCommandList();

	commandList->SetGraphicsRootSignature(D3D12Renderer::Get().GetRootSignature());
	commandList->SetPipelineState(m_PipelineState.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

DXGI_FORMAT D3D12Pipeline::ShaderDataTypeToDXGIFormat(NoctalEngine::ShaderDataType type)
{
	switch (type)
	{
		case NoctalEngine::ShaderDataType::FLOAT:	return DXGI_FORMAT_R32_FLOAT;
		case NoctalEngine::ShaderDataType::POS_2D:	return DXGI_FORMAT_R32G32_FLOAT;
		case NoctalEngine::ShaderDataType::POS_3D:	return DXGI_FORMAT_R32G32B32_FLOAT;
		case NoctalEngine::ShaderDataType::FLOAT_4:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case NoctalEngine::ShaderDataType::INT:		return DXGI_FORMAT_R32_SINT;
		case NoctalEngine::ShaderDataType::INT_2:	return DXGI_FORMAT_R32G32_SINT;
		case NoctalEngine::ShaderDataType::INT_3:	return DXGI_FORMAT_R32G32B32_SINT;
		case NoctalEngine::ShaderDataType::INT_4:	return DXGI_FORMAT_R32G32B32A32_SINT;
		default: break;
	}

	NE_ENGINE_ASSERT(false, "ShaderDataType can't be used as a vertex attribute in D3D12");
	return DXGI_FORMAT_UNKNOWN;
}
