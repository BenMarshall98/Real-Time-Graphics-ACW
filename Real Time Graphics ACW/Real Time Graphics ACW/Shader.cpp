#include "Shader.h"
#include <d3dcompiler.h>
#include "DX11Render.h"

Shader::Shader(std::wstring vertexFile, std::wstring fragmentFile, std::wstring geometryFile)
{
	ID3D11Device * device = DX11Render::Instance()->GetDevice();
	
	HRESULT result;

	ID3DBlob * vsBlob = nullptr;
	result = CompileShaderFromFile(vertexFile, "vs_4_0", &vsBlob);
	if (FAILED(result))
	{
		
	}

	result = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader);
	if (FAILED(result))
	{
		
	}

	ID3DBlob * psBlob = nullptr;
	result = CompileShaderFromFile(fragmentFile, "ps_4_0", &psBlob);
	if (FAILED(result))
	{
		
	}

	result = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader);
	if (FAILED(result))
	{
		
	}

	ID3DBlob * gsBlob = nullptr;
	if (geometryFile.compare(L""))
	{
		result = CompileShaderFromFile(geometryFile, "gs_4_0", &gsBlob);
		if (FAILED(result))
		{
			
		}

		result = device->CreateGeometryShader(gsBlob->GetBufferPointer(), gsBlob->GetBufferSize(), nullptr, &geometryShader);
		if (FAILED(result))
		{
			
		}
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	//Look at the warning code and see if the layout can be made to match exactly
	
	result = device->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
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

Shader::~Shader()
{
}

HRESULT Shader::CompileShaderFromFile(std::wstring fileName, const char * target, ID3DBlob** shaderBlob)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ID3DBlob * errorBlob = nullptr;
	HRESULT result = D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "main", target,
		shaderFlags, 0, shaderBlob, &errorBlob);

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

void Shader::UseShader()
{
	static Shader * shader = nullptr;

	if (shader != this)
	{
		ID3D11DeviceContext * deviceContext = DX11Render::Instance()->GetDeviceContext();
		deviceContext->VSSetShader(vertexShader, nullptr, 0);
		deviceContext->PSSetShader(pixelShader, nullptr, 0);
		deviceContext->GSSetShader(geometryShader, nullptr, 0);
		deviceContext->IASetInputLayout(inputLayout);
	}
}