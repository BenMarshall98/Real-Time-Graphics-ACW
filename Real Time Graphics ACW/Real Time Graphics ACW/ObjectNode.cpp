#include "ObjectNode.h"

ObjectNode::ObjectNode(Object * pObject) : mObject(pObject)
{
}

ObjectNode::ObjectNode() : mObject(nullptr)
{
}

ObjectNode::~ObjectNode()
{
	delete mObject;
}

void ObjectNode::read(std::istream& pIn)
{
	mObject = new Object();
	pIn >> *mObject;
}

void ObjectNode::update(DirectX::XMFLOAT4X4 pMatrix)
{
	mObject->setMatrix(pMatrix);
}

void ObjectNode::render()
{
	mObject->render();
}