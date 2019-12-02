#pragma once
#include "Technique.h"

class GourandShading final : public Technique
{
public:
	GourandShading();
	~GourandShading();

	void render(Object * pObject, bool pDeferred) override;
};

