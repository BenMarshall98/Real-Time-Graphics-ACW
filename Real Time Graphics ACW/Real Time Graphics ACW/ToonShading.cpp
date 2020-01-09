#include "ToonShading.h"
#include "ResourceManager.h"
#include "DX11Render.h"

ToonShading::ToonShading() : mFramebuffer(std::make_unique<Framebuffer>())
{
	std::shared_ptr<Shader> normalShader;
	std::shared_ptr<Shader> directionalShader;
	std::shared_ptr<Shader> omniDirectionalShader;
	std::shared_ptr<Shader> directionalSimpleShader;
	std::shared_ptr<Shader> pointSimpleShader;

	getNormalShader(normalShader);
	getDirectionalShader(directionalShader);
	getOmniDirectionalShader(omniDirectionalShader);
	getDirectionalSimpleShader(directionalSimpleShader);
	getPointSimpleShader(pointSimpleShader);

	ResourceManager::instance()->loadShader(normalShader, "ToonVertexShader.hlsl", "ToonFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(directionalShader, "NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(omniDirectionalShader, "NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl");
	ResourceManager::instance()->loadShader(directionalSimpleShader, "NormalDirectionalSimpleShadowVertexShader.hlsl", "NormalDirectionalSimpleShadowFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(pointSimpleShader, "NormalPointSimpleShadowVertexShader.hlsl", "NormalPointSimpleShadowFragmentShader.hlsl");

	setNormalShader(normalShader);
	setDirectionalShader(directionalShader);
	setOmniDirectionalShader(omniDirectionalShader);
	setDirectionalSimpleShader(directionalSimpleShader);
	setPointSimpleShader(pointSimpleShader);

	ResourceManager::instance()->loadModel(mRenderPlane, "plane.obj");
	ResourceManager::instance()->loadShader(mPostShader, "PostVertexShader.hlsl", "PostFragmentShader.hlsl");

	if (!mFramebuffer->loadFramebuffer(true, true, {{0.0f, 0.0f, 0.0f, 0.0f}}))
	{
		mFramebuffer.reset();
	}
}

ToonShading::~ToonShading() = default;

void ToonShading::render(const std::shared_ptr<Shape>& pShape, bool, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	mFramebuffer->useFramebuffer();
	
	useNormalShader();
	pShape->render();

	pCurrentFramebuffer->useFramebuffer(false);
}

void ToonShading::renderDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useDirectionalShader();
	pShape->render();
}

void ToonShading::renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useOmniDirectionalShader();
	pShape->render();
}

void ToonShading::renderDirectionalSimpleShadow(const std::shared_ptr<Shape> & pShape)
{
	useDirectionalSimpleShader();
	pShape->render();
}

void ToonShading::renderPointSimpleShadow(const std::shared_ptr<Shape> & pShape)
{
	usePointSimpleShader();
	pShape->render();
}

bool ToonShading::renderPostprocessing(const std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	mFramebuffer->useTexture(14);

	if (pCurrentFramebuffer == nullptr)
	{
		Dx11Render::instance()->bindDefaultFramebuffer();
	}
	else
	{
		pCurrentFramebuffer->useFramebuffer();
	}

	mPostShader->useShader();

	mRenderPlane->render();

	mFramebuffer->releaseTexture(14);

	return true;
}

void ToonShading::renderTransparent(const std::shared_ptr<Shape> &, const std::unique_ptr<Framebuffer> &)
{
	//Do Nothing
}