#pragma once
#include "Technique.h"

class BumpMapping final : public Technique
{
public:
	BumpMapping();
	~BumpMapping();

	BumpMapping(const BumpMapping&) = delete;
	BumpMapping(BumpMapping &&) = delete;
	BumpMapping & operator= (const BumpMapping &) = delete;
	BumpMapping & operator= (BumpMapping &&) = delete;

	void render(const std::shared_ptr<Shape> & pShape, bool pDeferred, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderDirectionalShadow(const std::shared_ptr<Shape> & pShape) override;
	void renderOmniDirectionalShadow(const std::shared_ptr<Shape> & pShape) override;
	bool renderPostprocessing(const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderTransparent(const std::shared_ptr<Shape> & pShape, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
};

