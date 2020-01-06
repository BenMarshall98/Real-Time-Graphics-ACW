#pragma once

#include "SceneGraphNode.h"
#include "DirectionalLight.h"

class DirectionalLightNode final : public SceneGraphNode
{
	std::shared_ptr<DirectionalLight> mDirectionalLight;
	
public:
	explicit DirectionalLightNode(DirectionalLight * pDirectionalLight);
	DirectionalLightNode() = default;
	~DirectionalLightNode();

	DirectionalLightNode(const DirectionalLightNode &) = delete;
	DirectionalLightNode(DirectionalLightNode &&) = delete;
	DirectionalLightNode & operator= (const DirectionalLightNode &) = delete;
	DirectionalLightNode & operator= (DirectionalLightNode &&) = delete;

	void read(std::istream & pIn) override;
	void update(const DirectX::XMFLOAT4X4 & pFullMatrix, const DirectX::XMFLOAT4X4 & pRotationMatrix) override;
};

