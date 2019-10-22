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
	~LightingManager();

	void SetDirectionalLight(DirectionalLight * pDirectionalLight);
	void SetPointLight(PointLight * pPointLight);
	void AddSpotLight(SpotLight * pSpotLight);

	void Update();
};

