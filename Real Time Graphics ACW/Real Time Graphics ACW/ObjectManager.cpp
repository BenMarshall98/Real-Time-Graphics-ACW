#include "ObjectManager.h"
#include "IExplode.h"
#include "Sphere.h"

ObjectManager * ObjectManager::mInstance = nullptr;

void ObjectManager::addShape(const std::shared_ptr<Shape>& pShape)
{
	mShapes.push_back(pShape);
	
	if (auto ptr = std::dynamic_pointer_cast<IExplode>(pShape))
	{
		auto key = mExplodingShapes.size() + 1;
		mExplodingShapes.insert(std::pair<unsigned int, std::shared_ptr<Shape>>(key, pShape));
	}
}

void ObjectManager::explode(unsigned pKey)
{
	const auto it = mExplodingShapes.find(pKey);

	if(it != mExplodingShapes.end())
	{
		const auto shape = it->second;

		//TODO: Make explode
	}
}
