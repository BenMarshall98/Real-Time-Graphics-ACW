#include "SpotLightNode.h"
#include "LightingManager.h"

SpotLightNode::SpotLightNode(SpotLight* pSpotLight) : mSpotLight(pSpotLight)
{
	LightingManager::instance()->addSpotLight(mSpotLight);
}

SpotLightNode::SpotLightNode() : mSpotLight(nullptr)
{
}

SpotLightNode::~SpotLightNode()
{
}

void SpotLightNode::read(std::istream& pIn)
{
	if (mSpotLight)
	{
		LightingManager::instance()->removeSpotLight(mSpotLight);
	}
	
	mSpotLight.reset(new SpotLight());
	pIn >> *mSpotLight;

	LightingManager::instance()->addSpotLight(mSpotLight);
}

void SpotLightNode::update(DirectX::XMFLOAT4X4 pMatrix)
{
	mSpotLight->update(pMatrix);
}
