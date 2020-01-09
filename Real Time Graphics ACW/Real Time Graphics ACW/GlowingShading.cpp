#include "GlowingShading.h"
#include "ResourceManager.h"
#include "DX11Render.h"

GlowingShading::GlowingShading() : mFramebufferOne(std::make_unique<Framebuffer>()), mFramebufferTwo(std::make_unique<Framebuffer>()), mFramebuffer(std::make_unique<Framebuffer>())
{
	std::shared_ptr<Shader> normalShader;
	std::shared_ptr<Shader> directionalShader;
	std::shared_ptr<Shader> omniDirectionalShader;
	std::shared_ptr<Shader> directionalSimpleShader;
	std::shared_ptr<Shader> pointSimpleShader;

	getNormalShader(normalShader);
	getDirectionalShader(directionalShader);
	getOmniDirectionalShader(omniDirectionalShader);
	getDirectionalSimpleShader(directionalSimpleShader);
	getPointSimpleShader(pointSimpleShader);

	ResourceManager::instance()->loadShader(normalShader, "PhongVertexShader.hlsl", "PhongFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(directionalShader, "NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(omniDirectionalShader, "NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl");
	ResourceManager::instance()->loadShader(directionalSimpleShader, "NormalDirectionalSimpleShadowVertexShader.hlsl", "NormalDirectionalSimpleShadowFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(pointSimpleShader, "NormalPointSimpleShadowVertexShader.hlsl", "NormalPointSimpleShadowFragmentShader.hlsl");

	setNormalShader(normalShader);
	setDirectionalShader(directionalShader);
	setOmniDirectionalShader(omniDirectionalShader);
	setDirectionalSimpleShader(directionalSimpleShader);
	setPointSimpleShader(pointSimpleShader);

	ResourceManager::instance()->loadModel(mRenderPlane, "plane.obj");
	ResourceManager::instance()->loadShader(mPostShader, "PostVertexShader.hlsl", "PostFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(mHorzShader, "GlowHorzVertexShader.hlsl", "GlowHorzFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(mVertShader, "GlowVertVertexShader.hlsl", "GlowVertFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(mGlowShader, "GlowVertexShader.hlsl", "GlowFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(mFinalShader, "FinalVertexShader.hlsl", "FinalFragmentShader.hlsl");

	if (!mFramebufferOne->loadFramebuffer(true, true, { {0.0f, 0.0f, 0.0f, 0.0f} }))
	{
		mFramebufferOne.reset();
	}

	if (!mFramebufferTwo->loadFramebuffer(true, true, { {0.0f, 0.0f, 0.0f, 0.0f} }))
	{
		mFramebufferOne.reset();
	}

	if (!mFramebuffer->loadFramebuffer(true, true, { {0.0f, 0.0f, 0.0f, 0.0f} }))
	{
		mFramebufferOne.reset();
	}
}

GlowingShading::~GlowingShading() = default;

void GlowingShading::render(const std::shared_ptr<Shape>& pShape, bool, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	mFramebuffer->useFramebuffer();

	useNormalShader();
	pShape->render();

	mFramebuffer->useTexture(26);

	mFramebufferTwo->useFramebuffer();
	mGlowShader->useShader();
	
	mRenderPlane->render();
	
	mFramebuffer->releaseTexture(26);

	for (auto i = 0u; i < 5; i++)
	{
		//Horz Blur
		{
			mFramebufferTwo->useTexture(26);

			mFramebufferOne->useFramebuffer();
			mHorzShader->useShader();

			mRenderPlane->render();

			mFramebufferTwo->releaseTexture(26);
		}

		//Vert Blur
		{
			mFramebufferOne->useTexture(26);

			mFramebufferTwo->useFramebuffer();
			mVertShader->useShader();

			mRenderPlane->render();

			mFramebufferOne->releaseTexture(26);
		}
	}

	mFramebuffer->useTexture(26);
	mFramebufferTwo->useTexture(28);

	mFramebufferOne->useFramebuffer();
	mFinalShader->useShader();

	mRenderPlane->render();

	mFramebufferTwo->releaseTexture(28);
	mFramebuffer->releaseTexture(26);

	pCurrentFramebuffer->useFramebuffer(false);
}

void GlowingShading::renderDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useDirectionalShader();
	pShape->render();
}

void GlowingShading::renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useOmniDirectionalShader();
	pShape->render();
}

void GlowingShading::renderDirectionalSimpleShadow(const std::shared_ptr<Shape> & pShape)
{
	useDirectionalSimpleShader();
	pShape->render();
}

void GlowingShading::renderPointSimpleShadow(const std::shared_ptr<Shape> & pShape)
{
	usePointSimpleShader();
	pShape->render();
}

bool GlowingShading::renderPostprocessing(const std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	mFramebufferOne->useTexture(14);

	if (pCurrentFramebuffer == nullptr)
	{
		Dx11Render::instance()->bindDefaultFramebuffer();
	}
	else
	{
		pCurrentFramebuffer->useFramebuffer();
	}

	mPostShader->useShader();

	mRenderPlane->render();

	mFramebufferOne->releaseTexture(14);

	return true;
}

void GlowingShading::renderTransparent(const std::shared_ptr<Shape> &, const std::unique_ptr<Framebuffer> &)
{
	//Do Nothing
}