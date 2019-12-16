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
	
	std::unique_ptr<Technique> staticTechnique;
	std::vector<std::unique_ptr<Technique>> dynamicTechniques;
	
	std::vector<std::shared_ptr<Shape>> staticShapes;
	std::vector<std::shared_ptr<Shape>> dynamicShapes;
	
	std::map<std::unique_ptr<Technique>, std::shared_ptr<Shape>> renderTechnique;
	
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
	void renderShadows();
	
	void addStaticShape(std::shared_ptr<Shape> pShape);
	void addDynamicShape(std::shared_ptr<Shape> pShape);
};