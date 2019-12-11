#include "DisplacementMapping.h"
#include "ResourceManager.h"

DisplacementMapping::DisplacementMapping() :
	Technique(ResourceManager::instance()->loadShader("DisplacementVertexShader.hlsl", "DisplacementFragmentShader.hlsl", 
		"DisplacementHullShader.hlsl", "DisplacementDomainShader.hlsl"), nullptr)
{
}

DisplacementMapping::~DisplacementMapping()
{
}

void DisplacementMapping::render(std::shared_ptr<Object> & pObject, bool pDeferred)
{
	mShader->useShader();
	pObject->render(true);
}
