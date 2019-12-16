#pragma once
#include <directxmath.h>
#include <istream>

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
	DirectX::XMFLOAT3 mColor;
	DirectX::XMFLOAT3 mPosition;
	float mAttenuationConstant;
	float mAttenuationLinear;
	float mAttenuationQuad;
	
public:
	PointLight(DirectX::XMFLOAT3 pColor, DirectX::XMFLOAT3 pPosition, float pAttenuationConstant, float pAttenuationLinear, float pAttenuationQuad);
	PointLight() = default;
	~PointLight() = default;

	PointLight(const PointLight &) = delete;
	PointLight(PointLight &&) = delete;
	PointLight & operator= (const PointLight &) = delete;
	PointLight & operator= (PointLight &&) = delete;

	void setColor(const DirectX::XMFLOAT3 pColor)
	{
		mColor = pColor;
	}

	void setPosition(const DirectX::XMFLOAT3 pPosition)
	{
		mPosition = pPosition;
	}

	void setAttenuation(const float pConstant, const float pLinear, const float pQuad)
	{
		mAttenuationConstant = pConstant;
		mAttenuationLinear = pLinear;
		mAttenuationQuad = pQuad;
	}

	void use(PointLightBuffer & pLightBuffer) const;
	void update(DirectX::XMFLOAT4X4 & pMatrix);
	void updateShadow();
};

std::istream& operator>>(std::istream & pIn, PointLight & pLight);

