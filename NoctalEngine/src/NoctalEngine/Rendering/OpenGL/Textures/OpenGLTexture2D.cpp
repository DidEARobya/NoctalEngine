#include "nepch.h"
#include "OpenGLTexture2D.h"
#include <GLAD/glad.h>
#include <stb_image/stb_image.h>

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path)
{
	int width;
	int height;
	int channels;

	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	NE_ENGINE_ASSERT(data, "Failed to load {} - Reason: {}", path, stbi_failure_reason());

	m_Width = width;
	m_Height = height;

	GLenum glFormat = 0;
	GLenum fileFormat = 0;

	if (channels == 4)
	{
		glFormat = GL_RGBA8;
		fileFormat = GL_RGBA;
	}
	else if (channels == 3)
	{
		glFormat = GL_RGB8;
		fileFormat = GL_RGB;
	}

	NE_ENGINE_ASSERT(glFormat & fileFormat, "Texture Format not Supported");

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, glFormat, m_Width, m_Height);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, fileFormat, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &m_RendererID);
}

void OpenGLTexture2D::Bind()
{
	glBindTextureUnit(m_Index, m_RendererID);
}
