// Batch2D pixel shader.
// Every quad in a batch can use a different texture: the vertex tells us which slot.

#ifndef MAX_TEXTURE_SLOTS
	#define MAX_TEXTURE_SLOTS 32	// Normally passed in from RendererConfig.h by D3D12Shader.cpp
#endif

Texture2D g_Textures[MAX_TEXTURE_SLOTS] : register(t0);
SamplerState g_Sampler : register(s0);

struct PSInput
{
	float4 Position : SV_POSITION;
	float4 Colour   : COLOR;
	float2 TexCoord : TEXCOORD;
	nointerpolation int TexIndex : TEXINDEX;
};

float4 main(PSInput input) : SV_TARGET
{
	// NonUniformResourceIndex: neighbouring pixels may pick different textures (two quads side
	// by side), and without this hint some GPUs would sample the wrong one.
	float4 texel = g_Textures[NonUniformResourceIndex(input.TexIndex)].Sample(g_Sampler, input.TexCoord);

	// Untextured quads use slot 0, a white texture, so this is just the colour.
	return texel * input.Colour;
}
