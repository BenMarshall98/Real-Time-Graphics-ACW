#include "Shape.h"
#include "DX11Render.h"

Shape::Shape(std::shared_ptr<Model> pModel, std::unique_ptr<TexturePack> pTexturePack, std::unique_ptr<Material> pMaterial) :
	mModel(std::move(pModel)), mTexturePack(std::move(pTexturePack)), mMaterial(std::move(pMaterial)), mCurrentMatrix(), mPreviousMatrix()
{
}

Shape::Shape() : mCurrentMatrix(), mPreviousMatrix()
{
}

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