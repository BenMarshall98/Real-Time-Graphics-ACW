#pragma once

#include "SceneGraphNode.h"
#include "Shape.h"

enum class ObjectType
{
	STATIC,
	DYNAMIC
};

class ShapeNode final : public SceneGraphNode
{
	std::shared_ptr<Shape> mShape = nullptr;
	
public:
	explicit ShapeNode(const std::shared_ptr<Shape> & pShape, ObjectType pType);
	ShapeNode() = default;
	~ShapeNode();

	ShapeNode(const ShapeNode &) = delete;
	ShapeNode(ShapeNode &&) = delete;
	ShapeNode & operator= (const ShapeNode &) = delete;
	ShapeNode & operator= (ShapeNode &&) = delete;
	
	void read(std::istream& pIn) override;
	void update(const DirectX::XMFLOAT4X4 & pFullMatrix, const DirectX::XMFLOAT4X4 & pRotationMatrix) override;
};
