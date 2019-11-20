#pragma once
#include <string>
#include <d3d11.h>

class Texture
{
	ID3D11ShaderResourceView * mTexture;
	ID3D11SamplerState * mSampler;
	
public:
	Texture(const std::wstring & pTextureFile);
	~Texture();
	Texture(const Texture & pTexture) = delete;
	Texture & operator= (const Texture & pTexture) = delete;

	void use() const;
};

