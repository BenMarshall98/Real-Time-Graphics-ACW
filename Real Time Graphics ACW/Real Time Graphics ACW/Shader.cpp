#include "Shader.h"
#include <d3dcompiler.h>
#include "DX11Render.h"
#include <locale>
#include <codecvt>
#include <wrl/client.h>

const D3D11_INPUT_ELEMENT_DESC Shader::mLayout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

//https://riptutorial.com/cplusplus/example/4190/conversion-to-std--wstring

bool Shader::loadShader(const std::string& pVertexFile, const std::string& pFragmentFile)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	auto device = Dx11Render::instance()->getDevice();

	{
		Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pVertexFile), "vs_4_0", vsBlob.ReleaseAndGetAddressOf());
		if (FAILED(result))
		{
			return false;
		}

		result = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, mVertexShader.ReleaseAndGetAddressOf());
		if (FAILED(result))
		{
			return false;
		}

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		
		const UINT numElements = ARRAYSIZE(layout);

		result = device->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), mInputLayout.ReleaseAndGetAddressOf());
		if (FAILED(result))
		{
			return false;
		}
	}

	{
		Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pFragmentFile), "ps_4_0", psBlob.ReleaseAndGetAddressOf());
		if (FAILED(result))
		{
			return false;
		}

		result = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, mPixelShader.ReleaseAndGetAddressOf());
		if (FAILED(result))
		{
			return false;
		}
	}

	return true;
}

bool Shader::loadShader(const std::string & pVertexFile, const std::string & pFragmentFile, const std::string & pGeometryFile)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	auto device = Dx11Render::instance()->getDevice();

	{
		Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pVertexFile), "vs_4_0", &vsBlob);
		if (FAILED(result))
		{
			return false;
		}

		result = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &mVertexShader);
		if (FAILED(result))
		{
			return false;
		}
		
		const UINT numElements = ARRAYSIZE(mLayout);

		//Look at the warning code and see if the layout can be made to match exactly

		result = device->CreateInputLayout(mLayout, numElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &mInputLayout);
		if (FAILED(result))
		{
			return false;
		}
	}

	{
		Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pFragmentFile), "ps_4_0", &psBlob);
		if (FAILED(result))
		{
			return false;
		}

		result = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &mPixelShader);
		if (FAILED(result))
		{
			return false;
		}
	}

	{
		Microsoft::WRL::ComPtr<ID3DBlob> gsBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pGeometryFile), "gs_4_0", &gsBlob);
		if (FAILED(result))
		{
			return false;
		}

		result = device->CreateGeometryShader(gsBlob->GetBufferPointer(), gsBlob->GetBufferSize(), nullptr, &mGeometryShader);
		if (FAILED(result))
		{
			return false;
		}
	}

	return true;
}

bool Shader::loadShader(const std::string& pVertexFile, const std::string& pFragmentFile, const std::string& pHullFile, const std::string& pDomainFile)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	auto device = Dx11Render::instance()->getDevice();

	{
		Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pVertexFile), "vs_5_0", &vsBlob);
		if (FAILED(result))
		{
			return false;
		}

		result = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &mVertexShader);
		if (FAILED(result))
		{
			return false;
		}
		
		const UINT numElements = ARRAYSIZE(mLayout);

		result = device->CreateInputLayout(mLayout, numElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &mInputLayout);
		if (FAILED(result))
		{
			return false;
		}
	}

	{
		Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pFragmentFile), "ps_5_0", &psBlob);
		if (FAILED(result))
		{
			return false;
		}

		result = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &mPixelShader);
		if (FAILED(result))
		{
			return false;
		}
	}

	{
		Microsoft::WRL::ComPtr<ID3DBlob> hsBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pHullFile), "hs_5_0", &hsBlob);
		if (FAILED(result))
		{
			return false;
		}

		result = device->CreateHullShader(hsBlob->GetBufferPointer(), hsBlob->GetBufferSize(), nullptr, &mHullShader);
		if (FAILED(result))
		{
			return false;
		}
	}

	{
		Microsoft::WRL::ComPtr<ID3DBlob> dsBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pDomainFile), "ds_5_0", &dsBlob);
		if (FAILED(result))
		{
			return false;
		}

		result = device->CreateDomainShader(dsBlob->GetBufferPointer(), dsBlob->GetBufferSize(), nullptr, &mDomainShader);
		if (FAILED(result))
		{
			return false;
		}
	}

	return true;
}

bool Shader::loadShader(const std::string& pVertexFile, const std::string& pFragmentFile, const std::string& pGeometryFile, const std::string& pHullFile, const std::string& pDomainFile)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	auto device = Dx11Render::instance()->getDevice();

	{
		Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pVertexFile), "vs_5_0", &vsBlob);
		if (FAILED(result))
		{
			return false;
		}

		result = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &mVertexShader);
		if (FAILED(result))
		{
			return false;
		}

		const UINT numElements = ARRAYSIZE(mLayout);

		result = device->CreateInputLayout(mLayout, numElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &mInputLayout);
		if (FAILED(result))
		{
			return false;
		}
	}

	{
		Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pFragmentFile), "ps_5_0", &psBlob);
		if (FAILED(result))
		{
			return false;
		}

		result = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &mPixelShader);
		if (FAILED(result))
		{
			return false;
		}
	}

	{
		Microsoft::WRL::ComPtr<ID3DBlob> hsBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pHullFile), "hs_5_0", &hsBlob);
		if (FAILED(result))
		{
			return false;
		}

		result = device->CreateHullShader(hsBlob->GetBufferPointer(), hsBlob->GetBufferSize(), nullptr, &mHullShader);
		if (FAILED(result))
		{
			return false;
		}
	}

	{
		Microsoft::WRL::ComPtr<ID3DBlob> dsBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pDomainFile), "ds_5_0", &dsBlob);
		if (FAILED(result))
		{
			return false;
		}

		result = device->CreateDomainShader(dsBlob->GetBufferPointer(), dsBlob->GetBufferSize(), nullptr, &mDomainShader);
		if (FAILED(result))
		{
			return false;
		}
	}

	{
		Microsoft::WRL::ComPtr<ID3DBlob> gsBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pGeometryFile), "gs_5_0", &gsBlob);
		if (FAILED(result))
		{
			return false;
		}

		result = device->CreateGeometryShader(gsBlob->GetBufferPointer(), gsBlob->GetBufferSize(), nullptr, &mGeometryShader);
		if (FAILED(result))
		{
			return false;
		}
	}

	return true;
}

bool Shader::loadShader(const std::string& pComputeFile)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	auto device = Dx11Render::instance()->getDevice();

	Microsoft::WRL::ComPtr<ID3DBlob> csBlob = nullptr;
	auto result = compileShaderFromFile(converter.from_bytes(pComputeFile), "cs_5_0", &csBlob);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreateComputeShader(csBlob->GetBufferPointer(), csBlob->GetBufferSize(), nullptr, &mComputeShader);
	if (FAILED(result))
	{
		return false;
	}
	
	return true;
}

HRESULT Shader::compileShaderFromFile(const std::wstring & pFileName, const char * pTarget, ID3DBlob** pShaderBlob)
{
	const DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	const auto result = D3DCompileFromFile(pFileName.c_str(), nullptr, nullptr, "main", pTarget,
		shaderFlags, 0, pShaderBlob, &errorBlob);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
		}
	}

	return result;
}

void Shader::useShader()
{
	static Shader * shader = nullptr;

	if (shader != this)
	{
		auto deviceContext = Dx11Render::instance()->getDeviceContext();
		
		deviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		deviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
		deviceContext->GSSetShader(mGeometryShader.Get(), nullptr, 0);
		deviceContext->HSSetShader(mHullShader.Get(), nullptr, 0);
		deviceContext->DSSetShader(mDomainShader.Get(), nullptr, 0);
		deviceContext->CSSetShader(mComputeShader.Get(), nullptr, 0);
		
		if (!mComputeShader)
		{
			deviceContext->IASetInputLayout(mInputLayout.Get());
		}
	}
}