#include "Shape.h"
#include "DX11Render.h"

Shape::Shape(std::unique_ptr<TexturePack> pTexturePack,
	std::unique_ptr<Material> pMaterial) :
	mTexturePack(std::move(pTexturePack)),
	mMaterial(std::move(pMaterial)), mCurrentMatrix(), mPreviousMatrix()
{
}

Shape::Shape() : mCurrentMatrix(), mPreviousMatrix(), mCurrentRotation(), mPreviousRotation()
{
}

Shape::~Shape() = default;


void Shape::render(const bool pTesselated) const
{
	const auto matrix = XMLoadFloat4x4(&mCurrentMatrix);

	const ModelBuffer mb = {
		XMMatrixTranspose(matrix),
		XMMatrixInverse(nullptr, matrix)
	};

	Dx11Render::instance()->useModelBuffer(mb);

	if (mMaterial)
	{
		mMaterial->use();
	}

	if (mTexturePack)
	{
		mTexturePack->use();
	}

	if (mModel)
	{
		mModel->render(pTesselated);
	}
}

std::istream& operator>>(std::istream& pIn, const std::shared_ptr<Shape>& pShape)
{
	char c;
	std::string s;

	pIn >> c;

	pIn >> s;

	if (s == "Material")
	{
		auto material = std::make_unique<Material>();
		pIn >> material;

		pShape->setMaterial(std::move(material));
	}

	pIn >> s;

	if (s == "Texture")
	{
		auto texture = std::make_unique<TexturePack>();
		pIn >> texture;

		pShape->setTexturePack(std::move(texture));
	}

	pIn >> c;

	return pIn;
}
