#include "Texture.h"
#include "DX11Render.h"
#include "DDSTextureLoader.h"
#include <codecvt>

bool Texture::loadTexture(const std::string & pTextureFile)
{
	//TODO: Check if there is a better way
	const auto temp = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(pTextureFile);
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Dx11Render::instance()->getDevice(device);
	
	auto result = DirectX::CreateDDSTextureFromFile(device.Get(), temp.c_str(), nullptr, mTexture.ReleaseAndGetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof desc);
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	result = device->CreateSamplerState(&desc, &mSampler);

	if (FAILED(result))
	{
		return false;
	}
	
	return true;
}

Texture::~Texture()
{
}

void Texture::useFragment(const unsigned int pIndex) const
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Dx11Render::instance()->getDeviceContext(deviceContext);

	deviceContext->PSSetShaderResources(pIndex, 1, mTexture.GetAddressOf());
	deviceContext->PSSetSamplers(pIndex, 1, mSampler.GetAddressOf());
}

void Texture::useDomain(const unsigned int pIndex) const
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Dx11Render::instance()->getDeviceContext(deviceContext);

	deviceContext->DSSetShaderResources(pIndex, 1, mTexture.GetAddressOf());
	deviceContext->DSSetSamplers(pIndex, 1, mSampler.GetAddressOf());
}