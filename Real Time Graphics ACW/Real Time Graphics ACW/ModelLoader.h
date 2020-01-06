#pragma once

#include "Model.h"
#include <memory>

class ModelLoader
{
	ModelLoader() = default;
	~ModelLoader() = default;
	
public:
	static void loadModelFromFile(const std::string& pModelFile, std::shared_ptr<Model> & pModel);
};

