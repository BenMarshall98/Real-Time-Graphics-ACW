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

	void addDirectionalLight(std::shared_ptr<DirectionalLight> & pDirectionalLight);
	void addPointLight(std::shared_ptr<PointLight> & pPointLight);
	void addSpotLight(std::shared_ptr<SpotLight> & pSpotLight);

	void removeDirectionalLight(std::shared_ptr<DirectionalLight> & pDirectionalLight);
	void removePointLight(std::shared_ptr<PointLight> & pPointLight);
	void removeSpotLight(std::shared_ptr<SpotLight> & pSpotLight);

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

