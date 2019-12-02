#include "Technique.h"

Technique::Technique(std::shared_ptr<Shader> pShader, std::shared_ptr<Shader> pDeferredShader) :
	mShader(pShader), mDeferredShader(pDeferredShader)
{
}

Technique::~Technique()
{
}
