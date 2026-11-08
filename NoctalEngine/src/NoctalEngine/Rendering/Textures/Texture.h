#pragma once
#include "NoctalEngine/Rendering/Bindable.h"

namespace NoctalEngine
{
	class Texture : public Bindable
	{
	public:
		virtual ~Texture() override = default;

		// Binds the texture into the slot set by SetTextureIndex().
		// The batch renderer gives each texture in a batch its own slot (0..MaxTextureSlots-1),
		// and each vertex stores which slot to sample from.
		virtual void Bind() override = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetTextureIndex(int index) = 0;

		// The file the texture came from (empty for textures made from raw pixels).
		virtual const std::string& GetPath() const = 0;
	};
}
