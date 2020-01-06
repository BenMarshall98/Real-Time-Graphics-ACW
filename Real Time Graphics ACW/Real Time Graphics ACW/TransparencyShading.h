#pragma once
#include "Technique.h"

class TransparencyShading final : public Technique
{
public:
	TransparencyShading();
	~TransparencyShading();

	TransparencyShading(const TransparencyShading &) = delete;
	TransparencyShading(TransparencyShading &&) = delete;
	TransparencyShading & operator= (const TransparencyShading &) = delete;
	TransparencyShading & operator= (TransparencyShading &&) = delete;

	void render(const std::shared_ptr<Shape> & pShape, bool pDeferred, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderDirectionalShadow(const std::shared_ptr<Shape> & pShape) override;
	void renderOmniDirectionalShadow(const std::shared_ptr<Shape> & pShape) override;
	bool renderPostprocessing(const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderTransparent(const std::shared_ptr<Shape> & pShape, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
};

