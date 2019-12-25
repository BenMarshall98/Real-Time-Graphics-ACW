#pragma once

#include "Model.h"
#include "Material.h"
#include "TexturePack.h"
#include "Particle.h"
#include "InkShapeRender.h"

#include <memory>

struct ModelBuffer
{
	DirectX::XMMATRIX mModel;
	DirectX::XMMATRIX mModelInverse;
};

class Shape
{
	//TODO: Temp
protected:
	std::shared_ptr<Model> mModel;
	std::unique_ptr<TexturePack> mTexturePack;
	std::unique_ptr<Material> mMaterial;
	std::unique_ptr<InkShapeRender> mInkRender;

	DirectX::XMFLOAT4X4 mCurrentMatrix;
	DirectX::XMFLOAT4X4 mPreviousMatrix;
	
public:
	Shape(std::shared_ptr<Model> pModel, std::unique_ptr<TexturePack> pTexturePack, std::unique_ptr<Material> pMaterial, std::unique_ptr<InkShapeRender> pInkRender);
	Shape();
	virtual ~Shape() = default;

	void setModel(const std::shared_ptr<Model> & pModel)
	{
		mModel = pModel;
	}

	void setTexturePack(std::unique_ptr<TexturePack> pTexturePack)
	{
		mTexturePack = std::move(pTexturePack);
	}

	void setMaterial(std::unique_ptr<Material> pMaterial)
	{
		mMaterial = std::move(pMaterial);
	}

	void setMatrix(const DirectX::XMFLOAT4X4 & pMatrix)
	{
		mPreviousMatrix = mCurrentMatrix;
		mCurrentMatrix = pMatrix;
	}

	void render(bool pTesselated = false) const;

	virtual void collideWith(Particle pParticle) = 0;
};

std::istream & operator>>(std::istream & pIn, std::shared_ptr<Shape> & pShape);