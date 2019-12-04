#include "Shader.h"
#include <d3dcompiler.h>
#include "DX11Render.h"
#include <locale>
#include <codecvt>
#include <wrl/client.h>

//https://riptutorial.com/cplusplus/example/4190/conversion-to-std--wstring

bool Shader::loadShader(const std::string& pVertexFile, const std::string& pFragmentFile)
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
			if (vsBlob)
			{
				vsBlob->Release();
			}
			return false;
		}

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		const UINT numElements = ARRAYSIZE(layout);

		result = device->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &mInputLayout);
		if (FAILED(result))
		{
			if (vsBlob)
			{
				vsBlob->Release();
			}
			return false;
		}

		if (vsBlob)
		{
			vsBlob->Release();
		}
	}

	{
		ID3DBlob * psBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pFragmentFile), "ps_4_0", &psBlob);
		if (FAILED(result))
		{
			if (psBlob)
			{
				psBlob->Release();
			}
			return false;
		}

		result = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &mPixelShader);
		if (FAILED(result))
		{
			if (psBlob)
			{
				psBlob->Release();
			}
			return false;
		}

		if (psBlob)
		{
			psBlob->Release();
		}
	}

	return true;
}

bool Shader::loadShader(const std::string & pVertexFile, const std::string & pFragmentFile, const std::string & pGeometryFile)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	auto device = Dx11Render::instance()->getDevice();

	{
		ID3DBlob * vsBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pVertexFile), "vs_4_0", &vsBlob);
		if (FAILED(result))
		{
			if (vsBlob)
			{
				vsBlob->Release();
			}
			return false;
		}

		result = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &mVertexShader);
		if (FAILED(result))
		{
			if (vsBlob)
			{
				vsBlob->Release();
			}
			return false;
		}

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		const UINT numElements = ARRAYSIZE(layout);

		//Look at the warning code and see if the layout can be made to match exactly

		result = device->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &mInputLayout);
		if (FAILED(result))
		{
			if (vsBlob)
			{
				vsBlob->Release();
			}
			return false;
		}

		if (vsBlob)
		{
			vsBlob->Release();
		}
	}

	{
		ID3DBlob * psBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pFragmentFile), "ps_4_0", &psBlob);
		if (FAILED(result))
		{
			if (psBlob)
			{
				psBlob->Release();
			}
			return false;
		}

		result = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &mPixelShader);
		if (FAILED(result))
		{
			if (psBlob)
			{
				psBlob->Release();
			}
			return false;
		}

		if (psBlob)
		{
			psBlob->Release();
		}
	}

	{
		ID3DBlob * gsBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pGeometryFile), "gs_4_0", &gsBlob);
		if (FAILED(result))
		{
			if (gsBlob)
			{
				gsBlob->Release();
			}

			return false;
		}

		result = device->CreateGeometryShader(gsBlob->GetBufferPointer(), gsBlob->GetBufferSize(), nullptr, &mGeometryShader);
		if (FAILED(result))
		{
			if (gsBlob)
			{
				gsBlob->Release();
			}

			return false;
		}

		if (gsBlob)
		{
			gsBlob->Release();
		}
	}

	return true;
}

bool Shader::loadShader(const std::string& pVertexFile, const std::string& pFragmentFile, const std::string& pHullFile, const std::string& pDomainFile)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	auto device = Dx11Render::instance()->getDevice();

	{
		ID3DBlob * vsBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pVertexFile), "vs_4_0", &vsBlob);
		if (FAILED(result))
		{
			if (vsBlob)
			{
				vsBlob->Release();
			}
			return false;
		}

		result = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &mVertexShader);
		if (FAILED(result))
		{
			if (vsBlob)
			{
				vsBlob->Release();
			}
			return false;
		}

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		const UINT numElements = ARRAYSIZE(layout);

		result = device->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &mInputLayout);
		if (FAILED(result))
		{
			if (vsBlob)
			{
				vsBlob->Release();
			}
			return false;
		}

		if (vsBlob)
		{
			vsBlob->Release();
		}
	}

	{
		ID3DBlob * psBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pFragmentFile), "ps_4_0", &psBlob);
		if (FAILED(result))
		{
			if (psBlob)
			{
				psBlob->Release();
			}
			return false;
		}

		result = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &mPixelShader);
		if (FAILED(result))
		{
			if (psBlob)
			{
				psBlob->Release();
			}
			return false;
		}

		if (psBlob)
		{
			psBlob->Release();
		}
	}

	{
		ID3DBlob * hsBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pHullFile), "hs_5_0", &hsBlob);
		if (FAILED(result))
		{
			if (hsBlob)
			{
				hsBlob->Release();
			}
			return false;
		}

		result = device->CreateHullShader(hsBlob->GetBufferPointer(), hsBlob->GetBufferSize(), nullptr, &mHullShader);
		if (FAILED(result))
		{
			if (hsBlob)
			{
				hsBlob->Release();
			}
			return false;
		}

		if (hsBlob)
		{
			hsBlob->Release();
		}
	}

	{
		ID3DBlob * dsBlob = nullptr;
		auto result = compileShaderFromFile(converter.from_bytes(pDomainFile), "ds_4_0", &dsBlob);
		if (FAILED(result))
		{
			if (dsBlob)
			{
				dsBlob->Release();
			}
			return false;
		}

		result = device->CreateDomainShader(dsBlob->GetBufferPointer(), dsBlob->GetBufferSize(), nullptr, &mDomainShader);
		if (FAILED(result))
		{
			if (dsBlob)
			{
				dsBlob->Release();
			}
			return false;
		}

		if (dsBlob)
		{
			dsBlob->Release();
		}
	}

	return true;
}

bool Shader::loadShader(const std::string& pComputeFile)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	auto device = Dx11Render::instance()->getDevice();

	ID3DBlob * csBlob = nullptr;
	auto result = compileShaderFromFile(converter.from_bytes(pComputeFile), "vs_4_0", &csBlob);
	if (FAILED(result))
	{
		if (csBlob)
		{
			csBlob->Release();
		}
		return false;
	}

	result = device->CreateComputeShader(csBlob->GetBufferPointer(), csBlob->GetBufferSize(), nullptr, &mComputeShader);
	if (FAILED(result))
	{
		if (csBlob)
		{
			csBlob->Release();
		}
		return false;
	}

	if (csBlob)
	{
		csBlob->Release();
	}
	return true;
}

HRESULT Shader::compileShaderFromFile(const std::wstring & pFileName, const char * pTarget, ID3DBlob** pShaderBlob)
{
	const DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ID3DBlob * errorBlob = nullptr;
	const auto result = D3DCompileFromFile(pFileName.c_str(), nullptr, nullptr, "main", pTarget,
		shaderFlags, 0, pShaderBlob, &errorBlob);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
		}
	}

	if (errorBlob)
	{
		errorBlob->Release();
	}

	return result;
}

void Shader::useShader()
{
	static Shader * shader = nullptr;

	if (shader != this)
	{
		auto deviceContext = Dx11Render::instance()->getDeviceContext();
		
		deviceContext->VSSetShader(mVertexShader, nullptr, 0);
		deviceContext->PSSetShader(mPixelShader, nullptr, 0);
		deviceContext->GSSetShader(mGeometryShader, nullptr, 0);
		deviceContext->HSSetShader(mHullShader, nullptr, 0);
		deviceContext->DSSetShader(mDomainShader, nullptr, 0);
		deviceContext->CSSetShader(mComputeShader, nullptr, 0);
		if (!mComputeShader)
		{
			deviceContext->IASetInputLayout(mInputLayout);
		}
	}
}