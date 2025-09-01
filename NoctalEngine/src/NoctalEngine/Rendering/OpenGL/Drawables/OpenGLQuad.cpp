#include "OpenGLQuad.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLVertexBuffer.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLIndexBuffer.h"
#include "NoctalEngine/Rendering/OpenGL/Shaders/OpenGLShaderProgram.h"
#include "NoctalEngine/Rendering/OpenGL/Textures/OpenGLTexture2D.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLVertexArray.h"
#include "NoctalEngine/Rendering/Geometry/Quad.h"

OpenGLQuad::OpenGLQuad(glm::vec2 position, glm::vec2 scale) : OpenGLBaseObject(position, scale)
{
	m_Geometry = NoctalEngine::Geometry::QUAD;

	if (IsStaticInitialised() == false)
	{
		//glCreateVertexArrays(1, &m_RendererID);
		//glBindVertexArray(m_RendererID);

		//SetRendererID(m_RendererID);

		auto shape = NoctalEngine::Quad::MakeTextured<Vertex>();

		NoctalEngine::VertexBufferData bufferData(NoctalEngine::VertexLayout{}.Append(NoctalEngine::VertexLayout::POS_3D).Append(NoctalEngine::VertexLayout::TEXCOORD));

		for (unsigned int i = 0; i < shape.m_Vertices.size(); i++)
		{
			bufferData.EmplaceBack(
				shape.m_Vertices[i].Pos,
				shape.m_Vertices[i].TexCoord
			);
		}
	
		SubmitVertexArrayToRenderer(m_Geometry, std::make_unique<OpenGLVertexArray>(bufferData, shape));

		//AddStaticBind(std::unique_ptr<NoctalEngine::VertexBuffer>(new OpenGLVertexBuffer(vbuf)));
		//SetStaticIndexBuffer(std::unique_ptr<NoctalEngine::IndexBuffer>(new OpenGLIndexBuffer(shape.m_Indices)));

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
		//SetRendererIDFromStatic();
		//SetIndexFromStatic();
		SetMaterialFromStatic();
	}

	m_ObjectBuffer = std::make_unique<OpenGLObjectUniformBufferObject>();
	NE_ENGINE_INFO("Quad Created");
}


