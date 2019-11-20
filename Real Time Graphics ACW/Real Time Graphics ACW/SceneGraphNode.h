#pragma once
#include <DirectXMath.h>
#include <vector>
#include <iostream>
#include "Object.h"

class SceneGraphNode
{
	DirectX::XMMATRIX mMatrix;
	std::vector<SceneGraphNode *> mChildren;
	
public:
	SceneGraphNode();
	~SceneGraphNode();
	void addChild(SceneGraphNode * pChild);
	virtual void read(std::istream & pIn) = 0;
};

std::istream & operator >> (std::istream & pIn, SceneGraphNode * pNode);