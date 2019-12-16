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

void DisplacementMapping::render(std::shared_ptr<Shape>& pShape, bool pDeferred)
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