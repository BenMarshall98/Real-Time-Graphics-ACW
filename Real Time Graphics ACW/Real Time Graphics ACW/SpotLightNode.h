#pragma once

#include "SceneGraphNode.h"
#include "SpotLight.h"

class SpotLightNode final : public SceneGraphNode
{
	std::shared_ptr<SpotLight> mSpotLight;

public:
	explicit SpotLightNode(const std::shared_ptr<SpotLight> & pSpotLight);
	SpotLightNode();
	~SpotLightNode();

	SpotLightNode(const SpotLightNode&) = delete;
	SpotLightNode(SpotLightNode &&) = delete;
	SpotLightNode & operator= (const SpotLightNode &) = delete;
	SpotLightNode & operator= (SpotLightNode &&) = delete;
	
	void read(std::istream &pIn) override;
	void update(const DirectX::XMFLOAT4X4 & pFullMatrix, const DirectX::XMFLOAT4X4 & pRotationMatrix) override;
};