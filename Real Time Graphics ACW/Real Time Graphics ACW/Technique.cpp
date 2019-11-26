#include "Technique.h"

Technique::Technique(Shader * pShader, Shader * pDeferredShader) :
	mShader(pShader), mDeferredShader(pDeferredShader)
{
}

Technique::~Technique()
{
}
