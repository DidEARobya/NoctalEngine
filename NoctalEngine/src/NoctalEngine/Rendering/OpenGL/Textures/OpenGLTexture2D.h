#pragma once
#include "NoctalEngine/Rendering/Textures/Texture2D.h"

class OpenGLTexture2D : public NoctalEngine::Texture2D
{
public:
	OpenGLTexture2D(const std::string& path);
	virtual ~OpenGLTexture2D() override;

	virtual void SetTextureIndex(int index) override { m_Index = index; };
	virtual void Bind() override;


	virtual uint32_t GetWidth() const override { return m_Width; };
	virtual uint32_t GetHeight() const override { return m_Height; };

private:
	std::string m_Path;
	uint32_t m_Width;
	uint32_t m_Height;
	uint32_t m_RendererID;
	uint32_t m_Index;
};