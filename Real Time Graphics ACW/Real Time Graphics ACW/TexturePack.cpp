#include "TexturePack.h"

TexturePack::TexturePack(const std::shared_ptr<Texture>& pDiffuseTexture, const std::shared_ptr<Texture>& pSpecularTexture, const std::shared_ptr<Texture>& pNormalTexture, const std::shared_ptr<Texture>& pHeightTexture) :
	mDiffuseTexture(pDiffuseTexture), mSpecularTexture(pSpecularTexture), mNormalTexture(pNormalTexture), mHeightTexture(pHeightTexture)
{
}

void TexturePack::use() const
{
	mDiffuseTexture->useFragment(0);
	mSpecularTexture->useFragment(1);
	mNormalTexture->useFragment(2);
	mHeightTexture->useDomain(0);
}