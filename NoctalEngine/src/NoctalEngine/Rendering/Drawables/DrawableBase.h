#pragma once
#include "Drawable.h"

// DrawableBase<T> gives every shape type (T) ONE shared copy of its shape data.
//
// This is the same idea the old version had with "static binds" (every quad shared one
// vertex/index buffer), just adapted for batching: instead of sharing GPU buffers, every
// QuadDrawable shares the same 4 local corners/UVs, built once on first use.
//
// T must provide:  static ShapeData BuildShape();
template<class T>
class DrawableBase : public Drawable
{
public:
	using Drawable::Drawable;

	const ShapeData& GetShape() const override
	{
		// A function-local static is created the first time this runs (thread-safe in C++11+),
		// then reused by every instance of T.
		static const ShapeData s_Shape = T::BuildShape();
		return s_Shape;
	}
};
