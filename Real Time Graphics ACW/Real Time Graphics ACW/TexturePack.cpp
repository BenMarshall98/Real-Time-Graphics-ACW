#include "TexturePack.h"
#include <utility>

TexturePack::TexturePack(std::shared_ptr<Texture> pDiffuseTexture, std::shared_ptr<Texture> pSpecularTexture, std::shared_ptr<Texture> pNormalTexture, std::shared_ptr<Texture> pHeightTexture) :
	mDiffuseTexture(std::move(pDiffuseTexture)), mSpecularTexture(std::move(pSpecularTexture)), mNormalTexture(std::move(pNormalTexture)), mHeightTexture(std::move(pHeightTexture))
{
}

void TexturePack::use() const
{
	mDiffuseTexture->useFragment(0);
	mSpecularTexture->useFragment(1);
	mNormalTexture->useFragment(2);
	mHeightTexture->useDomain(0);
}

std::istream & operator>>(std::istream & pIn, std::unique_ptr<TexturePack> & pTexturePack)
{
	//TODO: Implement
	return pIn;
}