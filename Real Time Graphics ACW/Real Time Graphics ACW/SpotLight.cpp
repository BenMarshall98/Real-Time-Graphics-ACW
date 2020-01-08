#include "SpotLight.h"
#include <string>
#include "Shadow.h"
#include "DX11Render.h"

SpotLight::SpotLight(const DirectX::XMFLOAT3& pColor, const DirectX::XMFLOAT3 & pPosition,
	const DirectX::XMFLOAT3 & pDirection, const float pInnerAngle, const float pOuterAngle,
	const float pAttenuationConstant, const float pAttenuationLinear, const float pAttenuationQuad) :
	mColor(pColor), mPosition(pPosition), mDirection(pDirection),
	mInnerAngle(pInnerAngle), mOuterAngle(pOuterAngle), mAttenuationConstant(pAttenuationConstant),
	mAttenuationLinear(pAttenuationLinear), mAttenuationQuad(pAttenuationQuad)
{
	mMappingFramebuffer = std::make_unique<Framebuffer>();
	mSimpleFramebuffer = std::make_unique<Framebuffer>();

	const DirectX::XMVECTORF32 colour = { 1.0f, 1.0f, 1.0f, 1.0f };

	if (mMappingFramebuffer->loadFramebuffer(true, false, 1024, 1024, { colour }, TextureType::TEXTURE_CUBE))
	{
		mMappingFramebuffer.reset();
	}

	if (!mSimpleFramebuffer->loadFramebuffer(false, true, 1024, 1024, { colour }))
	{
		mSimpleFramebuffer.reset();
	}
}

SpotLight::SpotLight() : mAttenuationConstant(1.0f), mAttenuationLinear(0.0f), mAttenuationQuad(0.0f), mInnerAngle(5.0f), mOuterAngle(15.0f)
{
	mMappingFramebuffer = std::make_unique<Framebuffer>();
	mSimpleFramebuffer = std::make_unique<Framebuffer>();

	const DirectX::XMVECTORF32 colour = { 1.0f, 1.0f, 1.0f, 1.0f };

	if (!mMappingFramebuffer->loadFramebuffer(true, false, 1024, 1024, { colour }, TextureType::TEXTURE_CUBE))
	{
		mMappingFramebuffer.reset();
	}

	if (!mSimpleFramebuffer->loadFramebuffer(false, true, 1024, 1024, { colour }))
	{
		mSimpleFramebuffer.reset();
	}
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
	pLightBuffer.mFarPlane[pIndex] = 20.0f;
}

void SpotLight::update(const DirectX::XMFLOAT4X4& pMatrix)
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

void SpotLight::updateMappingShadow() const
{
	ShadowMatrixBuffer mb;

	XMStoreFloat4x4(&mb.mShadowPerspective, XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, static_cast<float>(mMappingFramebuffer->getWidth()) / static_cast<float>(mMappingFramebuffer->getHeight()), 0.01f, 20.0f)));
	XMStoreFloat4x4(&mb.mShadowView[0], XMMatrixTranspose(DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(mPosition.x, mPosition.y, mPosition.z, 0.0f), DirectX::XMVectorSet(mPosition.x + 1.0f, mPosition.y + 0.0f, mPosition.z + 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))));
	XMStoreFloat4x4(&mb.mShadowView[1], XMMatrixTranspose(DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(mPosition.x, mPosition.y, mPosition.z, 0.0f), DirectX::XMVectorSet(mPosition.x - 1.0f, mPosition.y + 0.0f, mPosition.z + 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))));
	XMStoreFloat4x4(&mb.mShadowView[2], XMMatrixTranspose(DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(mPosition.x, mPosition.y, mPosition.z, 0.0f), DirectX::XMVectorSet(mPosition.x + 0.0f, mPosition.y + 1.0f, mPosition.z + 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f))));
	XMStoreFloat4x4(&mb.mShadowView[3], XMMatrixTranspose(DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(mPosition.x, mPosition.y, mPosition.z, 0.0f), DirectX::XMVectorSet(mPosition.x + 0.0f, mPosition.y - 1.0f, mPosition.z + 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f))));
	XMStoreFloat4x4(&mb.mShadowView[4], XMMatrixTranspose(DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(mPosition.x, mPosition.y, mPosition.z, 0.0f), DirectX::XMVectorSet(mPosition.x + 0.0f, mPosition.y + 0.0f, mPosition.z + 1.0f, 0.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))));
	XMStoreFloat4x4(&mb.mShadowView[5], XMMatrixTranspose(DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(mPosition.x, mPosition.y, mPosition.z, 0.0f), DirectX::XMVectorSet(mPosition.x + 0.0f, mPosition.y + 0.0f, mPosition.z - 1.0f, 0.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))));

	Dx11Render::instance()->useShadowMatrixBuffer(mb);

	ShadowLightBuffer lb;

	lb.mLightPosition = mPosition;
	lb.mFarPlane = 20.0f;

	Dx11Render::instance()->useShadowLightBuffer(lb);

	mMappingFramebuffer->useFramebuffer();
}

void SpotLight::useMappingShadow(const unsigned int pTextureSlot) const
{
	mMappingFramebuffer->useTexture(pTextureSlot);
}

void SpotLight::releaseMappingShadow(const unsigned int pTextureSlot) const
{
	mMappingFramebuffer->releaseTexture(pTextureSlot);
}

void SpotLight::updateSimpleShadow() const
{
	ShadowLightBuffer lb;

	lb.mLightPosition = mPosition;
	lb.mFarPlane = 20.0f;

	Dx11Render::instance()->useShadowLightBuffer(lb);

	mSimpleFramebuffer->useFramebuffer();
}

void SpotLight::useSimpleShadow(const unsigned int pTextureSlot) const
{
	mSimpleFramebuffer->useTexture(pTextureSlot);
}

void SpotLight::releaseSimpleShadow(const unsigned int pTextureSlot) const
{
	mSimpleFramebuffer->releaseTexture(pTextureSlot);
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
