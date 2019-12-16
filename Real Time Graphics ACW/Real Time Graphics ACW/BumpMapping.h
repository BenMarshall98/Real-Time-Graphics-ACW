#pragma once
#include "Technique.h"

class BumpMapping final : public Technique
{
public:
	BumpMapping();
	~BumpMapping() = default;

	BumpMapping(const BumpMapping&) = delete;
	BumpMapping(BumpMapping &&) = delete;
	BumpMapping & operator= (const BumpMapping &) = delete;
	BumpMapping & operator= (BumpMapping &&) = delete;

	void render(std::shared_ptr<Shape>& pShape, bool pDeferred) override;
	void renderDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
};

