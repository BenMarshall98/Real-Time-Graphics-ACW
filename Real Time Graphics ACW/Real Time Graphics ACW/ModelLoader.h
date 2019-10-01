#pragma once

#include "Model.h"

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	static Model * CreateSphere(float radius, int segments);
};

