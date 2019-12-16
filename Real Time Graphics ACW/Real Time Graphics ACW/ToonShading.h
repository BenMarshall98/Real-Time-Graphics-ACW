#pragma once
#include "Technique.h"

class ToonShading final : public Technique
{
public:
	ToonShading();
	~ToonShading();

	void render(std::shared_ptr<Shape>& pShape, bool pDeferred) override;
	void renderDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
};

