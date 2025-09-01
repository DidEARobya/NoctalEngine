#pragma once;

struct RendererData
{
   const uint32_t MaxQuads = 10000;
   const uint32_t MaxVerts = MaxQuads * 4;
   const uint32_t MaxIndices = MaxQuads * 6;

   uint32_t IndicesCount = 0;

   uint8_t* QuadVertexBufferBase = nullptr;
   uint8_t* QuadVertexBufferPtr = nullptr;
   uint32_t QuadVertexSize = 0;

   static RendererData& Instance() { return s_Instance; };
   inline uint8_t BytesPerQuad() const { return 4 * QuadVertexSize; }

private:
	static RendererData s_Instance;
};


