#include "SpotLight.h"
#include <string>

SpotLight::SpotLight(const DirectX::XMFLOAT3& pColor, const DirectX::XMFLOAT3 & pPosition,
	const DirectX::XMFLOAT3 & pDirection, const float pInnerAngle, const float pOuterAngle,
	const float pAttenuationConstant, const float pAttenuationLinear, const float pAttenuationQuad) :
	mColor(pColor), mPosition(pPosition), mDirection(pDirection),
	mInnerAngle(pInnerAngle), mOuterAngle(pOuterAngle), mAttenuationConstant(pAttenuationConstant),
	mAttenuationLinear(pAttenuationLinear), mAttenuationQuad(pAttenuationQuad)
{
}

void SpotLight::use(SpotLightBuffer& pLightBuffer, const unsigned int pIndex) const
{
	pLightBuffer.mColor[pIndex] = DirectX::XMFLOAT4(mColor.x, mColor.y, mColor.z, 0.0f);
	pLightBuffer.mPosition[pIndex] = DirectX::XMFLOAT4(mPosition.x, mPosition.y, mPosition.z, 0.0f);
	pLightBuffer.mDirection[pIndex] = DirectX::XMFLOAT4(mDirection.x, mDirection.y, mDirection.z, 0.0f);
	pLightBuffer.mInnerAngle[pIndex] = DirectX::XMConvertToRadians(mInnerAngle);
	pLightBuffer.mOuterAngle[pIndex] = DirectX::XMConvertToRadians(mOuterAngle);
	pLightBuffer.mAttenuationConstant[pIndex] = mAttenuationConstant;
	pLightBuffer.mAttenuationLinear[pIndex] = mAttenuationLinear;
	pLightBuffer.mAttenuationQuad[pIndex] = mAttenuationQuad;
	pLightBuffer.mIsUsed[pIndex] = true;
}

void SpotLight::update(DirectX::XMFLOAT4X4& pMatrix)
{
	auto center = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	auto direction = XMLoadFloat3(&mDirection);
	direction = DirectX::XMVectorAdd(center, direction);
	
	const auto matrix = XMLoadFloat4x4(&pMatrix);

	center = XMVector3Transform(center, matrix);
	direction = XMVector3Transform(direction, matrix);

	direction = DirectX::XMVectorSubtract(direction, center);

	XMStoreFloat3(&mPosition, center);
	XMStoreFloat3(&mDirection, direction);
}

void SpotLight::updateShadow()
{
	//TODO: Implement	
}

std::istream& operator>>(std::istream& pIn, SpotLight& pLight)
{
	std::string s;
	char c;

	float x, y, z;
	pIn >> s >> x >> c >> y >> c >> z;
	pLight.setColor(DirectX::XMFLOAT3(x,y, z));

	pIn >> s >> x >> c >> y >> c >> z;
	pLight.setPosition(DirectX::XMFLOAT3(x, y, z));

	pIn >> s >> x >> c >> y >> c >> z;
	pLight.setDirection(DirectX::XMFLOAT3(x, y, z));

	pIn >> s >> x >> s >> y;
	pLight.setConeAngles(x, y);

	pIn >> s >> x >> s >> y >> s >> z;
	pLight.setAttenuation(x, y, z);

	return pIn;
}
