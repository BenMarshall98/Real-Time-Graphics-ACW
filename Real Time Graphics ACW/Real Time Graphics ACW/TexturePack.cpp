#include "TexturePack.h"
#include <utility>
#include "ResourceManager.h"

TexturePack::TexturePack(std::shared_ptr<Texture> pDiffuseTexture, std::shared_ptr<Texture> pSpecularTexture, std::shared_ptr<Texture> pNormalTexture, std::shared_ptr<Texture> pHeightTexture) :
	mDiffuseTexture(std::move(pDiffuseTexture)), mSpecularTexture(std::move(pSpecularTexture)), mNormalTexture(std::move(pNormalTexture)), mHeightTexture(std::move(pHeightTexture))
{
}

void TexturePack::use() const
{
	mDiffuseTexture->useFragment(20);
	mSpecularTexture->useFragment(21);
	mNormalTexture->useFragment(22);
	mHeightTexture->useDomain(0);
}

std::istream & operator>>(std::istream & pIn, const std::unique_ptr<TexturePack> & pTexturePack)
{
	std::string type;

	pIn >> type;


	//Diffuse
	pIn >> type >> type;
	std::shared_ptr<Texture> texture;

	ResourceManager::instance()->loadTexture(texture, type);
	
	pTexturePack->setDiffuseTexture(texture);

	pIn >> type >> type;

	//Specular
	ResourceManager::instance()->loadTexture(texture, type);

	pTexturePack->setSpecularTexture(texture);

	pIn >> type >> type;

	//Normal
	ResourceManager::instance()->loadTexture(texture, type);

	pTexturePack->setNormalTexture(texture);

	pIn >> type >> type;

	//Height
	ResourceManager::instance()->loadTexture(texture, type);

	pTexturePack->setHeightTexture(texture);

	pIn >> type;
	
	return pIn;
}