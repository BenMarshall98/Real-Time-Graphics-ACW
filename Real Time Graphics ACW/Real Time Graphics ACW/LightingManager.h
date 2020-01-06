#pragma once
#include <d3d11_1.h>
#include <vector>

#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include <memory>

class LightingManager
{
	std::shared_ptr<DirectionalLight> mDirectionalLight = nullptr;
	std::shared_ptr<PointLight> mPointLight = nullptr;
	std::vector<std::shared_ptr<SpotLight>> mSpotLights;

	static LightingManager * mInstance;
	
	LightingManager() = default;

public:

	static LightingManager * instance()
	{
		if (!mInstance)
		{
			mInstance = new LightingManager();
		}

		return mInstance;
	}
	
	~LightingManager() = default;

	LightingManager(const LightingManager&) = delete;
	LightingManager(LightingManager &&) = delete;
	LightingManager & operator= (const LightingManager &) = delete;
	LightingManager & operator= (LightingManager &&) = delete;

	void addDirectionalLight(const std::shared_ptr<DirectionalLight> & pDirectionalLight)
	{
		mDirectionalLight = pDirectionalLight;
	}
	
	void addPointLight(const std::shared_ptr<PointLight> & pPointLight)
	{
		mPointLight = pPointLight;
	}
	
	void addSpotLight(const std::shared_ptr<SpotLight> & pSpotLight);

	void removeDirectionalLight(const std::shared_ptr<DirectionalLight> & pDirectionalLight);
	void removePointLight(const std::shared_ptr<PointLight> & pPointLight);
	void removeSpotLight(const std::shared_ptr<SpotLight> & pSpotLight);

	bool updateDirectionalLightShadow() const;
	bool updatePointLightShadow() const;
	void updateSpotLightShadow(unsigned int pLight) const;

	void useShadowTextures() const;
	
	unsigned int getNumberOfSpotLights() const
	{
		return mSpotLights.size();
	}

	void update() const;
};

