#include "GourandShading.h"
#include "ResourceManager.h"

GourandShading::GourandShading() :
	Technique(
		ResourceManager::instance()->loadShader("GourandVertexShader.hlsl", "GourandFragmentShader.hlsl"),
		nullptr,
		ResourceManager::instance()->loadShader("NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl")
	)
{
}

GourandShading::~GourandShading()
{
}

void GourandShading::render(std::shared_ptr<Object> & pObject, bool pDeferred)
{
	mNormalShader->useShader();
	pObject->render();
}

void GourandShading::renderDirectionalShadow(std::shared_ptr<Object>& pObject)
{
	mDirectionalShader->useShader();
	pObject->render();
}

void GourandShading::renderOmniDirectionalShadow(std::shared_ptr<Object>& pObject)
{
	mOmniDirectionalShader->useShader();
	pObject->render();
}
