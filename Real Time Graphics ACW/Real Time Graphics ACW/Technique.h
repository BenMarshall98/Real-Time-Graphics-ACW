#pragma once

#include "Shape.h"
#include "Shader.h"

class Technique
{
protected:
	std::shared_ptr<Shader> mNormalShader;
	std::shared_ptr<Shader> mDeferredShader;
	std::shared_ptr<Shader> mDirectionalShader;
	std::shared_ptr<Shader> mOmniDirectionalShader;
	
public:
	Technique(std::shared_ptr<Shader> pNormalShader, std::shared_ptr<Shader> pDeferredShader,
		std::shared_ptr<Shader> pDirectionalShader, std::shared_ptr<Shader> pOmniDirectionalShader);
	virtual ~Technique() = default;

	Technique(const Technique &) = delete;
	Technique(Technique &&) = delete;
	Technique & operator= (const Technique &) = delete;
	Technique & operator= (Technique &&) = delete;

	virtual void render(std::shared_ptr<Shape>& pShape, bool pDeferred, std::unique_ptr<Framebuffer> & pCurrentFramebuffer) = 0;
	virtual void renderDirectionalShadow(std::shared_ptr<Shape>& pShape) = 0;
	virtual void renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape) = 0;
	virtual void renderPostprocessing(std::unique_ptr<Framebuffer> & pCurrentFramebuffer) = 0;
};

