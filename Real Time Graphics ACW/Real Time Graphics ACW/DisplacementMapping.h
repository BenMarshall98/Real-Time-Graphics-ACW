#pragma once
#include "Technique.h"

class DisplacementMapping final : public Technique
{
public:
	DisplacementMapping();
	~DisplacementMapping();

	DisplacementMapping(const DisplacementMapping &) = delete;
	DisplacementMapping(DisplacementMapping &&) = delete;
	DisplacementMapping & operator= (const DisplacementMapping &) = delete;
	DisplacementMapping & operator= (DisplacementMapping &&) = delete;

	void render(const std::shared_ptr<Shape>& pShape, bool pDeferred, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	void renderDirectionalSimpleShadow(const std::shared_ptr<Shape> & pShape) override;
	void renderPointSimpleShadow(const std::shared_ptr<Shape> & pShape) override;
	bool renderPostprocessing(const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderTransparent(const std::shared_ptr<Shape> & pShape, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
};

