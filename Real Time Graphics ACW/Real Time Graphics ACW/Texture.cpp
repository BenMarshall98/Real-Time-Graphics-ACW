#include "Texture.h"
#include "DX11Render.h"
#include "DDSTextureLoader.h"
#include <codecvt>

bool Texture::loadTexture(const std::string & pTextureFile)
{
	const auto temp = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(pTextureFile);
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Dx11Render::instance()->getDevice(device);
	
	const auto result = DirectX::CreateDDSTextureFromFile(device.Get(), temp.c_str(), nullptr, mTexture.ReleaseAndGetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

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
}

void Texture::useDomain(const unsigned int pIndex) const
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Dx11Render::instance()->getDeviceContext(deviceContext);

	deviceContext->DSSetShaderResources(pIndex, 1, mTexture.GetAddressOf());
}