#include "ShapeNode.h"
#include "ObjectManager.h"
#include <string>
#include "Sphere.h"
#include "Cuboid.h"
#include "Plane.h"

ShapeNode::ShapeNode(Shape * pShape, const ObjectType pType) : mShape(pShape)
{
	if (pType == STATIC)
	{
		ObjectManager::instance()->addStaticShape(mShape);
	}
	else
	{
		ObjectManager::instance()->addDynamicShape(mShape);
	}
	
}

ShapeNode::ShapeNode() : mShape(nullptr)
{
}

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

	if (type == "Static")
	{
		ObjectManager::instance()->addStaticShape(mShape);
	}
	else
	{
		ObjectManager::instance()->addDynamicShape(mShape);
	}
}

void ShapeNode::update(const DirectX::XMFLOAT4X4 pMatrix)
{
	mShape->setMatrix(pMatrix);
}