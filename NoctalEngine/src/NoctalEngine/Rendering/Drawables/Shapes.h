#pragma once
#include "DrawableBase.h"

// The concrete shapes the renderer can create (see NoctalEngine::Geometry).
// To add a new 4-cornered shape: copy one of these, return a new Geometry value,
// and fill in BuildShape().

class QuadDrawable : public DrawableBase<QuadDrawable>
{
public:
	using DrawableBase::DrawableBase;

	NoctalEngine::Geometry GetGeometry() const override { return NoctalEngine::Geometry::QUAD; }
	static ShapeData BuildShape();
};

class TriangleDrawable : public DrawableBase<TriangleDrawable>
{
public:
	using DrawableBase::DrawableBase;

	NoctalEngine::Geometry GetGeometry() const override { return NoctalEngine::Geometry::TRIANGLE; }
	static ShapeData BuildShape();
};
