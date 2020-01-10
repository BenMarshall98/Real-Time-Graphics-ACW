#pragma once
#include <directxmath.h>
#include <istream>
#include "Framebuffer.h"
#include "Animation.h"
#include "QuaternionAnimation.h"

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
	DirectX::XMFLOAT4X4 mRotationMatrix = DirectX::XMFLOAT4X4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT3 mColor;
	DirectX::XMFLOAT3 mPosition;
	std::unique_ptr<Framebuffer> mMappingFramebuffer;
	std::unique_ptr<Framebuffer> mSimpleFramebuffer;
	std::unique_ptr<Animation> mAnimation;
	float mAttenuationConstant;
	float mAttenuationLinear;
	float mAttenuationQuad;
	bool mAnimate;
	
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

	void setAnimaiton(std::unique_ptr<QuaternionAnimation> & pAnimation)
	{
		mAnimation = std::move(pAnimation);
	}

	void enableAnimation()
	{
		mAnimate = true;
	}

	void disableAnimation()
	{
		mAnimate = false;
	}

	void use(PointLightBuffer & pLightBuffer) const;
	void update(const DirectX::XMFLOAT4X4 & pMatrix);
	void updateMappingShadow() const;
	void useMappingShadow(unsigned int pTextureSlot) const;
	void releaseMappingShadow(unsigned int pTextureSlot) const;
	void updateSimpleShadow() const;
	void useSimpleShadow(unsigned int pTextureSlot) const;
	void releaseSimpleShadow(unsigned int pTextureSlot) const;
};

std::istream& operator>>(std::istream & pIn, PointLight & pLight);

