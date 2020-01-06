#include "GourandShading.h"
#include "ResourceManager.h"
#include "DX11Render.h"

GourandShading::GourandShading() :
	mFramebuffer(std::make_unique<Framebuffer>())
{
	std::shared_ptr<Shader> normalShader;
	std::shared_ptr<Shader> directionalShader;
	std::shared_ptr<Shader> omniDirectionalShader;

	getNormalShader(normalShader);
	getDirectionalShader(directionalShader);
	getOmniDirectionalShader(omniDirectionalShader);

	ResourceManager::instance()->loadShader(normalShader, "GourandVertexShader.hlsl", "GourandFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(directionalShader, "NormalDirectionalShadowVertexShader.hlsl", "NormalDirectionalShadowFragmentShader.hlsl");
	ResourceManager::instance()->loadShader(omniDirectionalShader, "NormalOmniShadowVertexShader.hlsl", "NormalOmniShadowFragmentShader.hlsl", "NormalOmniShadowGeometryShader.hlsl");

	setNormalShader(normalShader);
	setDirectionalShader(directionalShader);
	setOmniDirectionalShader(omniDirectionalShader);
	
	ResourceManager::instance()->loadModel(mRenderPlane, "plane.obj");
	ResourceManager::instance()->loadShader(mPostShader, "PostVertexShader.hlsl", "PostFragmentShader.hlsl");
	
	if (!mFramebuffer->loadFramebuffer(true, true, { { 0.0f, 0.0f, 0.0f, 0.0f } }))
	{
		mFramebuffer.reset();
	}
}

GourandShading::~GourandShading() = default;

void GourandShading::render(const std::shared_ptr<Shape>& pShape, bool, const std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	mFramebuffer->useFramebuffer();
	useNormalShader();
	pShape->render();

	pCurrentFramebuffer->useFramebuffer(false);
}

void GourandShading::renderDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useDirectionalShader();
	pShape->render();
}

void GourandShading::renderOmniDirectionalShadow(const std::shared_ptr<Shape>& pShape)
{
	useOmniDirectionalShader();
	pShape->render();
}

bool GourandShading::renderPostprocessing(const std::unique_ptr<Framebuffer> & pCurrentFramebuffer)
{
	mFramebuffer->useTexture(8);

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

	mFramebuffer->releaseTexture(8);

	return true;
}

void GourandShading::renderTransparent(const std::shared_ptr<Shape> &, const std::unique_ptr<Framebuffer> &)
{
	//Do Nothing
}