#pragma once

#include "Object.h"
#include "Shader.h"

class Technique
{
protected:
	std::shared_ptr<Shader> mShader;
	std::shared_ptr<Shader> mDeferredShader;
	
public:
	Technique(std::shared_ptr<Shader> pShader, std::shared_ptr<Shader> pDeferredShader);
	virtual ~Technique();

	virtual void render(std::shared_ptr<Object> & pObject, bool pDeferred) = 0;
};

