#pragma once

#include "Model.h"

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	static Model * LoadModelFromFile(const std::string & pModelFile);
};

