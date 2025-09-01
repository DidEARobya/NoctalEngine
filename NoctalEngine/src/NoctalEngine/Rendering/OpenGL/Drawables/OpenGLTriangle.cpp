#include "OpenGLTriangle.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLVertexBuffer.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLIndexBuffer.h"
#include "NoctalEngine/Rendering/OpenGL/Shaders/OpenGLShaderProgram.h"
#include "NoctalEngine/Rendering/OpenGL/Textures/OpenGLTexture2D.h"
#include "NoctalEngine/Rendering/Geometry/Triangle.h"

OpenGLTriangle::OpenGLTriangle(glm::vec2 position, glm::vec2 scale) : OpenGLBaseObject(position, scale)
{
	if (IsStaticInitialised() == false)
	{
		auto shape = NoctalEngine::Triangle::MakeTextured<Vertex>();

		NoctalEngine::VertexBufferData vbuf(NoctalEngine::VertexLayout{}.Append(NoctalEngine::VertexLayout::POS_3D).Append(NoctalEngine::VertexLayout::TEXCOORD));

		for (unsigned int i = 0; i < shape.m_Vertices.size(); i++)
		{
			vbuf.EmplaceBack(
				shape.m_Vertices[i].Pos,
				shape.m_Vertices[i].TexCoord
			);
		}

		AddStaticBind(std::unique_ptr<NoctalEngine::VertexBuffer>(new OpenGLVertexBuffer(vbuf)));
		SetStaticMaterial(std::make_unique<NoctalEngine::Material>());
		NE_ENGINE_ASSERT(m_Material, "Failed to create Material");

		std::unique_ptr<OpenGLShaderProgram> shader = std::make_unique<OpenGLShaderProgram>(
			"ColouredTextureVS",
			"ColouredTextureFS",
			*this);

		if (shader->IsValid() == true)
		{
			m_Material->SetShader(std::move(shader));
			m_Material->SetBaseTexture(std::make_shared<OpenGLTexture2D>());
		}
		else
		{
			NE_ENGINE_ERROR("Drawable Created, but Shader Failed to bind");
		}
	}
	else
	{
		SetMaterialFromStatic();
	}

	m_ObjectBuffer = std::make_unique<OpenGLObjectUniformBufferObject>();
	NE_ENGINE_INFO("Triangle Created");
}
