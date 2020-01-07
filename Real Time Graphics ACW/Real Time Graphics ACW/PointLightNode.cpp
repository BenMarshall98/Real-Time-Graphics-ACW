#include "PointLightNode.h"
#include "LightingManager.h"

PointLightNode::PointLightNode(const std::shared_ptr<PointLight> & pPointLight) : mPointLight(pPointLight)
{
	LightingManager::instance()->addPointLight(mPointLight);
}

PointLightNode::PointLightNode() : mPointLight(nullptr)
{
}

PointLightNode::~PointLightNode() = default;

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

void PointLightNode::update(const DirectX::XMFLOAT4X4 & pFullMatrix, const DirectX::XMFLOAT4X4 & pRotationMatrix)
{
	mPointLight->update(pFullMatrix);
}