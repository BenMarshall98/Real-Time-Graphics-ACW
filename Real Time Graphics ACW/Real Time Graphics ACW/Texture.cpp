#include "Texture.h"
#include "DX11Render.h"
#include "DDSTextureLoader.h"

Texture::Texture(const std::wstring & pTextureFile)
{
	const auto device = Dx11Render::instance()->getDevice();
	auto result = DirectX::CreateDDSTextureFromFile(device, pTextureFile.c_str(), nullptr, &mTexture);

	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof desc);
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	result = device->CreateSamplerState(&desc, &mSampler);
}

Texture::~Texture()
{
	mTexture->Release();
	mSampler->Release();
}

void Texture::use() const
{
	const auto deviceContext = Dx11Render::instance()->getDeviceContext();

	deviceContext->PSSetShaderResources(0, 1, &mTexture);
	deviceContext->PSSetSamplers(0, 1, &mSampler);
}
