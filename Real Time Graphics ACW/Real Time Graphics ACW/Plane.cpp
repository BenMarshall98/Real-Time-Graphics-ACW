#include "Plane.h"
#include "ResourceManager.h"

Plane::Plane(std::unique_ptr<TexturePack> pTexturePack, std::unique_ptr<Material> pMaterial) :
	Shape(ResourceManager::instance()->loadModel("plane.obj"), std::move(pTexturePack), std::move(pMaterial))
{
}

Plane::Plane() :
	Shape(ResourceManager::instance()->loadModel("plane.obj"), nullptr, nullptr)
{
	
}

void Plane::collideWith(Particle pParticle)
{
	//TODO: Implement
}