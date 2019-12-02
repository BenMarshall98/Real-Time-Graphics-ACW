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
	~Technique();

	virtual void render(Object * pObject, bool pDeferred) = 0;
};

