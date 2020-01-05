#include "DirectionalLight.h"
#include <string>
#include "DX11Render.h"
#include "CameraManager.h"

DirectionalLight::DirectionalLight(const DirectX::XMFLOAT4 & pColor, const DirectX::XMFLOAT3 & pDirection) :
	mColor(pColor), mDirection(pDirection)
{
	mFramebuffer = std::make_unique<Framebuffer>();

	const DirectX::XMVECTORF32 colour = { 1.0f, 1.0f, 1.0f, 1.0f };

	if (!mFramebuffer->loadFramebuffer(true, false, 1024, 1024, { colour }))
	{
		mFramebuffer.reset();
	}
}

DirectionalLight::DirectionalLight() : mColor(), mDirection()
{
	mFramebuffer = std::make_unique<Framebuffer>();

	const DirectX::XMVECTORF32 colour = { 1.0f, 1.0f, 1.0f, 1.0f };

	if (!mFramebuffer->loadFramebuffer(true, false, 1024, 1024, { colour }))
	{
		mFramebuffer.reset();
	}
}


void DirectionalLight::use(DirectionalLightBuffer& pLightBuffer) const
{
	pLightBuffer.mColor = mColor;
	pLightBuffer.mDirection = mDirection;
	pLightBuffer.mIsUsed = true;
	const auto pos = CameraManager::instance()->getViewPosition();
	const auto eye = DirectX::XMLoadFloat3(&pos);
	const auto target = DirectX::XMVectorAdd(eye, DirectX::XMLoadFloat3(&mDirection));
	const auto up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	
	const auto view = XMMatrixTranspose(DirectX::XMMatrixLookAtLH(eye, target, up));
	XMStoreFloat4x4(&pLightBuffer.mViewMatrix, view);

	const auto projection = DirectX::XMMatrixTranspose(DirectX::XMMatrixOrthographicOffCenterLH(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f));
	XMStoreFloat4x4(&pLightBuffer.mProjectionMatrix, projection);
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

void DirectionalLight::updateShadow()
{
	mFramebuffer->useFramebuffer();
}

void DirectionalLight::useShadow(unsigned int pTextureSlot)
{
	mFramebuffer->useTexture(pTextureSlot);
}

void DirectionalLight::releaseShadow(unsigned int pTextureSlot)
{
	mFramebuffer->releaseTexture(pTextureSlot);
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

