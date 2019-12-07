#include "PointLightNode.h"
#include "LightingManager.h"

PointLightNode::PointLightNode(PointLight * pPointLight) : mPointLight(pPointLight)
{
	LightingManager::instance()->addPointLight(mPointLight);
}

PointLightNode::PointLightNode() : mPointLight(nullptr)
{
}

PointLightNode::~PointLightNode()
{
}

void PointLightNode::read(std::istream& pIn)
{
	if (mPointLight)
	{
		LightingManager::instance()->removePointLight(mPointLight);
	}

	mPointLight.reset(new PointLight());
	pIn >> *mPointLight;

	LightingManager::instance()->addPointLight(mPointLight);
}

void PointLightNode::update(DirectX::XMFLOAT4X4 pMatrix)
{
	mPointLight->update(pMatrix);
}