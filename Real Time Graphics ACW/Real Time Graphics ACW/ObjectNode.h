#pragma once

#include "SceneGraphNode.h"
#include "Object.h"

enum ObjectType
{
	Static,
	Dynamic
};

class ObjectNode final : public SceneGraphNode
{
	std::shared_ptr<Object> mObject;
	
public:
	explicit ObjectNode(Object * pObject, ObjectType pType);
	ObjectNode();
	~ObjectNode();
	
	void read(std::istream& pIn) override;
	void update(DirectX::XMFLOAT4X4 pMatrix) override;
};
