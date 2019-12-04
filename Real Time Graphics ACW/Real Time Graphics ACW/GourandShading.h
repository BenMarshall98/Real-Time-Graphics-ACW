#pragma once
#include "Technique.h"

class GourandShading final : public Technique
{
public:
	GourandShading();
	~GourandShading();

	void render(std::shared_ptr<Object> & pObject, bool pDeferred) override;
};

