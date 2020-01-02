#include "DisplacementMapping.h"
#include "ResourceManager.h"

DisplacementMapping::DisplacementMapping() :
	Technique(
		ResourceManager::instance()->loadShader("DisplacementVertexShader.hlsl", "DisplacementFragmentShader.hlsl", "DisplacementHullShader.hlsl", "DisplacementDomainShader.hlsl"),
		ResourceManager::instance()->loadShader("DisplacementDeferredVertexShader.hlsl", "DisplacementDeferredFragmentShader.hlsl", "DisplacementDeferredHullShader.hlsl", "DisplacementDeferredDomainShader.hlsl"),
		ResourceManager::instance()->loadShader("DisplacementDirectionalShadowVertexShader.hlsl", "DisplacementDirectionalShadowFragmentShader.hlsl", "DisplacementDirectionalShadowHullShader.hlsl", "DisplacementDirectionalShadowDomainShader.hlsl"),
		ResourceManager::instance()->loadShader("DisplacementOmniShadowVertexShader.hlsl", "DisplacementOmniShadowFragmentShader.hlsl", "DisplacementOmniShadowGeometryShader.hlsl", "DisplacementOmniShadowHullShader.hlsl", "DisplacementOmniShadowDomainShader.hlsl")
	)
{
}

void DisplacementMapping::render(std::shared_ptr<Shape>& pShape, bool pDeferred, std::unique_ptr<Framebuffer> &)
{
	if (pDeferred)
	{
		mDeferredShader->useShader();
	}
	else
	{
		mNormalShader->useShader();
	}
	
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

bool DisplacementMapping::renderPostprocessing(std::unique_ptr<Framebuffer> &)
{
	return false;
}

void DisplacementMapping::renderTransparent(std::shared_ptr<Shape> &, std::unique_ptr<Framebuffer> &)
{
	//Do Nothing
}