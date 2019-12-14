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

PhongShading::~PhongShading()
{
}

void PhongShading::render(std::shared_ptr<Object>& pObject, bool pDeferred)
{
	mNormalShader->useShader();
	pObject->render();
}

void PhongShading::renderDirectionalShadow(std::shared_ptr<Object>& pObject)
{
	mDirectionalShader->useShader();
	pObject->render();
}

void PhongShading::renderOmniDirectionalShadow(std::shared_ptr<Object>& pObject)
{
	mOmniDirectionalShader->useShader();
	pObject->render();
}