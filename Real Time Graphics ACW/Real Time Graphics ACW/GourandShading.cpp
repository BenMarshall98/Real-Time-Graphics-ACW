#include "GourandShading.h"

GourandShading::GourandShading() : Technique(new Shader(L"VertexShader.hlsl", L"PixelShader.hlsl"), nullptr)
{
}

GourandShading::~GourandShading()
{
}

void GourandShading::render(Object * pObject, bool pDeferred)
{
	mShader->useShader();
	//pObject->render();
}