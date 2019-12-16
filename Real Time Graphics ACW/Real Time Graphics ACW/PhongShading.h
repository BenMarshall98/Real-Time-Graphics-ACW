#pragma once
#include "Technique.h"

class PhongShading final : public Technique
{
public:
	PhongShading();
	~PhongShading() = default;

	PhongShading(const PhongShading &) = delete;
	PhongShading(PhongShading &&) = delete;
	PhongShading & operator= (const PhongShading &) = delete;
	PhongShading & operator= (PhongShading &&) = delete;

	void render(std::shared_ptr<Shape>& pShape, bool pDeferred) override;
	void renderDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
};

