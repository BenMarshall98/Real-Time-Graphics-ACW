#include "ShapeNode.h"
#include "ObjectManager.h"
#include <string>
#include "Sphere.h"
#include "Cuboid.h"
#include "Plane.h"
#include "RenderManager.h"

ShapeNode::ShapeNode(Shape * const pShape, const ObjectType pType) : mShape(pShape)
{
	if (pType == ObjectType::STATIC)
	{
		RenderManager::instance()->addStaticShape(mShape);
	}
	else
	{
		RenderManager::instance()->addDynamicShape(mShape);
	}
	
}

ShapeNode::~ShapeNode() = default;

void ShapeNode::read(std::istream& pIn)
{
	std::string s, type;
	pIn >> s >> type;

	std::string shape;
	pIn >> shape;

	if (shape == "Sphere")
	{
		mShape = std::make_shared<Sphere>();
	}
	else if (shape == "Cube")
	{
		mShape = std::make_shared<Cuboid>();
	}
	else if (shape == "Plane")
	{
		mShape = std::make_shared<Plane>();
	}

	pIn >> mShape;

	ObjectManager::instance()->addShape(mShape);

	if (type == "Static")
	{
		RenderManager::instance()->addStaticShape(mShape);
	}
	else
	{
		RenderManager::instance()->addDynamicShape(mShape);
	}
}

void ShapeNode::update(const DirectX::XMFLOAT4X4 & pFullMatrix, const DirectX::XMFLOAT4X4 & pRotationMatrix)
{
	mShape->setMatrix(pFullMatrix, pRotationMatrix);
}