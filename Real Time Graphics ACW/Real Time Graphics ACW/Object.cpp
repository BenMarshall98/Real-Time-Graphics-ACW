#include "Object.h"

//TODO: look at model

Object::Object(const std::string & pModelFile, const DirectX::XMFLOAT3 & pAmbient,
	const DirectX::XMFLOAT3 & pDiffuse, const DirectX::XMFLOAT3 & pSpecular,
	const float & pShininess) :
	mModel(nullptr), mAmbient(pAmbient), mDiffuse(pDiffuse), mSpecular(pSpecular), mShininess(pShininess)
{
}

Object::~Object()
{
}
