#pragma once
#include "Technique.h"

class GourandShading final : public Technique
{
	std::unique_ptr<Framebuffer> mFramebuffer;
	std::shared_ptr<Model> mRenderPlane;
	std::shared_ptr<Shader> mPostShader;

public:
	GourandShading();
	~GourandShading();

	GourandShading(const GourandShading &) = delete;
	GourandShading(GourandShading &&) = delete;
	GourandShading & operator= (const GourandShading &) = delete;
	GourandShading & operator= (GourandShading &&) = delete;

	void render(const std::shared_ptr<Shape>& pShape, bool pDeferred, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	void renderDirectionalSimpleShadow(const std::shared_ptr<Shape> & pShape) override;
	void renderPointSimpleShadow(const std::shared_ptr<Shape> & pShape) override;
	bool renderPostprocessing(const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderTransparent(const std::shared_ptr<Shape> & pShape, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
};

