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

void PhongShading::render(const std::shared_ptr<Shape>& pShape, bool pDeferred, std::unique_ptr<Framebuffer> &)
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

void PhongShading::renderDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useDirectionalShader();
	pShape->render();
}

void PhongShading::renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useOmniDirectionalShader();
	pShape->render();
}

bool PhongShading::renderPostprocessing(std::unique_ptr<Framebuffer> &)
{
	return false;
}

void PhongShading::renderTransparent(std::shared_ptr<Shape> &, std::unique_ptr<Framebuffer> &)
{
	//Do Nothing
}