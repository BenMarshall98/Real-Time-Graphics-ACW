#pragma once

#include "SceneGraphNode.h"
#include "PointLight.h"

class PointLightNode final : public SceneGraphNode
{
	std::shared_ptr<PointLight> mPointLight;
	
public:
	explicit PointLightNode(PointLight * pPointLight);
	PointLightNode();
	~PointLightNode();

	void read(std::istream& pIn) override;
	void update(DirectX::XMFLOAT4X4 pMatrix) override;
};

