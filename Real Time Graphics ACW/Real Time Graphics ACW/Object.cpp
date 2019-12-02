#include "Object.h"

#include <string>
#include "ModelLoader.h"
#include "ResourceManager.h"

Object::Object(const std::string & pModelFile, const DirectX::XMFLOAT3 & pAmbient,
	const DirectX::XMFLOAT3 & pDiffuse, const DirectX::XMFLOAT3 & pSpecular,
	const float & pShininess) :
	mModel(ResourceManager::instance()->loadModel(pModelFile)), mAmbient(pAmbient), mDiffuse(pDiffuse), mSpecular(pSpecular), mShininess(pShininess)
{
}

Object::Object() : mModel(nullptr), mMatrix(), mAmbient(), mDiffuse(), mSpecular(), mShininess()
{
}

std::istream& operator>>(std::istream& pIn, Object & pObject)
{
	std::string s;
	char c;

	std::string modelFile;
	pIn >> s >> modelFile;
	pObject.setModel(ResourceManager::instance()->loadModel(modelFile));

	float x, y, z;
	pIn >> s >> x >> c >> y >> c >> z;
	const auto ambient = DirectX::XMFLOAT3(x, y, z);
	pObject.setAmbient(ambient);
	
	pIn >> s >> x >> c >> y >> c >> z;
	const auto diffuse = DirectX::XMFLOAT3(x, y, z);
	pObject.setDiffuse(diffuse);

	pIn >> s >> x >> c >> y >> c >> z;
	const auto specular = DirectX::XMFLOAT3(x, y, z);
	pObject.setSpecular(specular);

	float shininess;
	pIn >> s >> shininess;
	pObject.setShininess(shininess);

	pIn >> s;

	if (s == "Texture")
	{
		//TODO: Look at texture pack
	}

	return pIn;
}