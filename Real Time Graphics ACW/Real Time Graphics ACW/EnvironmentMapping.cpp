#include "EnvironmentMapping.h"
#include "ResourceManager.h"

EnvironmentMapping::EnvironmentMapping() :
	Technique(
		ResourceManager::instance()->loadShader("EnvironmentVertexShader.hlsl", "EnvironmentFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("EnvironmentDeferredVertexShader.hlsl", "EnvironmentDeferredFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl")
	), mSetupShader(ResourceManager::instance()->loadShader("EnvironmentSetupVertexShader.hlsl", "EnvironmentSetupFragmentShader.hlsl", "EnvironmentSetupGeometryShader.hlsl"))
{
}

void EnvironmentMapping::render(std::shared_ptr<Shape>& pShape, bool pDeffered, std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	//TODO: Implement
}

void EnvironmentMapping::renderDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	//TODO: Implement
}

void EnvironmentMapping::renderOmniDirectionalShadow(std::shared_ptr<Shape>& pShape)
{
	//TODO: Implement	
}

void EnvironmentMapping::renderPostprocessing(std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	//TODO: Implement
}