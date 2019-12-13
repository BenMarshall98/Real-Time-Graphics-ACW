#pragma once
#include "Technique.h"

class ToonShading final : public Technique
{
public:
	ToonShading();
	~ToonShading();

	void render(std::shared_ptr<Object> & pObject, bool pDeferred) override;
};

