#pragma once

#include "Shape.h"
#include "Shader.h"
#include "Framebuffer.h"

class Technique
{
	std::shared_ptr<Shader> mNormalShader;
	std::shared_ptr<Shader> mDeferredShader;
	std::shared_ptr<Shader> mDirectionalShader;
	std::shared_ptr<Shader> mOmniDirectionalShader;
	std::shared_ptr<Shader> mDirectionalSimpleShader;
	std::shared_ptr<Shader> mPointSimpleShader;

protected:
	void getNormalShader(std::shared_ptr<Shader> & pNormalShader) const
	{
		pNormalShader = mNormalShader;
	}

	void setNormalShader(const std::shared_ptr<Shader> & pNormalShader)
	{
		mNormalShader = pNormalShader;
	}

	void getDeferredShader(std::shared_ptr<Shader> & pDeferredShader) const
	{
		pDeferredShader = mDeferredShader;
	}

	void setDeferredShader(const std::shared_ptr<Shader> & pDeferredShader)
	{
		mDeferredShader = pDeferredShader;
	}

	void getDirectionalShader(std::shared_ptr<Shader> & pDirectionalShader) const
	{
		pDirectionalShader = mDirectionalShader;
	}

	void setDirectionalShader(const std::shared_ptr<Shader> & pDirectionalShader)
	{
		mDirectionalShader = pDirectionalShader;
	}

	void getOmniDirectionalShader(std::shared_ptr<Shader> & pOmniDirectionalShader) const
	{
		pOmniDirectionalShader = mOmniDirectionalShader;
	}

	void setOmniDirectionalShader(const std::shared_ptr<Shader> & pOmniDirectionalShader)
	{
		mOmniDirectionalShader = pOmniDirectionalShader;
	}

	void getDirectionalSimpleShader(std::shared_ptr<Shader> & pDirectionalSimpleShader) const
	{
		pDirectionalSimpleShader = mDirectionalSimpleShader;
	}

	void setDirectionalSimpleShader(const std::shared_ptr<Shader> & pDirectionalSimpleShader)
	{
		mDirectionalSimpleShader = pDirectionalSimpleShader;
	}

	void getPointSimpleShader(std::shared_ptr<Shader> & pPointSimpleShader) const
	{
		pPointSimpleShader = mPointSimpleShader;
	}

	void setPointSimpleShader(const std::shared_ptr<Shader> & pPointSimpleShader)
	{
		mPointSimpleShader = pPointSimpleShader;
	}

	void useNormalShader() const
	{
		mNormalShader->useShader();
	}

	void useDeferredShader() const
	{
		mDeferredShader->useShader();
	}

	void useDirectionalShader() const
	{
		mDirectionalShader->useShader();
	}

	void useOmniDirectionalShader() const
	{
		mOmniDirectionalShader->useShader();
	}

	void useDirectionalSimpleShader() const
	{
		mDirectionalSimpleShader->useShader();
	}

	void usePointSimpleShader() const
	{
		mPointSimpleShader->useShader();
	}
	
public:
	Technique() = default;
	virtual ~Technique();

	Technique(const Technique &) = delete;
	Technique(Technique &&) = delete;
	Technique & operator= (const Technique &) = delete;
	Technique & operator= (Technique &&) = delete;

	virtual void render(const std::shared_ptr<Shape> & pShape, bool pDeferred, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) = 0;
	virtual void renderDirectionalShadow(const std::shared_ptr<Shape> & pShape) = 0;
	virtual void renderOmniDirectionalShadow(const std::shared_ptr<Shape> & pShape) = 0;
	virtual void renderDirectionalSimpleShadow(const std::shared_ptr<Shape> & pShape) = 0;
	virtual void renderPointSimpleShadow(const std::shared_ptr<Shape> & pShape) = 0;
	virtual bool renderPostprocessing(const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) = 0;
	virtual void renderTransparent(const std::shared_ptr<Shape> & pShape, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer) = 0;
};

