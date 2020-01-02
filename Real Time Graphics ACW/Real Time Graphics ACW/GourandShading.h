#pragma once
#include "Technique.h"

class GourandShading final : public Technique
{
	std::unique_ptr<Framebuffer> mFramebuffer;
	std::shared_ptr<Model> mRenderPlane;
	std::shared_ptr<Shader> mPostShader;

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
	bool renderPostprocessing(std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderTransparent(std::shared_ptr<Shape> & pShape, std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
};

