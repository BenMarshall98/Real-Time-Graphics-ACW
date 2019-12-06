#pragma once
#include <directxmath.h>

struct SpotLightBuffer
{
	DirectX::XMFLOAT4 mColor[4];
	DirectX::XMFLOAT4 mPosition[4];
	DirectX::XMFLOAT4 mDirection[4];
	float mInnerAngle[4];
	float mOuterAngle[4];
	float mAttenuationConstant[4];
	float mAttenuationLinear[4];
	float mAttenuationQuad[4];
	int mIsUsed[4];
};

class SpotLight
{
	DirectX::XMFLOAT4 mColor;
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mDirection;
	float mInnerAngle;
	float mOuterAngle;
	bool mDirty;
	
public:
	SpotLight();
	~SpotLight() = default;

	SpotLight(const SpotLight &) = delete;
	SpotLight(SpotLight &&) = delete;
	SpotLight & operator= (const SpotLight &) = delete;
	SpotLight & operator= (SpotLight &&) = delete;
};

