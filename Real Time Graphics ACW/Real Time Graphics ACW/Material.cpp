#include "Material.h"
#include <string>
#include "DX11Render.h"

Material::Material(const DirectX::XMFLOAT3& pAmbient, const DirectX::XMFLOAT3& pDiffuse, const DirectX::XMFLOAT3& pSpecular, const float pShininess) :
	mAmbient(pAmbient), mDiffuse(pDiffuse), mSpecular(pSpecular), mShininess(pShininess)
{
}

Material::Material() : mAmbient(), mDiffuse(), mSpecular(), mShininess()
{
	
}

void Material::use() const
{
	const MaterialBuffer mb = {
		DirectX::XMFLOAT4(mAmbient.x, mAmbient.y, mAmbient.z, 1.0f),
		DirectX::XMFLOAT4(mDiffuse.x, mDiffuse.y, mDiffuse.z, 1.0f),
		DirectX::XMFLOAT4(mSpecular.x, mSpecular.y, mSpecular.z, 1.0f),
		mShininess,
		DirectX::XMFLOAT3()
	};

	Dx11Render::instance()->useMaterialBuffer(mb);
}

std::istream& operator>>(std::istream& pIn, std::unique_ptr<Material>& pMaterial)
{
	std::string s;
	char c;

	pIn >> s;

	float x, y, z;
	pIn >> s >> x >> c >> y >> c >> z;
	pMaterial->setAmbient(DirectX::XMFLOAT3(x, y, z));

	pIn >> s >> x >> c >> y >> c >> z;
	pMaterial->setDiffuse(DirectX::XMFLOAT3(x, y, z));

	pIn >> s >> x >> c >> y >> c >> z;
	pMaterial->setSpecular(DirectX::XMFLOAT3(x, y, z));

	float shininess;
	pIn >> s >> shininess;
	pMaterial->setShininess(shininess);

	pIn >> s;

	return pIn;
}
