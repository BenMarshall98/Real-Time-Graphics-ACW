#include "DirectionalLightNode.h"
#include "LightingManager.h"

DirectionalLightNode::DirectionalLightNode(DirectionalLight * pDirectionalLight) :
	mDirectionalLight(pDirectionalLight)
{
	LightingManager::instance()->addDirectionalLight(mDirectionalLight);
}

DirectionalLightNode::DirectionalLightNode() : mDirectionalLight(nullptr)
{
}

DirectionalLightNode::~DirectionalLightNode()
{
}

void DirectionalLightNode::read(std::istream& pIn)
{
	if (mDirectionalLight)
	{
		LightingManager::instance()->removeDirectionalLight(mDirectionalLight);
	}

	mDirectionalLight.reset(new DirectionalLight());
	pIn >> *mDirectionalLight;

	LightingManager::instance()->addDirectionalLight(mDirectionalLight);
}

void DirectionalLightNode::update(DirectX::XMFLOAT4X4 pMatrix)
{
	mDirectionalLight->update(pMatrix);
}