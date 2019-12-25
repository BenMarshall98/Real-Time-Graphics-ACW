#include "Cuboid.h"
#include "ResourceManager.h"
#include "InkCubeRender.h"

Cuboid::Cuboid(std::unique_ptr<TexturePack> pTexturePack, std::unique_ptr<Material> pMaterial) :
	Shape(ResourceManager::instance()->loadModel("cube.obj"), std::move(pTexturePack), std::move(pMaterial), std::make_unique<InkCubeRender>())
{
}

Cuboid::Cuboid() :
	Shape(ResourceManager::instance()->loadModel("cube.obj"), nullptr, nullptr, std::make_unique<InkCubeRender>())
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