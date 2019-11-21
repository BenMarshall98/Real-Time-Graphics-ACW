#include "DirectionalLight.h"
#include "DX11Render.h"

DirectionalLight::DirectionalLight(const DirectX::XMFLOAT4 & pColor, const DirectX::XMFLOAT3 & pDirection) :
	mColor(pColor), mDirection(pDirection), mDirty(true)
{
}

void DirectionalLight::use(DirectionalLight* pDirectionalLight, ID3D11Buffer* pDeviceBuffer)
{
	DirectionalLightBuffer db;
	ZeroMemory(&db, sizeof db);

	if (pDirectionalLight)
	{
		if (pDirectionalLight->mDirty)
		{
			db.mDirection = pDirectionalLight->mDirection;
			db.mColor = pDirectionalLight->mColor;
			db.mIsUsed = true;
			
			pDirectionalLight->mDirty = false;
		}
		else
		{
			return;
		}
	}
	else
	{
		db.mIsUsed = false;
	}

	auto deviceContext = Dx11Render::instance()->getDeviceContext();

	deviceContext->UpdateSubresource(pDeviceBuffer, 0, nullptr, &db, 0, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &pDeviceBuffer);
}
