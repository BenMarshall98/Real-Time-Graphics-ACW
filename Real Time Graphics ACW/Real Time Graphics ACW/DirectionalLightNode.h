#pragma once

#include "SceneGraphNode.h"
#include "DirectionalLight.h"

class DirectionalLightNode final : public SceneGraphNode
{
	std::shared_ptr<DirectionalLight> mDirectionalLight;
	
public:
	explicit DirectionalLightNode(DirectionalLight * pDirectionalLight);
	DirectionalLightNode();
	~DirectionalLightNode();

	void read(std::istream & pIn) override;
	void update(DirectX::XMFLOAT4X4 pMatrix) override;
};

