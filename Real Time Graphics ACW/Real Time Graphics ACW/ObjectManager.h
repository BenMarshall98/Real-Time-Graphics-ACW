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
	std::unique_ptr<SceneGraphNode> node;
	
	std::vector<std::shared_ptr<Object>> staticObjects;
	std::vector<std::shared_ptr<Object>> dynamicObjects;
	
	std::map<std::unique_ptr<Technique>, std::shared_ptr<Object>> renderTechnique;
	
public:
	~ObjectManager();
	
	static ObjectManager * instance()
	{
		if (!mInstance)
		{
			mInstance = new Instance();
		}
		return instance;
	}
	
	void update();
	void render();
	
	void addStaticObject(std::shared_ptr<Object> pObject);
	void addDynamicObject(std::shared_ptr<Object> pObject);
};