// Batch2D vertex shader.
// Vertices arrive already in world space (the CPU transformed them while batching),
// so all that's left is applying the camera.

cbuffer Camera : register(b0)
{
	// glm and HLSL both default to column-major matrices, so the matrix can be uploaded
	// as-is and used as mul(matrix, vector), just like glm's matrix * vector.
	float4x4 u_ViewProjection;
};

struct VSInput
{
	float3 Position : POSITION;
	float4 Colour   : COLOR;
	float2 TexCoord : TEXCOORD;
	int    TexIndex : TEXINDEX;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float4 Colour   : COLOR;
	float2 TexCoord : TEXCOORD;
	// Integers can't be blended across a triangle, and every vertex of a quad has the same
	// value anyway, so tell the GPU not to interpolate it.
	nointerpolation int TexIndex : TEXINDEX;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	output.Position = mul(u_ViewProjection, float4(input.Position, 1.0f));
	output.Colour = input.Colour;
	output.TexCoord = input.TexCoord;
	output.TexIndex = input.TexIndex;
	return output;
}
