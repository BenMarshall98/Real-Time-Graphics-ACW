#include "Sphere.h"
#include "ResourceManager.h"

Sphere::Sphere(std::unique_ptr<TexturePack> pTexturePack, std::unique_ptr<Material> pMaterial) :
	Shape(ResourceManager::instance()->loadModel("sphere.obj"), std::move(pTexturePack), std::move(pMaterial))
{
}

Sphere::Sphere() :
	Shape(ResourceManager::instance()->loadModel("sphere.obj"), nullptr, nullptr)
{
	
}

void Sphere::explode()
{
	//TODO: Implement
}

void Sphere::collideWith(Particle pParticle)
{
	//TODO: Implement	
}