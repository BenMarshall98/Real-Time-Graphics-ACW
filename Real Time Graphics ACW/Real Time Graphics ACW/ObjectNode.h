#pragma once

#include "SceneGraphNode.h"
#include "Shape.h"

enum ObjectType
{
	STATIC,
	DYNAMIC
};

class ObjectNode final : public SceneGraphNode
{
	std::shared_ptr<Shape> mShape;
	
public:
	explicit ObjectNode(Shape * pShape, ObjectType pType);
	ObjectNode();
	~ObjectNode() = default;

	ObjectNode(const ObjectNode &) = delete;
	ObjectNode(ObjectNode &&) = delete;
	ObjectNode & operator= (const ObjectNode &) = delete;
	ObjectNode & operator= (ObjectNode &&) = delete;
	
	void read(std::istream& pIn) override;
	void update(DirectX::XMFLOAT4X4 pMatrix) override;
};
