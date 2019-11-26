#include "Shader.h"
#include <d3dcompiler.h>
#include "DX11Render.h"

Shader::Shader(const std::wstring & pVertexFile, const std::wstring & pFragmentFile, const std::wstring & pGeometryFile)
{
	auto device = Dx11Render::instance()->getDevice();

	ID3DBlob * vsBlob = nullptr;
	auto result = compileShaderFromFile(pVertexFile, "vs_4_0", &vsBlob);
	if (FAILED(result))
	{
		
	}

	result = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &mVertexShader);
	if (FAILED(result))
	{
		
	}

	ID3DBlob * psBlob = nullptr;
	result = compileShaderFromFile(pFragmentFile, "ps_4_0", &psBlob);
	if (FAILED(result))
	{
		
	}

	result = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &mPixelShader);
	if (FAILED(result))
	{
		
	}

	ID3DBlob * gsBlob = nullptr;
	if (!pGeometryFile.empty())
	{
		result = compileShaderFromFile(pGeometryFile, "gs_4_0", &gsBlob);
		if (FAILED(result))
		{
			
		}

		result = device->CreateGeometryShader(gsBlob->GetBufferPointer(), gsBlob->GetBufferSize(), nullptr, &mGeometryShader);
		if (FAILED(result))
		{
			
		}
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
		
	}

	if (vsBlob)
	{
		vsBlob->Release();
	}

	if (psBlob)
	{
		psBlob->Release();
	}

	if (gsBlob)
	{
		gsBlob->Release();
	}
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
		deviceContext->IASetInputLayout(mInputLayout);
	}
}