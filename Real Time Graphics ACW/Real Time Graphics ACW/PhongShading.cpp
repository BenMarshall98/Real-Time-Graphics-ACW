#include "PhongShading.h"
#include "ResourceManager.h"

PhongShading::PhongShading() :
	Technique(
		ResourceManager::instance()->loadShader("PhongVertexShader.hlsl", "PhongFragmentShader.hlsl"),
		nullptr,
		ResourceManager::instance()->loadShader("NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl")
	)
{
}

void PhongShading::render(std::shared_ptr<Shape>& pShape, bool)
{
	mNormalShader->useShader();
	pShape->render();
}

void PhongShading::renderDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	mDirectionalShader->useShader();
	pShape->render();
}

void PhongShading::renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	mOmniDirectionalShader->useShader();
	pShape->render();
}