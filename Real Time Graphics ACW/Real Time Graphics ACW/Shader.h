#pragma once

#include <string>
#include <d3d11_1.h>
#include <WRL/client.h>

class Shader
{
	static const D3D11_INPUT_ELEMENT_DESC layout[];
	static const D3D11_INPUT_ELEMENT_DESC particle_layout[];
	
	Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> mGeometryShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11HullShader> mHullShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DomainShader> mDomainShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> mComputeShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout = nullptr;

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
	bool loadShader(const std::string & pVertexFile, const std::string & pFragmentFile, const std::string & pGeometryFile, const std::string & pHullFile, const std::string & pDomainFile);
	bool loadShader(const std::string & pComputeFile);
	bool loadParticleShader(const std::string & pVertexFile, const std::string & pFragmentFile);

	void useShader();
};

