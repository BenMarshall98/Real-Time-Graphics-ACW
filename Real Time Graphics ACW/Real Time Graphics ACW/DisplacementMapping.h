#pragma once
#include "Technique.h"

class DisplacementMapping final : public Technique
{
public:
	DisplacementMapping();
	~DisplacementMapping();

	void render(std::shared_ptr<Object> & pObject, bool pDeferred) override;
};

