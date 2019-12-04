#include "ObjectNode.h"
#include "ObjectManager.h"

ObjectNode::ObjectNode(Object * pObject) : mObject(pObject)
{
}

ObjectNode::ObjectNode() : mObject(nullptr)
{
}

ObjectNode::~ObjectNode()
{
}

void ObjectNode::read(std::istream& pIn)
{
	mObject = std::make_shared<Object>();
	pIn >> *mObject;

	ObjectManager::instance()->addDynamicObject(mObject);
}

void ObjectNode::update(DirectX::XMFLOAT4X4 pMatrix)
{
	mObject->setMatrix(pMatrix);
}

void ObjectNode::render()
{
	mObject->render();
}