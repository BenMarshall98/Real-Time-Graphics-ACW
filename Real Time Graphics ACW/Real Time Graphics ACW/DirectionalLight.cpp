#include "DirectionalLight.h"
#include <string>

DirectionalLight::DirectionalLight(const DirectX::XMFLOAT4 & pColor, const DirectX::XMFLOAT3 & pDirection) :
	mColor(pColor), mDirection(pDirection)
{
}

void DirectionalLight::use(DirectionalLightBuffer& pLightBuffer) const
{
	pLightBuffer.mColor = mColor;
	pLightBuffer.mDirection = mDirection;
	pLightBuffer.mIsUsed = true;
}

void DirectionalLight::update(DirectX::XMFLOAT4X4& pMatrix)
{
	auto center = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	auto direction = XMLoadFloat3(&mDirection);
	const auto matrix = XMLoadFloat4x4(&pMatrix);

	center = XMVector3Transform(center, matrix);
	direction = XMVector3Transform(direction, matrix);

	direction = DirectX::XMVectorSubtract(direction, center);

	XMStoreFloat3(&mDirection, direction);
}

std::istream& operator>>(std::istream& pIn, DirectionalLight& pLight)
{
	std::string s;
	char c;

	float x, y, z;
	pIn >> s >> x >> c >> y >> c >> z;
	pLight.setColor(DirectX::XMFLOAT4(x, y, z, 1.0f));

	pIn >> s >> x >> c >> y >> c >> z;
	pLight.setDirection(DirectX::XMFLOAT3(x, y, z));

	return pIn;
}