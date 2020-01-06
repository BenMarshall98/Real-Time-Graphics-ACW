#include "DirectionalLightNode.h"
#include "LightingManager.h"

DirectionalLightNode::DirectionalLightNode(DirectionalLight * const pDirectionalLight) :
	mDirectionalLight(pDirectionalLight)
{
	LightingManager::instance()->addDirectionalLight(mDirectionalLight);
}

DirectionalLightNode::~DirectionalLightNode() = default;

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

void DirectionalLightNode::update(const DirectX::XMFLOAT4X4 & pFullMatrix, const DirectX::XMFLOAT4X4 & pRotationMatrix)
{
	mDirectionalLight->update(pFullMatrix);
}