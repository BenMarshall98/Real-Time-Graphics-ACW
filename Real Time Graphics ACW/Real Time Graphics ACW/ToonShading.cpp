#include "ToonShading.h"
#include "ResourceManager.h"

ToonShading::ToonShading() :
	Technique(
		ResourceManager::instance()->loadShader("ToonVertexShader.hlsl", "ToonFragmentShader.hlsl"),
		nullptr,
		ResourceManager::instance()->loadShader("NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl")
	)
{
}

void ToonShading::render(std::shared_ptr<Shape>& pShape, bool, std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	//mNormalShader->useShader();
	//pShape->render();
}

void ToonShading::renderDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	mDirectionalShader->useShader();
	pShape->render();
}

void ToonShading::renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	mOmniDirectionalShader->useShader();
	pShape->render();
}

bool ToonShading::renderPostprocessing(std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	return false; //TODO: Implement
}

bool ToonShading::renderTransparent(std::unique_ptr<Framebuffer> &)
{
	return false;
}