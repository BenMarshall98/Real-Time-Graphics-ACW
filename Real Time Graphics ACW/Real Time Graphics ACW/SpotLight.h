#pragma once
#include <directxmath.h>
#include <iostream>
#include "Framebuffer.h"
#include <array>

struct SpotLightBuffer
{
	std::array<DirectX::XMFLOAT4, 4> mColor;
	std::array<DirectX::XMFLOAT4, 4> mPosition;
	std::array<DirectX::XMFLOAT4, 4> mDirection;
	std::array<float, 4> mInnerAngle;
	std::array<float, 4> mOuterAngle;
	std::array<float, 4> mAttenuationConstant;
	std::array<float, 4> mAttenuationLinear;
	std::array<float, 4> mAttenuationQuad;
	std::array<int, 4> mIsUsed;
	std::array<float, 4> mFarPlane;
};

class SpotLight
{
	DirectX::XMFLOAT3 mColor;
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mDirection;
	std::unique_ptr<Framebuffer> mFramebuffer;
	float mInnerAngle;
	float mOuterAngle;
	float mAttenuationConstant;
	float mAttenuationLinear;
	float mAttenuationQuad;
	
	
public:
	SpotLight(const DirectX::XMFLOAT3 & pColor, const DirectX::XMFLOAT3 & pPosition, const DirectX::XMFLOAT3 & pDirection, float pInnerAngle, float pOuterAngle, float pAttenuationConstant, float pAttenuationLinear, float pAttenuationQuad);
	SpotLight();
	~SpotLight() = default;

	SpotLight(const SpotLight &) = delete;
	SpotLight(SpotLight &&) = delete;
	SpotLight & operator= (const SpotLight &) = delete;
	SpotLight & operator= (SpotLight &&) = delete;

	void setColor(const DirectX::XMFLOAT3 & pColor)
	{
		mColor = pColor;
	}

	void setPosition(const DirectX::XMFLOAT3 & pPosition)
	{
		mPosition = pPosition;
	}

	void setDirection(const DirectX::XMFLOAT3 & pDirection)
	{
		mDirection = pDirection;
	}

	void setConeAngles(const float pInnerAngle, const float pOuterAngle)
	{
		mInnerAngle = pInnerAngle;
		mOuterAngle = pOuterAngle;
	}

	void setAttenuation(const float pConstant, const float pLinear, const float pQuad)
	{
		mAttenuationConstant = pConstant;
		mAttenuationLinear = pLinear;
		mAttenuationQuad = pQuad;
	}

	void use(SpotLightBuffer & pLightBuffer, unsigned int pIndex) const;
	void update(const DirectX::XMFLOAT4X4 & pMatrix);
	void updateShadow() const;
	void useShadow(unsigned int pTextureSlot) const;
	void releaseShadow(unsigned int pTextureSlot) const;
};

std::istream& operator>>(std::istream & pIn, SpotLight & pLight);

