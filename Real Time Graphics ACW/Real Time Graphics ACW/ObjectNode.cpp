#include "ObjectNode.h"
#include "ObjectManager.h"
#include <string>

ObjectNode::ObjectNode(Object * pObject, ObjectType pType) : mObject(pObject)
{
	if (pType == Static)
	{
		ObjectManager::instance()->addStaticObject(mObject);
	}
	else
	{
		ObjectManager::instance()->addDynamicObject(mObject);
	}
	
}

ObjectNode::ObjectNode() : mObject(nullptr)
{
}

ObjectNode::~ObjectNode()
{
}

void ObjectNode::read(std::istream& pIn)
{
	std::string s, type;
	pIn >> s >> type;
	mObject = std::make_shared<Object>();
	pIn >> *mObject;

	if (type == "Static")
	{
		ObjectManager::instance()->addStaticObject(mObject);
	}
	else
	{
		ObjectManager::instance()->addDynamicObject(mObject);
	}
}

void ObjectNode::update(DirectX::XMFLOAT4X4 pMatrix)
{
	mObject->setMatrix(pMatrix);
}