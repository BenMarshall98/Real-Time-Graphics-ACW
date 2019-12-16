#pragma once
#include "Technique.h"

class BumpMapping final : public Technique
{
public:
	BumpMapping();
	~BumpMapping();

	void render(std::shared_ptr<Shape>& pShape, bool pDeferred) override;
	void renderDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
};

