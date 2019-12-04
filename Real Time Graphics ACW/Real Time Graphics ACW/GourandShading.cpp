#include "GourandShading.h"
#include "ResourceManager.h"

GourandShading::GourandShading() : Technique(ResourceManager::instance()->loadShader("VertexShader.hlsl", "PixelShader.hlsl"), nullptr)
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