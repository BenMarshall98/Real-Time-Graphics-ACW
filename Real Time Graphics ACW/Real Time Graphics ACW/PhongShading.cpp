#include "PhongShading.h"
#include "ResourceManager.h"

PhongShading::PhongShading() :
	Technique(
		ResourceManager::instance()->loadShader("PhongVertexShader.hlsl", "PhongFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("PhongDeferredVertexShader.hlsl", "PhongDeferredFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl")
	)
{
}

void PhongShading::render(std::shared_ptr<Shape>& pShape, bool pDeferred)
{
	if (pDeferred)
	{
		mDeferredShader->useShader();
	}
	else
	{
		mNormalShader->useShader();
	}
	
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