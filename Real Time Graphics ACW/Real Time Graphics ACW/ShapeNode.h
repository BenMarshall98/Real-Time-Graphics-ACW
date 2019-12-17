#pragma once

#include "SceneGraphNode.h"
#include "Shape.h"

enum ObjectType
{
	STATIC,
	DYNAMIC
};

class ShapeNode final : public SceneGraphNode
{
	std::shared_ptr<Shape> mShape;
	
public:
	explicit ShapeNode(Shape * pShape, ObjectType pType);
	ShapeNode();
	~ShapeNode() = default;

	ShapeNode(const ShapeNode &) = delete;
	ShapeNode(ShapeNode &&) = delete;
	ShapeNode & operator= (const ShapeNode &) = delete;
	ShapeNode & operator= (ShapeNode &&) = delete;
	
	void read(std::istream& pIn) override;
	void update(DirectX::XMFLOAT4X4 pMatrix) override;
};
