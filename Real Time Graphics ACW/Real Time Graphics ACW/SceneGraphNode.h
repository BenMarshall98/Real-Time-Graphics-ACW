#pragma once
#include <DirectXMath.h>
#include <vector>
#include <iostream>
#include "Object.h"

class SceneGraphNode
{
	std::vector<SceneGraphNode *> mChildren;
	
protected:
	DirectX::XMMATRIX mMatrix;
	
	
public:
	explicit SceneGraphNode(const DirectX::XMMATRIX & pMatrix);
	SceneGraphNode();
	virtual ~SceneGraphNode();
	void addChild(SceneGraphNode * pChild);
	
	virtual void read(std::istream & pIn) = 0;
};

std::istream& operator>>(std::istream& pIn, SceneGraphNode* pNode);
