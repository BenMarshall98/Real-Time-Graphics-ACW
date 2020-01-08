#include "EnvironmentMapping.h"
#include "ResourceManager.h"

EnvironmentMapping::EnvironmentMapping()
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

	ResourceManager::instance()->loadShader(normalShader,"EnvironmentVertexShader.hlsl", "EnvironmentFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(deferredShader, "EnvironmentDeferredVertexShader.hlsl", "EnvironmentDeferredFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(directionalShader, "NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(omniDirectionalShader, "NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl");
	ResourceManager::instance()->loadShader(mSetupShader, "EnvironmentSetupVertexShader.hlsl", "EnvironmentSetupFragmentShader.hlsl", "EnvironmentSetupGeometryShader.hlsl");
	ResourceManager::instance()->loadShader(directionalSimpleShader, "NormalDirectionalSimpleShadowVertexShader.hlsl", "NormalDirectionalSimpleShadowFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(pointSimpleShader, "NormalPointSimpleShadowVertexShader.hlsl", "NormalPointSimpleShadowFragmentShader.hlsl");

	setNormalShader(normalShader);
	setDeferredShader(deferredShader);
	setDirectionalShader(directionalShader);
	setOmniDirectionalShader(omniDirectionalShader);
	setDirectionalSimpleShader(directionalSimpleShader);
	setPointSimpleShader(pointSimpleShader);
}

EnvironmentMapping::~EnvironmentMapping() = default;

void EnvironmentMapping::render(const std::shared_ptr<Shape>& pShape, const bool pDeffered, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	//TODO: Implement
}

void EnvironmentMapping::renderDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	//TODO: Implement
}

void EnvironmentMapping::renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	//TODO: Implement	
}

void EnvironmentMapping::renderDirectionalSimpleShadow(const std::shared_ptr<Shape> & pShape)
{
	useDirectionalSimpleShader();
	pShape->render();
}

void EnvironmentMapping::renderPointSimpleShadow(const std::shared_ptr<Shape> & pShape)
{
	usePointSimpleShader();
	pShape->render();
}

bool EnvironmentMapping::renderPostprocessing(const std::unique_ptr<Framebuffer> &)
{
	return false;
}

void EnvironmentMapping::renderTransparent(const std::shared_ptr<Shape> &, const std::unique_ptr<Framebuffer> &)
{
	//Do Nothing
}