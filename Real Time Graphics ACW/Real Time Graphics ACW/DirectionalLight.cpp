#include "DirectionalLight.h"
#include "DX11Render.h"

DirectionalLight::DirectionalLight(DirectX::XMFLOAT4 pColor, DirectX::XMFLOAT3 pDirection) :
	mColor(pColor), mDirection(pDirection), mDirty(true)
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::use(DirectionalLight* pDirectionalLight, ID3D11Buffer* pDeviceBuffer)
{
	DirectionalLightBuffer db;
	ZeroMemory(&db, sizeof(db));

	if (pDirectionalLight)
	{
		if (pDirectionalLight->mDirty)
		{
			db.mDirection = pDirectionalLight->mDirection;
			db.mColor = pDirectionalLight->mColor;
			db.isUsed = true;
			
			pDirectionalLight->mDirty = false;
		}
		else
		{
			return;
		}
	}
	else
	{
		db.isUsed = false;
	}

	ID3D11DeviceContext * deviceContext = DX11Render::Instance()->GetDeviceContext();

	deviceContext->UpdateSubresource(pDeviceBuffer, 0, nullptr, &db, 0, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &pDeviceBuffer);
}
