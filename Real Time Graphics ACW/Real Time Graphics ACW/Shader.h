#pragma once

#include <string>
#include <d3d11_1.h>

class Shader
{
	ID3D11VertexShader * mVertexShader = nullptr;
	ID3D11PixelShader * mPixelShader = nullptr;
	ID3D11GeometryShader * mGeometryShader = nullptr;
	ID3D11HullShader * mHullShader = nullptr;
	ID3D11DomainShader * mDomainShader = nullptr;
	ID3D11ComputeShader * mComputeShader = nullptr;
	ID3D11InputLayout * mInputLayout = nullptr;

	static HRESULT compileShaderFromFile(const std::wstring & pFileName, const char * pTarget, ID3DBlob** pShaderBlob);
	
public:
	Shader() = default;
	~Shader() = default;

	Shader(const Shader&) = delete;
	Shader(Shader &&) = delete;
	Shader & operator= (const Shader &) = delete;
	Shader & operator= (Shader &&) = delete;

	bool loadShader(const std::string & pVertexFile, const std::string & pFragmentFile);
	bool loadShader(const std::string & pVertexFile, const std::string & pFragmentFile, const std::string & pGeometryFile);
	bool loadShader(const std::string & pVertexFile, const std::string & pFragmentFile, const std::string & pHullFile, const std::string & pDomainFile);
	bool loadShader(const std::string & pComputeFile);

	void useShader();
};

