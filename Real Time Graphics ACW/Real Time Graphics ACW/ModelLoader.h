#pragma once

#include "Model.h"

class ModelLoader
{
public:
	static Model * loadModelFromFile(const std::string & pModelFile);
};

