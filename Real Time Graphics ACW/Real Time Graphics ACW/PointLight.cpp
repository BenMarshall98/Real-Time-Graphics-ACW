#include "PointLight.h"
#include <string>

PointLight::PointLight(const DirectX::XMFLOAT3 pColor, const DirectX::XMFLOAT3 pPosition,
	const float pAttenuationConstant, const float pAttenuationLinear, const float pAttenuationQuad) :
	mColor(pColor), mPosition(pPosition), mAttenuationConstant(pAttenuationConstant),
	mAttenuationLinear(pAttenuationLinear), mAttenuationQuad(pAttenuationQuad)
{
}

void PointLight::use(PointLightBuffer & pLightBuffer) const
{
	pLightBuffer.mColor = DirectX::XMFLOAT4(mColor.x, mColor.y, mColor.z, 0.0f);
	pLightBuffer.mPosition = DirectX::XMFLOAT4(mPosition.x, mPosition.y, mPosition.z, 0.0f);
	pLightBuffer.mAttenuationConstant = mAttenuationConstant;
	pLightBuffer.mAttenuationLinear = mAttenuationLinear;
	pLightBuffer.mAttenuationQuad = mAttenuationQuad;
	pLightBuffer.mIsUsed = true;
}

void PointLight::update(DirectX::XMFLOAT4X4& pMatrix)
{
	auto center = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	const auto matrix = XMLoadFloat4x4(&pMatrix);

	center = XMVector3Transform(center, matrix);

	XMStoreFloat3(&mPosition, center);
}

void PointLight::updateShadow()
{
	//TODO: Implement
}

std::istream& operator>>(std::istream& pIn, PointLight& pLight)
{
	std::string s;
	char c;

	float x, y, z;
	pIn >> s >> x >> c >> y >> c >> z;
	pLight.setColor(DirectX::XMFLOAT3(x, y, z));

	pIn >> s >> x >> c >> y >> c >> z;
	pLight.setPosition(DirectX::XMFLOAT3(x, y, z));

	pIn >> s >> x >> s >> y >> s >> z;
	pLight.setAttenuation(x, y, z);

	return pIn;
}