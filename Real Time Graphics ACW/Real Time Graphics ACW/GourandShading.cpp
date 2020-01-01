#include "GourandShading.h"
#include "ResourceManager.h"
#include "DX11Render.h"

GourandShading::GourandShading() :
	Technique(
		ResourceManager::instance()->loadShader("GourandVertexShader.hlsl", "GourandFragmentShader.hlsl"),
		nullptr,
		ResourceManager::instance()->loadShader("NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl")
	), mFramebuffer(std::make_unique<Framebuffer>()), mRenderPlane(ResourceManager::instance()->loadModel("plane.obj")),
	mPostShader(ResourceManager::instance()->loadShader("PostVertexShader.hlsl", "PostFragmentShader.hlsl"))
{
	if (!mFramebuffer->loadFramebuffer(true, true, { { 0.0f, 0.0f, 0.0f, 0.0f } }))
	{
		mFramebuffer.reset();
	}
}

void GourandShading::render(std::shared_ptr<Shape>& pShape, bool, std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	mFramebuffer->useFramebuffer();
	mNormalShader->useShader();
	pShape->render();

	pCurrentFramebuffer->useFramebuffer(false);
}

void GourandShading::renderDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	mDirectionalShader->useShader();
	pShape->render();
}

void GourandShading::renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	mOmniDirectionalShader->useShader();
	pShape->render();
}

bool GourandShading::renderPostprocessing(std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	mFramebuffer->useTexture(8);

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

	mFramebuffer->releaseTexture(8);

	return true;
}

bool GourandShading::renderTransparent(std::unique_ptr<Framebuffer> &)
{
	return false;
}