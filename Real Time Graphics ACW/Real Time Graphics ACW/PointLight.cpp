#include "PointLight.h"
#include "DX11Render.h"

PointLight::PointLight(DirectX::XMFLOAT4 pColor, DirectX::XMFLOAT3 pPosition) :
	mColor(pColor), mPosition(pPosition), mDirty(true)
{
}

PointLight::~PointLight()
{
}

void PointLight::use(PointLight* pPointLight, ID3D11Buffer* pDeviceBuffer)
{
	static 
	PointLightBuffer pb;
	ZeroMemory(&pb, sizeof(pb));

	if (pPointLight)
	{
		if (pPointLight->mDirty)
		{
			pb.mPosition = pPointLight->mPosition;
			pb.mColor = pPointLight->mColor;
			pb.isUsed = true;

			pPointLight->mDirty = false;
		}
		else
		{
			return;
		}
	}
	else
	{
		pb.isUsed = false;
	}

	ID3D11DeviceContext * deviceContext = DX11Render::Instance()->GetDeviceContext();

	deviceContext->UpdateSubresource(pDeviceBuffer, 0, nullptr, &pb, 0, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &pDeviceBuffer);
}
