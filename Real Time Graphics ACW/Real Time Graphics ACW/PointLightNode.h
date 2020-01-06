#pragma once

#include "SceneGraphNode.h"
#include "PointLight.h"

class PointLightNode final : public SceneGraphNode
{
	std::shared_ptr<PointLight> mPointLight;
	
public:
	explicit PointLightNode(PointLight * pPointLight);
	PointLightNode();
	~PointLightNode() = default;

	PointLightNode(const PointLightNode &) = delete;
	PointLightNode(PointLightNode &&) = delete;
	PointLightNode & operator= (const PointLightNode &) = delete;
	PointLightNode & operator= (PointLightNode &&) = delete;

	void read(std::istream& pIn) override;
	void update(const DirectX::XMFLOAT4X4 & pFullMatrix, DirectX::XMFLOAT4X4 & pRotationMatrix) override;
};

