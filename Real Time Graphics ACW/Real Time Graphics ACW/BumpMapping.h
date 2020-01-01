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

	void render(std::shared_ptr<Shape>& pShape, bool pDeferred, std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
	bool renderPostprocessing(std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	bool renderTransparent(std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
};

