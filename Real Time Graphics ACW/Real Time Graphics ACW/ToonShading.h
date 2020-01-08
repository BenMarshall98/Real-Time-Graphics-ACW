#pragma once
#include "Technique.h"

class ToonShading final : public Technique
{
public:
	ToonShading();
	~ToonShading();

	ToonShading(const ToonShading &) = delete;
	ToonShading(ToonShading &&) = delete;
	ToonShading & operator= (const ToonShading &) = delete;
	ToonShading & operator= (ToonShading &&) = delete;

	void render(const std::shared_ptr<Shape>& pShape, bool pDeferred, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	void renderDirectionalSimpleShadow(const std::shared_ptr<Shape> & pShape) override;
	void renderPointSimpleShadow(const std::shared_ptr<Shape> & pShape) override;
	bool renderPostprocessing(const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderTransparent(const std::shared_ptr<Shape> & pShape, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
};

