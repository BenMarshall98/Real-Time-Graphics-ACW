#include "BumpMapping.h"
#include "ResourceManager.h"

BumpMapping::BumpMapping() :
	Technique(
		ResourceManager::instance()->loadShader("BumpVertexShader.hlsl", "BumpFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("BumpDeferredVertexShader.hlsl", "BumpDeferredFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl")
	)
{
}

void BumpMapping::render(std::shared_ptr<Shape>& pShape, bool pDeferred, std::unique_ptr<Framebuffer> &)
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

void BumpMapping::renderDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	mDirectionalShader->useShader();
	pShape->render();
}

void BumpMapping::renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	mOmniDirectionalShader->useShader();
	pShape->render();
}

void BumpMapping::renderPostprocessing(std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	//TODO: Implement
}