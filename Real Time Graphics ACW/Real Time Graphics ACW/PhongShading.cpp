#include "PhongShading.h"
#include "ResourceManager.h"

PhongShading::PhongShading()
{
	std::shared_ptr<Shader> normalShader;
	std::shared_ptr<Shader> deferredShader;
	std::shared_ptr<Shader> directionalShader;
	std::shared_ptr<Shader> omniDirectionalShader;
	std::shared_ptr<Shader> directionalSimpleShader;
	std::shared_ptr<Shader> pointSimpleShader;

	getNormalShader(normalShader);
	getDeferredShader(deferredShader);
	getDirectionalShader(directionalShader);
	getOmniDirectionalShader(omniDirectionalShader);
	getDirectionalSimpleShader(directionalSimpleShader);
	getPointSimpleShader(pointSimpleShader);

	ResourceManager::instance()->loadShader(normalShader, "PhongVertexShader.hlsl", "PhongFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(deferredShader, "PhongDeferredVertexShader.hlsl", "PhongDeferredFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(directionalShader, "NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(omniDirectionalShader, "NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl");
	ResourceManager::instance()->loadShader(directionalSimpleShader, "NormalDirectionalSimpleShadowVertexShader.hlsl", "NormalDirectionalSimpleShadowFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(pointSimpleShader, "NormalPointSimpleShadowVertexShader.hlsl", "NormalPointSimpleShadowFragmentShader.hlsl");

	setNormalShader(normalShader);
	setDeferredShader(deferredShader);
	setDirectionalShader(directionalShader);
	setOmniDirectionalShader(omniDirectionalShader);
	setDirectionalSimpleShader(directionalSimpleShader);
	setPointSimpleShader(pointSimpleShader);
}

PhongShading::~PhongShading() = default;

void PhongShading::render(const std::shared_ptr<Shape>& pShape, const bool pDeferred, const std::unique_ptr<Framebuffer> &)
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

void PhongShading::renderDirectionalSimpleShadow(const std::shared_ptr<Shape> & pShape)
{
	useDirectionalSimpleShader();
	pShape->render();
}

void PhongShading::renderPointSimpleShadow(const std::shared_ptr<Shape> & pShape)
{
	usePointSimpleShader();
	pShape->render();
}

bool PhongShading::renderPostprocessing(const std::unique_ptr<Framebuffer> &)
{
	return false;
}

void PhongShading::renderTransparent(const std::shared_ptr<Shape> &, const std::unique_ptr<Framebuffer> &)
{
	//Do Nothing
}