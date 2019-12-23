#pragma once

#include <DirectXMath.h>

struct ShadowMatrixBuffer
{
	DirectX::XMFLOAT4X4 mShadowPerspective;
	DirectX::XMFLOAT4X4 mShadowView[6];
};

struct ShadowLightBuffer
{
	DirectX::XMFLOAT3 mLightPosition;
	float mFarPlane;
};