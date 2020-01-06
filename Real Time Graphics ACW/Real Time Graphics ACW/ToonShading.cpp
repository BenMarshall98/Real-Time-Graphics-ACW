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

void ToonShading::render(const std::shared_ptr<Shape>& pShape, bool, std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	//mNormalShader->useShader();
	//pShape->render();
}

void ToonShading::renderDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useDirectionalShader();
	pShape->render();
}

void ToonShading::renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useOmniDirectionalShader();
	pShape->render();
}

bool ToonShading::renderPostprocessing(std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	return false; //TODO: Implement
}

void ToonShading::renderTransparent(std::shared_ptr<Shape> &, std::unique_ptr<Framebuffer> &)
{
	//Do Nothing
}