#include "Plane.h"
#include "ResourceManager.h"
#include "InkPlaneRender.h"

Plane::Plane(std::unique_ptr<TexturePack> pTexturePack, std::unique_ptr<Material> pMaterial) :
	Shape(ResourceManager::instance()->loadModel("plane.obj"), std::move(pTexturePack), std::move(pMaterial), std::make_unique<InkPlaneRender>())
{
}

Plane::Plane() :
	Shape(ResourceManager::instance()->loadModel("plane.obj"), nullptr, nullptr, std::make_unique<InkPlaneRender>())
{
}

void Plane::collideWith(Particle pParticle)
{
	//TODO: Implement
}