#pragma once

#include <string>
#include <d3d11_1.h>

class Shader
{
	ID3D11VertexShader * mVertexShader = nullptr;
	ID3D11PixelShader * mPixelShader = nullptr;
	ID3D11GeometryShader * mGeometryShader = nullptr;
	ID3D11InputLayout * mInputLayout = nullptr;

	static HRESULT compileShaderFromFile(const std::wstring & pFileName, const char * pTarget, ID3DBlob** pShaderBlob);
	
public:
	Shader() = default;
	Shader(const std::wstring & pVertexFile, const std::wstring & pFragmentFile, const std::wstring & pGeometryFile = L"");
	~Shader() = default;

	Shader(const Shader&) = delete;
	Shader(Shader &&) = delete;
	Shader & operator= (const Shader &) = delete;
	Shader & operator= (Shader &&) = delete;

	void useShader();
};

