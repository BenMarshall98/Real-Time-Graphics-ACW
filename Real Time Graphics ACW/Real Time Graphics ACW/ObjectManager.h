#pragma once
#include "Shape.h"
#include <map>
#include <vector>

class ObjectManager final
{
	static ObjectManager * mInstance;

	ObjectManager() = default;

	std::vector<std::shared_ptr<Shape>> mShapes;

	std::map<unsigned int, std::shared_ptr<Shape>> mExplodingShapes;

public:
	~ObjectManager()
	{
		mInstance = nullptr;
	}
	ObjectManager(const ObjectManager &) = delete;
	ObjectManager(ObjectManager &&) = delete;
	ObjectManager & operator= (const ObjectManager &) = delete;
	ObjectManager & operator= (ObjectManager &&) = delete;

	static ObjectManager * instance()
	{
		if (!mInstance)
		{
			mInstance = new ObjectManager();
		}
		return mInstance;
	}

	void addShape(const std::shared_ptr<Shape> & pShape);
	void explode(unsigned int pKey);

	void getAllShapes(std::vector<std::shared_ptr<Shape>> & pShapes)
	{
		pShapes = mShapes;
	}
};
