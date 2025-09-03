#include "nepch.h"
#include "OpenGLVertexBuffer.h"
#include "NoctalEngine/Rendering/RendererData.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(const NoctalEngine::VertexBufferData& buffer) : m_Size(buffer.SizeBytes()), m_Layout({
		{ NoctalEngine::ShaderDataType::POS_3D, "a_Position"},
		{ NoctalEngine::ShaderDataType::POS_2D, "a_TexCoord"},
	})
{
	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, m_Size * RendererData::MaxVerts, nullptr, GL_DYNAMIC_DRAW);

	uint32_t index = 0;

	for (const auto& element : buffer.GetLayout().elements)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, element.Count(), ElementTypeToOpenGLBaseType(element.GetType()), GL_FALSE, buffer.GetLayout().Size(), (const void*)element.GetOffset());
		index++;
	}

    //for (const auto& element : buffer.GetLayout().elements)
    //{
    //    GLenum baseType = ElementTypeToOpenGLBaseType(element.GetType());

    //    if (element.GetType() == NoctalEngine::VertexLayout::ElementType::TRANSFORM)
    //    {
    //        // A mat4 = 4 vec4s (4 attributes)
    //        for (uint32_t i = 0; i < 4; i++)
    //        {
    //            glEnableVertexAttribArray(index + i);
    //            glVertexAttribPointer(
    //                index + i,
    //                4,                              // vec4 = 4 floats
    //                baseType,                       // GL_FLOAT
    //                GL_FALSE,
    //                buffer.GetLayout().Size(),      // stride = size of entire vertex/instance
    //                (const void*)(element.GetOffset() + sizeof(float) * 4 * i) // offset to column i
    //            );

    //            // If this is per-instance data (common for transforms), you also need:
    //            glVertexAttribDivisor(index + i, 1);
    //        }

    //        index += 4; // consumed 4 attribute slots
    //    }
    //    else
    //    {
    //        glEnableVertexAttribArray(index);
    //        glVertexAttribPointer(
    //            index,
    //            element.Count(),                    // 1..4
    //            baseType,
    //            GL_FALSE,
    //            buffer.GetLayout().Size(),
    //            (const void*)element.GetOffset()
    //        );
    //        index++;
    //    }
    //}
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void OpenGLVertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

GLenum OpenGLVertexBuffer::ElementTypeToOpenGLBaseType(NoctalEngine::VertexLayout::ElementType type)
{
	switch (type)
	{
		case NoctalEngine::VertexLayout::ElementType::POS_2D:			return GL_FLOAT;
		case NoctalEngine::VertexLayout::ElementType::POS_3D:			return GL_FLOAT;
		case NoctalEngine::VertexLayout::ElementType::TEXCOORD:			return GL_FLOAT;
		case NoctalEngine::VertexLayout::ElementType::TRANSFORM:        return GL_FLOAT;
	}

	NE_ENGINE_ASSERT(false, "Unknown ShaderDataType");
	return 0;
}

