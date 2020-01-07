#pragma once

#include "Model.h"
#include "Material.h"
#include "TexturePack.h"
#include "Particle.h"

#include <memory>

struct ModelBuffer
{
	DirectX::XMMATRIX mModel;
	DirectX::XMMATRIX mModelInverse;
};

class Shape
{
	std::shared_ptr<Model> mModel;
	std::unique_ptr<TexturePack> mTexturePack;
	std::unique_ptr<Material> mMaterial;

	DirectX::XMFLOAT4X4 mCurrentMatrix;
	DirectX::XMFLOAT4X4 mPreviousMatrix;

	DirectX::XMFLOAT4X4 mCurrentRotation;
	DirectX::XMFLOAT4X4 mPreviousRotation;
	
protected:
	
	void getModel(std::shared_ptr<Model> & pModel) const
	{
		pModel = mModel;
	}
	
public:
	Shape(std::unique_ptr<TexturePack> & pTexturePack, std::unique_ptr<Material> & pMaterial);
	Shape();
	virtual ~Shape();

	Shape(const Shape &) = delete;
	Shape(Shape &&) = delete;
	Shape & operator= (const Shape &) = delete;
	Shape & operator= (Shape &&) = delete;

	void setModel(const std::shared_ptr<Model> & pModel)
	{
		mModel = pModel;
	}

	void setTexturePack(std::unique_ptr<TexturePack> & pTexturePack)
	{
		mTexturePack = std::move(pTexturePack);
	}

	void setMaterial(std::unique_ptr<Material> & pMaterial)
	{
		mMaterial = std::move(pMaterial);
	}

	void setMatrix(const DirectX::XMFLOAT4X4 & pMatrix, const DirectX::XMFLOAT4X4 & pRotationMatrix)
	{
		mPreviousMatrix = mCurrentMatrix;
		mCurrentMatrix = pMatrix;

		mPreviousRotation = mCurrentRotation;
		mCurrentRotation = pRotationMatrix;
	}

	void render(bool pTesselated = false) const;

	void getCurrentMatrix(DirectX::XMFLOAT4X4 & pCurrentMatrix) const
	{
		pCurrentMatrix = mCurrentMatrix;
	}

	void getPreviousMatrix(DirectX::XMFLOAT4X4 & pPreviousMatrix) const
	{
		pPreviousMatrix = mPreviousMatrix;
	}

	void getCurrentRotationMatrix(DirectX::XMFLOAT4X4 & pCurrentRotation) const
	{
		pCurrentRotation = mCurrentRotation;
	}

	void getPreviousRotationMatrix(DirectX::XMFLOAT4X4 & pPreviousRotation) const
	{
		pPreviousRotation = mPreviousRotation;
	}

	virtual void collideWith(const Particle & pParticle) = 0;
};

std::istream & operator>>(std::istream & pIn, const std::shared_ptr<Shape> & pShape);