#pragma once

#include "Shape.h"
#include "SceneGraphNode.h"
#include "Technique.h"
#include <map>
#include <vector>

struct GlobalBuffer
{
	float mTime;
	int mScreenMode;
	int mShadowMode;
	float mInkHeight;
};

class RenderManager final
{
	static RenderManager * mInstance;

	RenderManager();

	std::shared_ptr<Shader> mHDRShader;
	std::shared_ptr<Shader> mDeferredShader;

	std::shared_ptr<Model> mOutputModel;

	std::unique_ptr<Technique> mStaticTechnique;
	std::vector<std::unique_ptr<Technique>> mDynamicTechniques;

	std::vector<std::shared_ptr<Shape>> mStaticShapes;
	std::vector<std::shared_ptr<Shape>> mDynamicShapes;

	std::unique_ptr<Framebuffer> mHdrFramebuffer;
	std::unique_ptr<Framebuffer> mDeferredBuffer;

	std::map<std::unique_ptr<Technique>, std::shared_ptr<Shape>> mRenderTechnique;

	unsigned int mMode = 0u;

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

	void setup(float pCurrentTime);
	void render();
	void renderShadows();
	void renderToScreen();

	void addStaticShape(const std::shared_ptr<Shape>& pShape);
	void addDynamicShape(const std::shared_ptr<Shape>& pShape);

	void removeShape(const std::shared_ptr<Shape> & pShape);

	void changeMode();
};