#pragma once
#include "Technique.h"

class ToonShading final : public Technique
{
public:
	ToonShading();
	~ToonShading() = default;

	ToonShading(const ToonShading &) = delete;
	ToonShading(ToonShading &&) = delete;
	ToonShading & operator= (const ToonShading &) = delete;
	ToonShading & operator= (ToonShading &&) = delete;

	void render(std::shared_ptr<Shape>& pShape, bool pDeferred) override;
	void renderDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
};

