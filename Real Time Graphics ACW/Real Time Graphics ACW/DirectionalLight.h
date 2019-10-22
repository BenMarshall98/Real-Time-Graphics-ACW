#pragma once
#include <DirectXMath.h>
#include <d3d11_1.h>

struct DirectionalLightBuffer
{
	DirectX::XMFLOAT4 mColor;
	DirectX::XMFLOAT3 mDirection;
	int isUsed;
};

class DirectionalLight
{
	DirectX::XMFLOAT4 mColor;
	DirectX::XMFLOAT3 mDirection;
	bool mDirty;
	
public:
	DirectionalLight(DirectX::XMFLOAT4 pColor, DirectX::XMFLOAT3 pDirection);
	~DirectionalLight();

	static void use(DirectionalLight * pDirectionalLight, ID3D11Buffer * pDeviceBuffer);
};