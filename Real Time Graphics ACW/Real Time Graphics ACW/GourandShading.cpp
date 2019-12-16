#include "GourandShading.h"
#include "ResourceManager.h"

GourandShading::GourandShading() :
	Technique(
		ResourceManager::instance()->loadShader("GourandVertexShader.hlsl", "GourandFragmentShader.hlsl"),
		nullptr,
		ResourceManager::instance()->loadShader("NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl")
	)
{
}

void GourandShading::render(std::shared_ptr<Shape>& pShape, bool)
{
	mNormalShader->useShader();
	pShape->render();
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
