#pragma once

#include "Object.h"
#include "Shader.h"

class Technique
{
protected:
	Shader * mShader;
	Shader * mDeferredShader;
	
public:
	Technique(Shader * pShader, Shader * pDeferredShader);
	~Technique();

	virtual void render(Object * pObject, bool pDeferred) = 0;
};

