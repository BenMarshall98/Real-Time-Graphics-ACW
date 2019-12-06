#include "PointLight.h"
#include "DX11Render.h"

PointLight::PointLight(DirectX::XMFLOAT4 pColor, DirectX::XMFLOAT3 pPosition) :
	mColor(pColor), mPosition(pPosition), mDirty(true)
{
}

void PointLight::use(ID3D11Buffer * pDeviceBuffer)
{
	static 
	PointLightBuffer pb;
	ZeroMemory(&pb, sizeof(pb));

	if (mDirty)
	{
		//pb.mPosition = mPosition;
		pb.mColor = mColor;
		pb.mIsUsed = true;

		mDirty = false;
	}
	else
	{
		return;
	}

	auto deviceContext = Dx11Render::instance()->getDeviceContext();

	deviceContext->UpdateSubresource(pDeviceBuffer, 0, nullptr, &pb, 0, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &pDeviceBuffer);
}
