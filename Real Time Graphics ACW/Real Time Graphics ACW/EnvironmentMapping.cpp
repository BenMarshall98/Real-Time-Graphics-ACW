#include "EnvironmentMapping.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "DX11Render.h"

EnvironmentMapping::EnvironmentMapping() : mFramebuffer(std::make_unique<Framebuffer>())
{
	std::shared_ptr<Shader> normalShader;
	std::shared_ptr<Shader> deferredShader;
	std::shared_ptr<Shader> directionalShader;
	std::shared_ptr<Shader> omniDirectionalShader;
	std::shared_ptr<Shader> directionalSimpleShader;
	std::shared_ptr<Shader> pointSimpleShader;

	getNormalShader(normalShader);
	getDeferredShader(deferredShader);
	getDirectionalShader(directionalShader);
	getOmniDirectionalShader(omniDirectionalShader);
	getDirectionalSimpleShader(directionalSimpleShader);
	getPointSimpleShader(pointSimpleShader);

	ResourceManager::instance()->loadShader(normalShader,"EnvironmentVertexShader.hlsl", "EnvironmentFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(deferredShader, "EnvironmentDeferredVertexShader.hlsl", "EnvironmentDeferredFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(directionalShader, "NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(omniDirectionalShader, "NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl");
	ResourceManager::instance()->loadShader(mSetupShader, "EnvironmentSetupVertexShader.hlsl", "EnvironmentSetupFragmentShader.hlsl", "EnvironmentSetupGeometryShader.hlsl");
	ResourceManager::instance()->loadShader(directionalSimpleShader, "NormalDirectionalSimpleShadowVertexShader.hlsl", "NormalDirectionalSimpleShadowFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(pointSimpleShader, "NormalPointSimpleShadowVertexShader.hlsl", "NormalPointSimpleShadowFragmentShader.hlsl");

	setNormalShader(normalShader);
	setDeferredShader(deferredShader);
	setDirectionalShader(directionalShader);
	setOmniDirectionalShader(omniDirectionalShader);
	setDirectionalSimpleShader(directionalSimpleShader);
	setPointSimpleShader(pointSimpleShader);

	if (!mFramebuffer->loadFramebuffer(true, false, 1024, 1024, {{0.0f, 0.0f, 0.0f, 0.0f}}, TextureType::TEXTURE_CUBE))
	{
		mFramebuffer.reset();
	}
}

EnvironmentMapping::~EnvironmentMapping() = default;

void EnvironmentMapping::render(const std::shared_ptr<Shape>& pShape, const bool pDeferred, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	DirectX::XMFLOAT4X4 matrix;
	pShape->getCurrentMatrix(matrix);

	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMLoadFloat4x4(&matrix)));
	
	EnvironmentMatrixBuffer mb;
	XMStoreFloat4x4(&mb.mPerspective, XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, static_cast<float>(mFramebuffer->getWidth()) / static_cast<float>(mFramebuffer->getHeight()), 0.01f, 20.0f)));
	XMStoreFloat4x4(&mb.mView[0], XMMatrixTranspose(DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(position.x, position.y,position.z, 0.0f), DirectX::XMVectorSet(position.x + 1.0f, position.y + 0.0f, position.z + 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))));
	XMStoreFloat4x4(&mb.mView[1], XMMatrixTranspose(DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(position.x, position.y,position.z, 0.0f), DirectX::XMVectorSet(position.x - 1.0f, position.y + 0.0f, position.z + 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))));
	XMStoreFloat4x4(&mb.mView[2], XMMatrixTranspose(DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(position.x, position.y,position.z, 0.0f), DirectX::XMVectorSet(position.x + 0.0f, position.y + 1.0f, position.z + 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f))));
	XMStoreFloat4x4(&mb.mView[3], XMMatrixTranspose(DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(position.x, position.y,position.z, 0.0f), DirectX::XMVectorSet(position.x + 0.0f, position.y - 1.0f, position.z + 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f))));
	XMStoreFloat4x4(&mb.mView[4], XMMatrixTranspose(DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(position.x, position.y,position.z, 0.0f), DirectX::XMVectorSet(position.x + 0.0f, position.y + 0.0f, position.z + 1.0f, 0.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))));
	XMStoreFloat4x4(&mb.mView[5], XMMatrixTranspose(DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(position.x, position.y,position.z, 0.0f), DirectX::XMVectorSet(position.x + 0.0f, position.y + 0.0f, position.z - 1.0f, 0.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))));

	Dx11Render::instance()->useEnvironmentMappingBuffer(mb);
	
	mFramebuffer->useFramebuffer();
	mSetupShader->useShader();

	std::vector<std::shared_ptr<Shape>> shapes;
	RenderManager::instance()->getStaticShape(shapes);

	for (auto i = 0u; i < shapes.size(); i++)
	{
		shapes[i]->render();
	}
	
	mFramebuffer->useTexture(24);
	pCurrentFramebuffer->useFramebuffer(false);

	if (pDeferred)
	{
		useDeferredShader();
	}
	else
	{
		useNormalShader();
	}

	pShape->render();
	mFramebuffer->releaseTexture(24);
}

void EnvironmentMapping::renderDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useDirectionalShader();
	pShape->render();
}

void EnvironmentMapping::renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useOmniDirectionalShader();
	pShape->render();
}

void EnvironmentMapping::renderDirectionalSimpleShadow(const std::shared_ptr<Shape> & pShape)
{
	useDirectionalSimpleShader();
	pShape->render();
}

void EnvironmentMapping::renderPointSimpleShadow(const std::shared_ptr<Shape> & pShape)
{
	usePointSimpleShader();
	pShape->render();
}

bool EnvironmentMapping::renderPostprocessing(const std::unique_ptr<Framebuffer> &)
{
	return false;
}

void EnvironmentMapping::renderTransparent(const std::shared_ptr<Shape> &, const std::unique_ptr<Framebuffer> &)
{
	//Do Nothing
}