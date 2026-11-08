#pragma once
#include <glm/glm.hpp>
#include <memory>

namespace NoctalEngine
{
	class Texture;

	// Describes how a Drawable should look: a tint colour and an optional texture.
	//
	// With batch rendering a Material no longer binds anything itself. Instead the batch
	// renderer reads the colour/texture and writes them into the vertices, so objects with
	// different materials can still be drawn in the same draw call.
	class Material
	{
	public:
		Material() = default;
		~Material() = default;

		void SetColour(const glm::vec4& colour) { m_Colour = colour; }
		const glm::vec4& GetColour() const { return m_Colour; }

		// nullptr = untextured (the renderer uses a 1x1 white texture, so you just see the colour).
		void SetBaseTexture(std::shared_ptr<Texture> texture) { m_BaseTexture = std::move(texture); }
		Texture* GetBaseTexture() const { return m_BaseTexture.get(); }

	private:
		glm::vec4 m_Colour = glm::vec4(1.0f);
		std::shared_ptr<Texture> m_BaseTexture;
	};
}
