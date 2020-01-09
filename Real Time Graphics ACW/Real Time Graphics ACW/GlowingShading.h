#pragma once
#include "Technique.h"

class GlowingShading final : public Technique
{
	std::unique_ptr<Framebuffer> mFramebufferOne;
	std::unique_ptr<Framebuffer> mFramebufferTwo;
	std::unique_ptr<Framebuffer> mFramebuffer;
	std::shared_ptr<Shader> mPostShader;
	std::shared_ptr<Shader> mHorzShader;
	std::shared_ptr<Shader> mVertShader;
	std::shared_ptr<Shader> mGlowShader;
	std::shared_ptr<Shader> mFinalShader;
	std::shared_ptr<Model> mRenderPlane;

public:
	GlowingShading();
	~GlowingShading();

	GlowingShading(const GlowingShading &) = delete;
	GlowingShading(GlowingShading &&) = delete;
	GlowingShading & operator= (const GlowingShading &) = delete;
	GlowingShading & operator= (GlowingShading &&) = delete;

	void render(const std::shared_ptr<Shape>& pShape, bool pDeferred, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	void renderDirectionalSimpleShadow(const std::shared_ptr<Shape> & pShape) override;
	void renderPointSimpleShadow(const std::shared_ptr<Shape> & pShape) override;
	bool renderPostprocessing(const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderTransparent(const std::shared_ptr<Shape> & pShape, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
};
