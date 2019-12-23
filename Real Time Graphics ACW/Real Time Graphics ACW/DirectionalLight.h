#pragma once
#include <DirectXMath.h>
#include <iostream>
#include "Framebuffer.h"

struct DirectionalLightBuffer
{
	DirectX::XMFLOAT4 mColor;
	DirectX::XMFLOAT3 mDirection;
	int mIsUsed;
	DirectX::XMFLOAT4X4 mViewMatrix;
	DirectX::XMFLOAT4X4 mProjectionMatrix;
};

class DirectionalLight
{
	DirectX::XMFLOAT4 mColor;
	DirectX::XMFLOAT3 mDirection;
	std::unique_ptr<Framebuffer> mFramebuffer;
	
public:
	DirectionalLight(const DirectX::XMFLOAT4 & pColor, const DirectX::XMFLOAT3 & pDirection);
	DirectionalLight();
	~DirectionalLight() = default;
	DirectionalLight(const DirectionalLight&) = delete;
	DirectionalLight(DirectionalLight &&) = delete;
	DirectionalLight & operator= (const DirectionalLight &) = delete;
	DirectionalLight & operator= (DirectionalLight &&) = delete;

	void setColor(const DirectX::XMFLOAT4 & pColor)
	{
		mColor = pColor;
	}
	
	void setDirection(const DirectX::XMFLOAT3 & pDirection)
	{
		mDirection = pDirection;
	}
	
	void use(DirectionalLightBuffer & pLightBuffer) const;
	void update(DirectX::XMFLOAT4X4 & pMatrix);
	void updateShadow();
	void useShadow(unsigned int pTextureSlot);
	void releaseShadow(unsigned int pTextureSlot);
};

std::istream& operator>>(std::istream & pIn, DirectionalLight & pLight);