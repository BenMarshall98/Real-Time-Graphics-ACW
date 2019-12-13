#include "ToonShading.h"
#include "ResourceManager.h"

ToonShading::ToonShading() :
	Technique(ResourceManager::instance()->loadShader(
		"ToonVertexShader.hlsl", "ToonFragmentShader.hlsl"), nullptr)
{
}

ToonShading::~ToonShading()
{
}

void ToonShading::render(std::shared_ptr<Object>& pObject, bool pDeferred)
{
	mShader->useShader();
	pObject->render();
}
