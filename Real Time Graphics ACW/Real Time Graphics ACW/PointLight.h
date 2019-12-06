#pragma once
#include <directxmath.h>
#include <d3d11_1.h>

struct PointLightBuffer
{
	DirectX::XMFLOAT4 mColor;
	DirectX::XMFLOAT4 mPosition;
	float mAttenuationConstant;
	float mAttenuationLinear;
	float mAttenuationQuad;
	int mIsUsed;
};

class PointLight
{
	DirectX::XMFLOAT4 mColor;
	DirectX::XMFLOAT3 mPosition;
	bool mDirty;
	
public:
	PointLight(DirectX::XMFLOAT4 pColor, DirectX::XMFLOAT3 pDirection);
	~PointLight() = default;

	void use(ID3D11Buffer * pDeviceBuffer);
};

