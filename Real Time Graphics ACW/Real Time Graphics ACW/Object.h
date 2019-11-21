#pragma once
#include <DirectXMath.h>
#include "Model.h"
#include <memory>

class Object
{
	std::unique_ptr<Model> mModel;
	DirectX::XMFLOAT3 mAmbient;
	DirectX::XMFLOAT3 mDiffuse;
	DirectX::XMFLOAT3 mSpecular;
	float mShininess;
	
public:
	Object(const std::string & pModelFile, const DirectX::XMFLOAT3 & pAmbient,
		const DirectX::XMFLOAT3 & pDiffuse, const DirectX::XMFLOAT3 & pSpecular,
		const float & pShininess);
	Object();
	~Object() = default;

	void setModel(Model * pModel)
	{
		mModel.reset(pModel);
	}

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

	void setShininess(const float & pShininess)
	{
		mShininess = pShininess;
	}
};

std::istream& operator>>(std::istream& pIn, Object & pObject);