#pragma once
#include <directxmath.h>
#include <iostream>

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
	DirectX::XMFLOAT3 mColor;
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mDirection;
	float mInnerAngle;
	float mOuterAngle;
	float mAttenuationConstant;
	float mAttenuationLinear;
	float mAttenuationQuad;
	
public:
	SpotLight(const DirectX::XMFLOAT3 & pColor, const DirectX::XMFLOAT3 pPosition, const DirectX::XMFLOAT3 pDirection, float pInnerAngle, float pOuterAngle, float pAttenuationConstant, float pAttenuationLinear, float pAttenuationQuad);
	SpotLight() = default;
	~SpotLight() = default;

	SpotLight(const SpotLight &) = delete;
	SpotLight(SpotLight &&) = delete;
	SpotLight & operator= (const SpotLight &) = delete;
	SpotLight & operator= (SpotLight &&) = delete;

	void setColor(DirectX::XMFLOAT3 pColor)
	{
		mColor = pColor;
	}

	void setPosition(DirectX::XMFLOAT3 pPosition)
	{
		mPosition = pPosition;
	}

	void setDirection(DirectX::XMFLOAT3 pDirection)
	{
		mDirection = pDirection;
	}

	void setConeAngles(float pInnerAngle, float pOuterAngle)
	{
		mInnerAngle = pInnerAngle;
		mOuterAngle = pOuterAngle;
	}

	void setAttenuation(float pConstant, float pLinear, float pQuad)
	{
		mAttenuationConstant = pConstant;
		mAttenuationLinear = pLinear;
		mAttenuationQuad = pQuad;
	}

	void use(SpotLightBuffer & pLightBuffer, unsigned int pIndex);
	void update(DirectX::XMFLOAT4X4 & pMatrix);
};

std::istream& operator>>(std::istream & pIn, SpotLight & pLight);

