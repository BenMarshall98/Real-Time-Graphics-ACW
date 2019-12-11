#pragma once
#include <string>
#include <d3d11.h>

class Texture
{
	ID3D11ShaderResourceView * mTexture = nullptr;
	ID3D11SamplerState * mSampler = nullptr;
	
public:
	Texture() = default;
	~Texture();
	Texture(const Texture & pTexture) = delete;
	Texture & operator= (const Texture & pTexture) = delete;

	bool loadTexture(const std::string & pTextureFile);
	void useFragment(unsigned int pIndex) const;
	void useDomain(unsigned int pIndex) const;
};

