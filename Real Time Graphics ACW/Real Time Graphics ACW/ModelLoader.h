#pragma once

#include "Model.h"
#include <memory>

class ModelLoader
{
public:
	static std::shared_ptr<Model> loadModelFromFile(const std::string & pModelFile);
};

