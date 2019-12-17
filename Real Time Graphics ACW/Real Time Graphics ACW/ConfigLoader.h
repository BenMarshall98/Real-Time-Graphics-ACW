#pragma once
#include <istream>
#include "SceneGraphNode.h"

class ConfigLoader
{
public:
	static void readScene(std::istream & pIn, std::unique_ptr<SceneGraphNode> & pNode);
};

