#pragma once
#include "Technique.h"

class BumpMapping final : public Technique
{
public:
	BumpMapping();
	~BumpMapping();

	void render(std::shared_ptr<Object> & pObject, bool pDeferred) override;
};

