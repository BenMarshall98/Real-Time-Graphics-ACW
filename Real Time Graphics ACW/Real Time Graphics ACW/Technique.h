#pragma once

#include "Object.h"
#include "Shader.h"

class Technique
{
protected:
	std::shared_ptr<Shader> mNormalShader;
	std::shared_ptr<Shader> mDeferredShader;
	std::shared_ptr<Shader> mDirectionalShader;
	std::shared_ptr<Shader> mOmniDirectionalShader;
	
public:
	Technique(std::shared_ptr<Shader> pNormalShader, std::shared_ptr<Shader> pDeferredShader, std::shared_ptr<Shader> pDirectionalShader, std::shared_ptr<Shader> pOmniDirectionalShader);
	virtual ~Technique();

	virtual void render(std::shared_ptr<Object> & pObject, bool pDeferred) = 0;
	virtual void renderDirectionalShadow(std::shared_ptr<Object> & pObject) = 0;
	virtual void renderOmniDirectionalShadow(std::shared_ptr<Object> & pObject) = 0;
};

