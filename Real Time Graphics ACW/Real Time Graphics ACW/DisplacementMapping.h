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

	void render(std::shared_ptr<Shape>& pShape, bool pDeferred, std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
	bool renderPostprocessing(std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	bool renderTransparent(std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
};

