#pragma once

#include "SceneGraphNode.h"
#include "Object.h"

class ObjectNode final : public SceneGraphNode
{
	std::shared_ptr<Object> mObject;
	
public:
	explicit ObjectNode(Object * pObject);
	ObjectNode();
	~ObjectNode();
	
	void read(std::istream& pIn) override;
	void update(DirectX::XMFLOAT4X4 pMatrix) override;
};
