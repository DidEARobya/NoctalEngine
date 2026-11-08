#pragma once
#include "Texture.h"

namespace NoctalEngine
{
	// A regular 2D image texture. API versions (e.g. D3D12Texture2D) inherit from this.
	// (Renamed from 'TEXCOORD', which looked like an accidental find-and-replace.)
	class Texture2D : public Texture
	{
	};
}
