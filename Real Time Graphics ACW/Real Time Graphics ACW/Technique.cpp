#include "Technique.h"
#include <utility>

Technique::Technique(std::shared_ptr<Shader> pNormalShader, std::shared_ptr<Shader> pDeferredShader,
    std::shared_ptr<Shader> pDirectionalShader, std::shared_ptr<Shader> pOmniDirectionalShader) :
	mNormalShader(std::move(pNormalShader)), mDeferredShader(std::move(pDeferredShader)),
	mDirectionalShader(std::move(pDirectionalShader)), mOmniDirectionalShader(std::move(pOmniDirectionalShader))
{
}