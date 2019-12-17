#include "Cuboid.h"
#include "ResourceManager.h"

Cuboid::Cuboid(std::unique_ptr<TexturePack> pTexturePack, std::unique_ptr<Material> pMaterial) :
	Shape(ResourceManager::instance()->loadModel("cube.obj"), std::move(pTexturePack), std::move(pMaterial))
{
}

Cuboid::Cuboid() :
	Shape(ResourceManager::instance()->loadModel("cube.obj"), nullptr, nullptr)
{
	
}


void Cuboid::explode()
{
	//TODO: Implement
}

void Cuboid::collideWith(Particle pParticle)
{
	//TODO: Implement
}