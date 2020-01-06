#pragma once
#include "Technique.h"

class PhongShading final : public Technique
{
public:
	PhongShading();
	~PhongShading();

	PhongShading(const PhongShading &) = delete;
	PhongShading(PhongShading &&) = delete;
	PhongShading & operator= (const PhongShading &) = delete;
	PhongShading & operator= (PhongShading &&) = delete;

	void render(const std::shared_ptr<Shape>& pShape, bool pDeferred, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	bool renderPostprocessing(const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderTransparent(const std::shared_ptr<Shape> & pShape, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
};

