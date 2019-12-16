#pragma once
#include "Technique.h"

class GourandShading final : public Technique
{
public:
	GourandShading();
	~GourandShading() = default;

	GourandShading(const GourandShading &) = delete;
	GourandShading(GourandShading &&) = delete;
	GourandShading & operator= (const GourandShading &) = delete;
	GourandShading & operator= (GourandShading &&) = delete;

	void render(std::shared_ptr<Shape>& pShape, bool pDeferred) override;
	void renderDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
};

