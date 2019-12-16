#pragma once
#include "Technique.h"

class GourandShading final : public Technique
{
public:
	GourandShading();
	~GourandShading();

	void render(std::shared_ptr<Shape>& pShape, bool pDeferred) override;
	void renderDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
};

