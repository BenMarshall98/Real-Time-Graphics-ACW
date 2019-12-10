#include "BumpMapping.h"
#include "ResourceManager.h"

BumpMapping::BumpMapping() :
	Technique(ResourceManager::instance()->loadShader(
		"BumpVertexShader.hlsl", "BumpFragmentShader.hlsl"
	), nullptr)
{
}

BumpMapping::~BumpMapping()
{
}

void BumpMapping::render(std::shared_ptr<Object>& pObject, bool pDeferred)
{
	mShader->useShader();
	pObject->render();
}
