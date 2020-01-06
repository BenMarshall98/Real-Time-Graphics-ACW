#pragma once
#include <istream>
#include "SceneGraphNode.h"

class ConfigLoader
{
	ConfigLoader() = default;
	~ConfigLoader() = default;
public:
	static void readScene(std::istream & pIn, const std::unique_ptr<SceneGraphNode> & pNode);
};

