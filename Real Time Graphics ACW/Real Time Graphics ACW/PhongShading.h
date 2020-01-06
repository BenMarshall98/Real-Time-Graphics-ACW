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

	void render(const std::shared_ptr<Shape>& pShape, bool pDeferred, std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	bool renderPostprocessing(std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderTransparent(std::shared_ptr<Shape> & pShape, std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
};

