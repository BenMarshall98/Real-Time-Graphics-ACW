#pragma once

#include "Object.h"
#include "SceneGraphNode.h"
#include "Technique.h"
#include <map>
#include <vector>

class ObjectManager final
{
	static ObjectManager * mInstance;
	
	ObjectManager();
	
	std::unique_ptr<Technique> staticTechnique;
	std::vector<std::unique_ptr<Technique>> dynamicTechniques;
	
	std::vector<std::shared_ptr<Object>> staticObjects;
	std::vector<std::shared_ptr<Object>> dynamicObjects;
	
	std::map<std::unique_ptr<Technique>, std::shared_ptr<Object>> renderTechnique;
	
public:
	~ObjectManager();
	
	static ObjectManager * instance()
	{
		if (!mInstance)
		{
			mInstance = new ObjectManager();
		}
		return mInstance;
	}
	
	void render();
	
	void addStaticObject(std::shared_ptr<Object> pObject);
	void addDynamicObject(std::shared_ptr<Object> pObject);
};