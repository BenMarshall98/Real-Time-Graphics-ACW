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
}

void TransparencyShading::render(std::shared_ptr<Shape> & pShape, bool, std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
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