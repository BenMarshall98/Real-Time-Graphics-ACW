#pragma once

#include "SceneGraphNode.h"
#include "SpotLight.h"

class SpotLightNode final : public SceneGraphNode
{
	std::shared_ptr<SpotLight> mSpotLight;

public:
	explicit SpotLightNode(SpotLight * pSpotLight);
	SpotLightNode();
	~SpotLightNode() = default;

	SpotLightNode(const SpotLightNode&) = delete;
	SpotLightNode(SpotLightNode &&) = delete;
	SpotLightNode & operator= (const SpotLightNode &) = delete;
	SpotLightNode & operator= (SpotLightNode &&) = delete;
	
	void read(std::istream &pIn) override;
	void update(DirectX::XMFLOAT4X4 pMatrix) override;
};