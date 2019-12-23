#include "DisplacementMapping.h"
#include "ResourceManager.h"

DisplacementMapping::DisplacementMapping() :
	Technique(
		ResourceManager::instance()->loadShader("DisplacementVertexShader.hlsl", "DisplacementFragmentShader.hlsl", "DisplacementHullShader.hlsl", "DisplacementDomainShader.hlsl"),
		nullptr,
		ResourceManager::instance()->loadShader("DisplacementDirectionalShadowVertexShader.hlsl", "DisplacementDirectionalShadowFragmentShader.hlsl", "DisplacementDirectionalShadowHullShader.hlsl", "DisplacementDirectionalShadowDomainShader.hlsl"),
		ResourceManager::instance()->loadShader("DisplacementOmniShadowVertexShader.hlsl", "DisplacementOmniShadowFragmentShader.hlsl", "DisplacementOmniShadowGeometryShader.hlsl", "DisplacementOmniShadowHullShader.hlsl", "DisplacementOmniShadowDomainShader.hlsl")
	)
{
}

void DisplacementMapping::render(std::shared_ptr<Shape>& pShape, bool)
{
	mNormalShader->useShader();
	pShape->render(true);
}

void DisplacementMapping::renderDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	mDirectionalShader->useShader();
	pShape->render(true);
}

void DisplacementMapping::renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	mOmniDirectionalShader->useShader();
	pShape->render(true);
}