#pragma once

#include "Technique.h"

class EnvironmentMapping final : public Technique
{
	std::shared_ptr<Shader> mSetupShader;
	
public:
	EnvironmentMapping();
	~EnvironmentMapping() = default;

	EnvironmentMapping(const EnvironmentMapping &) = delete;
	EnvironmentMapping(EnvironmentMapping &&) = delete;
	EnvironmentMapping & operator= (const EnvironmentMapping &) = delete;
	EnvironmentMapping & operator= (EnvironmentMapping &&) = delete;

	void render(std::shared_ptr<Shape> & pShape, bool pDeffered, std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape) override;
	bool renderPostprocessing(std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	bool renderTransparent(std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
};

