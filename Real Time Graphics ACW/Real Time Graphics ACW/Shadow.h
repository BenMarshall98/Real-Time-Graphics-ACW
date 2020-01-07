#pragma once

#include <DirectXMath.h>
#include <array>

struct ShadowMatrixBuffer
{
	DirectX::XMFLOAT4X4 mShadowPerspective;
	std::array<DirectX::XMFLOAT4X4, 6> mShadowView;
};

struct ShadowLightBuffer
{
	DirectX::XMFLOAT3 mLightPosition;
	float mFarPlane;
};