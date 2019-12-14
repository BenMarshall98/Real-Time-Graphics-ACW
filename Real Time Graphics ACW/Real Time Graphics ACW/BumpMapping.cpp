#include "BumpMapping.h"
#include "ResourceManager.h"

BumpMapping::BumpMapping() :
	Technique(
		ResourceManager::instance()->loadShader("BumpVertexShader.hlsl", "BumpFragmentShader.hlsl"),
		nullptr,
		ResourceManager::instance()->loadShader("NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl")
	)
{
}

BumpMapping::~BumpMapping()
{
}

void BumpMapping::render(std::shared_ptr<Object>& pObject, bool pDeferred)
{
	mNormalShader->useShader();
	pObject->render();
}

void BumpMapping::renderDirectionalShadow(std::shared_ptr<Object>& pObject)
{
	mDirectionalShader->useShader();
	pObject->render();
}

void BumpMapping::renderOmniDirectionalShadow(std::shared_ptr<Object>& pObject)
{
	mOmniDirectionalShader->useShader();
	pObject->render();
}