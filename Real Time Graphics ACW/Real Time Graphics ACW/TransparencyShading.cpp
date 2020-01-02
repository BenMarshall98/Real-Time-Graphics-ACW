#include "TransparencyShading.h"
#include "ResourceManager.h"
#include "DX11Render.h"

TransparencyShading::TransparencyShading() :
	Technique(
		ResourceManager::instance()->loadShader("TransparencyVertexShader.hlsl", "TransparencyFragmentShader.hlsl"),
		nullptr,
		ResourceManager::instance()->loadShader("NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl")
	)
{
	/*if (!mFramebuffer->loadFramebuffer(true, true, { { 0.0f, 0.0f, 0.0f, 0.0f } }))
	{
		mFramebuffer.reset();
	}*/
}

void TransparencyShading::render(std::shared_ptr<Shape> & pShape, bool, std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	//Dx11Render::instance()->enableBlend();
	/*mFramebuffer->useFramebuffer();
	mNormalShader->useShader();
	pShape->render();

	pCurrentFramebuffer->useFramebuffer(false);*/
	//Dx11Render::instance()->disableBlend();
}

void TransparencyShading::renderDirectionalShadow(std::shared_ptr<Shape> & pShape)
{
	mDirectionalShader->useShader();
	pShape->render();
}

void TransparencyShading::renderOmniDirectionalShadow(std::shared_ptr<Shape> & pShape)
{
	mOmniDirectionalShader->useShader();
	pShape->render();
}

bool TransparencyShading::renderPostprocessing(std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	return false;
}

void TransparencyShading::renderTransparent(std::shared_ptr<Shape> & pShape, std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	Dx11Render::instance()->enableBlend();
	mNormalShader->useShader();
	pShape->render();
	Dx11Render::instance()->disableBlend();
}