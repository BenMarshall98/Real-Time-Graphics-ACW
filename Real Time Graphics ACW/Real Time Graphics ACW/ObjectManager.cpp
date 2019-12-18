#include "ObjectManager.h"
#include "IExplode.h"
#include "Sphere.h"
#include "RenderManager.h"

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
	const auto eIt = mExplodingShapes.find(pKey);

	if(eIt != mExplodingShapes.end())
	{
		const auto shape = eIt->second;

		if (const auto ptr = std::dynamic_pointer_cast<IExplode>(shape))
		{
			ptr->explode;
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
