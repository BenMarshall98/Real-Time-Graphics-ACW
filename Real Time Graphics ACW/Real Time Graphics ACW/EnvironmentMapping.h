#pragma once

#include "Technique.h"
#include "Framebuffer.h"
#include <array>

struct EnvironmentMatrixBuffer
{
	std::array<DirectX::XMFLOAT4X4, 6> mView;
	DirectX::XMFLOAT4X4 mPerspective;
};

class EnvironmentMapping final : public Technique
{
	std::shared_ptr<Shader> mSetupShader;
	std::unique_ptr<Framebuffer> mFramebuffer;
	
public:
	EnvironmentMapping();
	~EnvironmentMapping();

	EnvironmentMapping(const EnvironmentMapping &) = delete;
	EnvironmentMapping(EnvironmentMapping &&) = delete;
	EnvironmentMapping & operator= (const EnvironmentMapping &) = delete;
	EnvironmentMapping & operator= (EnvironmentMapping &&) = delete;

	void render(const std::shared_ptr<Shape> & pShape, bool pDeferred, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	void renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape) override;
	void renderDirectionalSimpleShadow(const std::shared_ptr<Shape> & pShape) override;
	void renderPointSimpleShadow(const std::shared_ptr<Shape> & pShape) override;
	bool renderPostprocessing(const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
	void renderTransparent(const std::shared_ptr<Shape> & pShape, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) override;
};

