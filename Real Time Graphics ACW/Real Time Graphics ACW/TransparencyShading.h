#pragma once
#include "Technique.h"

class TransparencyShading final : public Technique
{
	std::unique_ptr<Framebuffer> mFramebuffer;
	std::shared_ptr<Model> mRenderPlane;
	std::shared_ptr<Shader> mPostShader;

public:
	TransparencyShading();
	~TransparencyShading() = default;

	TransparencyShading(const TransparencyShading &) = delete;
	TransparencyShading(TransparencyShading &&) = delete;
	TransparencyShading & operator= (const TransparencyShading &) = delete;
	TransparencyShading & operator= (TransparencyShading &&) = delete;

	void render(std::shared_ptr<Shape> & pShape, bool pDeferred, std::unique_ptr<Framebuffer> & pCurrentFramebuffer);
	void renderDirectionalShadow(std::shared_ptr<Shape> & pShape) override;
	void renderOmniDirectionalShadow(std::shared_ptr<Shape> & pShape) override;
	bool renderPostprocessing(std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	bool renderTransparent(std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
};

