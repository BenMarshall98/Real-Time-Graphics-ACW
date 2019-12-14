#pragma once
#include <DirectXMath.h>
#include <iostream>

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
	
public:
	DirectionalLight(const DirectX::XMFLOAT4 & pColor, const DirectX::XMFLOAT3 & pDirection);
	DirectionalLight() = default;
	~DirectionalLight() = default;
	DirectionalLight(const DirectionalLight&) = delete;
	DirectionalLight(DirectionalLight &&) = delete;
	DirectionalLight & operator= (const DirectionalLight &) = delete;
	DirectionalLight & operator= (DirectionalLight &&) = delete;

	void setColor(DirectX::XMFLOAT4 pColor)
	{
		mColor = pColor;
	}
	
	void setDirection(DirectX::XMFLOAT3 pDirection)
	{
		mDirection = pDirection;
	}
	
	void use(DirectionalLightBuffer & pLightBuffer) const;
	void update(DirectX::XMFLOAT4X4 & pMatrix);
	void updateShadow();
};

std::istream& operator>>(std::istream & pIn, DirectionalLight & pLight);