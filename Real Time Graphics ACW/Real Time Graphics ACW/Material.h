#pragma once
#include <DirectXMath.h>
#include <iostream>

struct MaterialBuffer
{
	DirectX::XMFLOAT4 mAmbient;
	DirectX::XMFLOAT4 mDiffuse;
	DirectX::XMFLOAT4 mSpecular;
	float mShininess;
	DirectX::XMFLOAT3 mPadding;
};

class Material
{
	DirectX::XMFLOAT3 mAmbient;
	DirectX::XMFLOAT3 mDiffuse;
	DirectX::XMFLOAT3 mSpecular;
	float mShininess;

public:
	Material(const DirectX::XMFLOAT3 & pAmbient, const DirectX::XMFLOAT3 & pDiffuse, const DirectX::XMFLOAT3 & pSpecular, float pShininess);
	Material();
	~Material() = default;

	Material(const Material &) = delete;
	Material(Material &&) = delete;
	Material & operator= (const Material &) = delete;
	Material & operator= (Material &&) = delete;

	void setAmbient(const DirectX::XMFLOAT3 & pAmbient)
	{
		mAmbient = pAmbient;
	}

	void setDiffuse(const DirectX::XMFLOAT3 & pDiffuse)
	{
		mDiffuse = pDiffuse;
	}

	void setSpecular(const DirectX::XMFLOAT3 & pSpecular)
	{
		mSpecular = pSpecular;
	}

	void setShininess(const float pShininess)
	{
		mShininess = pShininess;
	}

	void use() const;
};

std::istream & operator>>(std::istream & pIn, std::unique_ptr<Material> & pMaterial);