#pragma once

#include "Model.h"
#include <memory>

class ModelLoader
{
	ModelLoader() = default;
	~ModelLoader() = default;
	
public:
	static std::shared_ptr<Model> loadModelFromFile(const std::string & pModelFile);
};

