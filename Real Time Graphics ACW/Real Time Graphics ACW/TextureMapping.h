#pragma once
#include "Technique.h"

class TextureMapping final : public Technique
{
public:
	TextureMapping();
	~TextureMapping();

	TextureMapping(const TextureMapping&) = delete;
	TextureMapping(TextureMapping &&) = delete;
	TextureMapping & operator= (const TextureMapping &) = delete;
	TextureMapping & operator= (TextureMapping &&) = delete;

	void render(const std::shared_ptr<Shape>& pShape, bool pDeferred, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	bool renderPostprocessing(const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderTransparent(const std::shared_ptr<Shape> & pShape, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
};

