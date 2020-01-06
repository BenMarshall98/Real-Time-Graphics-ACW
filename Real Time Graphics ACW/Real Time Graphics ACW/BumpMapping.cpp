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

BumpMapping::~BumpMapping() = default;

void BumpMapping::render(const std::shared_ptr<Shape> & pShape, const bool pDeferred, std::unique_ptr<Framebuffer> &)
{
	if (pDeferred)
	{
		useDeferredShader();
	}
	else
	{
		useNormalShader();
	}
	
	pShape->render();
}

void BumpMapping::renderDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useDirectionalShader();
	pShape->render();
}

void BumpMapping::renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useOmniDirectionalShader();
	pShape->render();
}

bool BumpMapping::renderPostprocessing(std::unique_ptr<Framebuffer> &)
{
	return false;
}

void BumpMapping::renderTransparent(std::shared_ptr<Shape> &, std::unique_ptr<Framebuffer> &)
{
	//Do Nothing
}