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
