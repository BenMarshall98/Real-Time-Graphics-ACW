#pragma once
#include <d3d11_1.h>
#include <vector>

#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"


class LightingManager
{
	ID3D11Buffer * mDirectionalLightBuffer = nullptr;
	ID3D11Buffer * mPointLightBuffer = nullptr;
	ID3D11Buffer * mSpotLightBuffer = nullptr;

	DirectionalLight * mDirectionalLight = nullptr;
	PointLight * mPointLight = nullptr;
	std::vector<SpotLight *> mSpotLights;

public:
	LightingManager();
	~LightingManager() = default;

	LightingManager(const LightingManager&) = delete;
	LightingManager(LightingManager &&) = delete;
	LightingManager & operator= (const LightingManager &) = delete;
	LightingManager & operator= (LightingManager &&) = delete;

	void setDirectionalLight(DirectionalLight * pDirectionalLight);
	void setPointLight(PointLight * pPointLight);
	void addSpotLight(SpotLight * pSpotLight);

	void update() const;
};

