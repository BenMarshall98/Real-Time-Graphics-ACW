#include "GourandShading.h"
#include "ResourceManager.h"

GourandShading::GourandShading() :
	Technique(ResourceManager::instance()->loadShader("GourandVertexShader.hlsl", "GourandFragmentShader.hlsl"), nullptr)
{
}

GourandShading::~GourandShading()
{
}

void GourandShading::render(std::shared_ptr<Object> & pObject, bool pDeferred)
{
	mShader->useShader();
	pObject->render();
}