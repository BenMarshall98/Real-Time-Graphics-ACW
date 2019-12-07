#pragma once

#include "SceneGraphNode.h"
#include "SpotLight.h"

class SpotLightNode final : public SceneGraphNode
{
	std::shared_ptr<SpotLight> mSpotLight;

public:
	explicit SpotLightNode(SpotLight * pSpotLight);
	SpotLightNode();
	~SpotLightNode();
	
	void read(std::istream &pIn) override;
	void update(DirectX::XMFLOAT4X4 pMatrix) override;
};