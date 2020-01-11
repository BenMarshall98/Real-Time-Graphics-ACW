#include "ObjectManager.h"
#include "Sphere.h"
#include "RenderManager.h"

ObjectManager * ObjectManager::mInstance = nullptr;

void ObjectManager::addShape(const std::shared_ptr<Shape>& pShape)
{
	mShapes.push_back(pShape);
	
	if (pShape->getType() == Type::EXPLODE)
	{
		auto key = mExplodingShapes.size() + 1;
		mExplodingShapes.insert(std::pair<unsigned int, std::shared_ptr<Shape>>(key, pShape));
	}
}

void ObjectManager::explode(const unsigned int pKey)
{
	const auto eIt = mExplodingShapes.find(pKey);

	if(eIt != mExplodingShapes.end())
	{
		const auto shape = eIt->second;

		if (shape->getType() == Type::EXPLODE)
		{
			shape->explode();
		}

		RenderManager::instance()->removeShape(shape);

		const auto sIt = std::find(mShapes.begin(), mShapes.end(), shape);

		if (sIt != mShapes.end())
		{
			mShapes.erase(sIt);
		}

		mExplodingShapes.erase(eIt);
	}
}
