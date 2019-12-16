#pragma once

#include "Shape.h"
#include "SceneGraphNode.h"
#include "Technique.h"
#include <map>
#include <vector>

class ObjectManager final
{
	static ObjectManager * mInstance;
	
	ObjectManager();
	
	std::unique_ptr<Technique> mStaticTechnique;
	std::vector<std::unique_ptr<Technique>> mDynamicTechniques;
	
	std::vector<std::shared_ptr<Shape>> mStaticShapes;
	std::vector<std::shared_ptr<Shape>> mDynamicShapes;
	
	std::map<std::unique_ptr<Technique>, std::shared_ptr<Shape>> mRenderTechnique;
	
public:
	~ObjectManager() = default;
	ObjectManager(const ObjectManager&) = delete;
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
	
	void render();
	void renderShadows();
	
	void addStaticShape(const std::shared_ptr<Shape>& pShape);
	void addDynamicShape(const std::shared_ptr<Shape>& pShape);
};