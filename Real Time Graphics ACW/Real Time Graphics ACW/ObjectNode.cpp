#include "ObjectNode.h"
#include "ObjectManager.h"
#include <string>

ObjectNode::ObjectNode(Shape * pShape, const ObjectType pType) : mShape(pShape)
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

ObjectNode::ObjectNode() : mShape(nullptr)
{
}

void ObjectNode::read(std::istream& pIn)
{
	std::string s, type;
	pIn >> s >> type;
	mShape = std::make_shared<Shape>();
	pIn >> *mShape;

	if (type == "Static")
	{
		ObjectManager::instance()->addStaticShape(mShape);
	}
	else
	{
		ObjectManager::instance()->addDynamicShape(mShape);
	}
}

void ObjectNode::update(const DirectX::XMFLOAT4X4 pMatrix)
{
	mShape->setMatrix(pMatrix);
}