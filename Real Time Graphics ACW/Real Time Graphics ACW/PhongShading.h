#pragma once
#include "Technique.h"

class PhongShading final : public Technique
{
public:
	PhongShading();
	~PhongShading();

	void render(std::shared_ptr<Object>& pObject, bool pDeferred) override;
};

