#include "Technique.h"

Technique::Technique(std::shared_ptr<Shader> pNormalShader, std::shared_ptr<Shader> pDeferredShader,
	std::shared_ptr<Shader> pDirectionalShader, std::shared_ptr<Shader> pOmniDirectionalShader) :
	mNormalShader(pNormalShader), mDeferredShader(pDeferredShader), mDirectionalShader(pDirectionalShader), mOmniDirectionalShader(pOmniDirectionalShader)
{
	
}


Technique::~Technique()
{
}
