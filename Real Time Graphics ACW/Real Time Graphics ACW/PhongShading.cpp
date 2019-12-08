#include "PhongShading.h"
#include "ResourceManager.h"

PhongShading::PhongShading() :
	Technique(ResourceManager::instance()->loadShader(
		"PhongVertexShader.hlsl", "PhongFragmentShader.hlsl"), nullptr)
{
}

PhongShading::~PhongShading()
{
}

void PhongShading::render(std::shared_ptr<Object>& pObject, bool pDeferred)
{
	mShader->useShader();
	pObject->render();
}
