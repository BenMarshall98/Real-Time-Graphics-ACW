#include "DisplacementMapping.h"
#include "ResourceManager.h"

DisplacementMapping::DisplacementMapping() :
	Technique(
		ResourceManager::instance()->loadShader("DisplacementVertexShader.hlsl", "DisplacementFragmentShader.hlsl", "DisplacementHullShader.hlsl", "DisplacementDomainShader.hlsl"),
		nullptr,
		ResourceManager::instance()->loadShader("DisplacementDirectionalShadowVertexShader.hlsl", "DisplacementDirectionalShadowFragmentShader.hlsl", "DisplacementDirectionalShadowHullShader.hlsl", "DisplacementDirectionalShadowDomainShader.hlsl"),
		ResourceManager::instance()->loadShader("DisplacementOmniShadowVertexShader.hlsl", "DisplacementOmniShadowFragmentShader.hlsl", "DisplacmentOmniShadowGeometryShader.hlsl", "DisplacmentOmniShadowHullShader.hlsl", "DisplacementOmniDomainShader.hlsl")
	)
{
}

DisplacementMapping::~DisplacementMapping()
{
}

void DisplacementMapping::render(std::shared_ptr<Object> & pObject, bool pDeferred)
{
	mNormalShader->useShader();
	pObject->render(true);
}

void DisplacementMapping::renderDirectionalShadow(std::shared_ptr<Object>& pObject)
{
	mDirectionalShader->useShader();
	pObject->render(true);
}

void DisplacementMapping::renderOmniDirectionalShadow(std::shared_ptr<Object>& pObject)
{
	mOmniDirectionalShader->useShader();
	pObject->render(true);
}