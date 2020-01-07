#pragma once
#include <directxmath.h>
#include <istream>
#include "Framebuffer.h"

struct PointLightBuffer
{
	DirectX::XMFLOAT4 mColor;
	DirectX::XMFLOAT3 mPosition;
	float mAttenuationConstant;
	float mAttenuationLinear;
	float mAttenuationQuad;
	int mIsUsed;
	float mFarPlane;
};

class PointLight
{
	DirectX::XMFLOAT3 mColor;
	DirectX::XMFLOAT3 mPosition;
	std::unique_ptr<Framebuffer> mFramebuffer;
	float mAttenuationConstant;
	float mAttenuationLinear;
	float mAttenuationQuad;
	
	
public:
	PointLight(const DirectX::XMFLOAT3 & pColor, const DirectX::XMFLOAT3 & pPosition, float pAttenuationConstant, float pAttenuationLinear, float pAttenuationQuad);
	PointLight();
	~PointLight() = default;

	PointLight(const PointLight &) = delete;
	PointLight(PointLight &&) = delete;
	PointLight & operator= (const PointLight &) = delete;
	PointLight & operator= (PointLight &&) = delete;

	void setColor(const DirectX::XMFLOAT3 & pColor)
	{
		mColor = pColor;
	}

	void setPosition(const DirectX::XMFLOAT3 & pPosition)
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
	void update(const DirectX::XMFLOAT4X4 & pMatrix);
	void updateShadow() const;
	void useShadow(unsigned int pTextureSlot) const;
	void releaseShadow(unsigned int pTextureSlot) const;
};

std::istream& operator>>(std::istream & pIn, PointLight & pLight);

