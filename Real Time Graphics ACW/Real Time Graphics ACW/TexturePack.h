#pragma once

#include "Texture.h"

class TexturePack
{
	std::shared_ptr<Texture> mDiffuseTexture;
	std::shared_ptr<Texture> mSpecularTexture;
	std::shared_ptr<Texture> mNormalTexture;
	std::shared_ptr<Texture> mHeightTexture;
	
public:
	TexturePack(std::shared_ptr<Texture> pDiffuseTexture, std::shared_ptr<Texture> pSpecularTexture,
	            std::shared_ptr<Texture> pNormalTexture, std::shared_ptr<Texture> pHeightTexture);
	TexturePack() = default;
	~TexturePack() = default;

	TexturePack(const TexturePack&) = delete;
	TexturePack(TexturePack &&) = delete;
	TexturePack & operator= (const TexturePack &) = delete;
	TexturePack & operator= (TexturePack &&) = delete;

	void setDiffuseTexture(const std::shared_ptr<Texture> & pDiffuseTexture)
	{
		mDiffuseTexture = pDiffuseTexture;
	}

	void setSpecularTexture(const std::shared_ptr<Texture> & pSpecularTexture)
	{
		mSpecularTexture = pSpecularTexture;
	}

	void setNormalTexture(const std::shared_ptr<Texture> & pNormalTexture)
	{
		mNormalTexture = pNormalTexture;
	}

	void setHeightTexture(const std::shared_ptr<Texture> & pHeightTexture)
	{
		mHeightTexture = pHeightTexture;
	}

	void use() const;
};

std::istream & operator>>(std::istream & pIn, std::unique_ptr<TexturePack> & pTexturePack);