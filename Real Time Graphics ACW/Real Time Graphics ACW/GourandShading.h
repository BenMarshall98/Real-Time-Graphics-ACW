#pragma once
#include "Technique.h"

class GourandShading final : public Technique
{
	std::unique_ptr<Framebuffer> mFramebuffer;

public:
	GourandShading();
	~GourandShading() = default;

	GourandShading(const GourandShading &) = delete;
	GourandShading(GourandShading &&) = delete;
	GourandShading & operator= (const GourandShading &) = delete;
	GourandShading & operator= (GourandShading &&) = delete;

	void render(std::shared_ptr<Shape>& pShape, bool pDeferred, std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
	void renderPostprocessing(std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
};

