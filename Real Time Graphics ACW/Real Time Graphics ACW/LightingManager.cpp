#include "LightingManager.h"
#include "DX11Render.h"

LightingManager::LightingManager()
{
	ID3D11Device * device = DX11Render::Instance()->GetDevice();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(DirectionalLightBuffer);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	device->CreateBuffer(&bufferDesc, nullptr, &mDirectionalLightBuffer);

	bufferDesc.ByteWidth = sizeof(PointLightBuffer);

	device->CreateBuffer(&bufferDesc, nullptr, &mPointLightBuffer);
	
	bufferDesc.ByteWidth = sizeof(SpotLightBuffer);

	device->CreateBuffer(&bufferDesc, nullptr, &mSpotLightBuffer);
}

LightingManager::~LightingManager()
{
}

void LightingManager::SetDirectionalLight(DirectionalLight* pDirectionalLight)
{
	delete mDirectionalLight;

	mDirectionalLight = pDirectionalLight;
}

void LightingManager::SetPointLight(PointLight* pPointLight)
{
	delete mPointLight;

	mPointLight = pPointLight;
}

void LightingManager::AddSpotLight(SpotLight* pSpotLight)
{
	if (mSpotLights.size() == 4)
	{
		delete mSpotLights[0];
		mSpotLights.erase(mSpotLights.begin());
	}

	mSpotLights.push_back(pSpotLight);
}

void LightingManager::Update()
{
	DirectionalLight::use(mDirectionalLight, mDirectionalLightBuffer);
	

	if (mPointLight)
	{
		//TODO
		//mPointLight->use(mPointLightBuffer);
	}

	SpotLightBuffer spotLightBuffer;
	
	for (auto i = 0u; i < mSpotLights.size(); i++)
	{
		//TODO
		//mSpotLights[i]->use(&spotLightBuffer, i);
	}

	ID3D11DeviceContext * deviceContext = DX11Render::Instance()->GetDeviceContext();
	
	deviceContext->UpdateSubresource(mSpotLightBuffer, 0, nullptr, &spotLightBuffer, 0, 0);
	deviceContext->PSSetConstantBuffers(2, 1, &mSpotLightBuffer);
}
