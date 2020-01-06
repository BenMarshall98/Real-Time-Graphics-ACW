#pragma once
#include "Technique.h"

class DisplacementMapping final : public Technique
{
public:
	DisplacementMapping();
	~DisplacementMapping() = default;

	DisplacementMapping(const DisplacementMapping &) = delete;
	DisplacementMapping(DisplacementMapping &&) = delete;
	DisplacementMapping & operator= (const DisplacementMapping &) = delete;
	DisplacementMapping & operator= (DisplacementMapping &&) = delete;

	void render(const std::shared_ptr<Shape>& pShape, bool pDeferred, std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	bool renderPostprocessing(std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderTransparent(std::shared_ptr<Shape> & pShape, std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
};

