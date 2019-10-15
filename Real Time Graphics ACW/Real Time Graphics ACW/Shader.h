#pragma once

#include <string>
#include <d3d11_1.h>

class Shader
{
	ID3D11VertexShader * vertexShader = nullptr;
	ID3D11PixelShader * pixelShader = nullptr;
	ID3D11GeometryShader * geometryShader = nullptr;
	ID3D11InputLayout * inputLayout = nullptr;

	HRESULT CompileShaderFromFile(std::wstring fileName, const char * target, ID3DBlob** shaderBlob);
	
public:
	Shader(std::wstring vertexFile, std::wstring fragmentFile, std::wstring geometryShader = L"");
	~Shader();

	void UseShader();
};

