#pragma once

#include "Shape.h"
#include "SceneGraphNode.h"
#include "Technique.h"
#include <map>
#include <vector>

class RenderManager final
{
	static RenderManager * mInstance;

	RenderManager();

	std::unique_ptr<Technique> mStaticTechnique;
	std::vector<std::unique_ptr<Technique>> mDynamicTechniques;

	std::vector<std::shared_ptr<Shape>> mStaticShapes;
	std::vector<std::shared_ptr<Shape>> mDynamicShapes;

	std::map<std::unique_ptr<Technique>, std::shared_ptr<Shape>> mRenderTechnique;

public:
	~RenderManager() = default;
	RenderManager(const RenderManager&) = delete;
	RenderManager(RenderManager &&) = delete;
	RenderManager & operator= (const RenderManager &) = delete;
	RenderManager & operator= (RenderManager &&) = delete;

	static RenderManager * instance()
	{
		if (!mInstance)
		{
			mInstance = new RenderManager();
		}
		return mInstance;
	}

	void render();
	void renderShadows();

	void addStaticShape(const std::shared_ptr<Shape>& pShape);
	void addDynamicShape(const std::shared_ptr<Shape>& pShape);
};