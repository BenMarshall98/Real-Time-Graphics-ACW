#pragma once
#include <DirectXMath.h>
#include <d3d11_1.h>

struct DirectionalLightBuffer
{
	DirectX::XMFLOAT4 mColor;
	DirectX::XMFLOAT3 mDirection;
	int mIsUsed;
};

class DirectionalLight
{
	DirectX::XMFLOAT4 mColor;
	DirectX::XMFLOAT3 mDirection;
	bool mDirty;
	
public:
	DirectionalLight(const DirectX::XMFLOAT4 & pColor, const DirectX::XMFLOAT3 & pDirection);
	~DirectionalLight() = default;
	DirectionalLight(const DirectionalLight&) = delete;
	DirectionalLight(DirectionalLight &&) = delete;
	DirectionalLight & operator= (const DirectionalLight &) = delete;
	DirectionalLight & operator= (DirectionalLight &&) = delete;

	static void use(DirectionalLight * pDirectionalLight, ID3D11Buffer * pDeviceBuffer);
};