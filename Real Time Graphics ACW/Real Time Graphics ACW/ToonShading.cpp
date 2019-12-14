#include "ToonShading.h"
#include "ResourceManager.h"

ToonShading::ToonShading() :
	Technique(
		ResourceManager::instance()->loadShader("ToonVertexShader.hlsl", "ToonFragmentShader.hlsl"),
		nullptr,
		ResourceManager::instance()->loadShader("NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl"),
		ResourceManager::instance()->loadShader("NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl")
	)
{
}

ToonShading::~ToonShading()
{
}

void ToonShading::render(std::shared_ptr<Object>& pObject, bool pDeferred)
{
	mNormalShader->useShader();
	pObject->render();
}

void ToonShading::renderDirectionalShadow(std::shared_ptr<Object>& pObject)
{
	mDirectionalShader->useShader();
	pObject->render();
}

void ToonShading::renderOmniDirectionalShadow(std::shared_ptr<Object>& pObject)
{
	mOmniDirectionalShader->useShader();
	pObject->render();
}
