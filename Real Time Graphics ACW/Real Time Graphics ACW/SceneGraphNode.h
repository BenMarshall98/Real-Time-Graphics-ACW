#pragma once
#include <DirectXMath.h>
#include <vector>
#include <iostream>
#include "Object.h"

class SceneGraphNode
{
	std::vector<std::shared_ptr<SceneGraphNode>> mChildren;
	
protected:
	DirectX::XMMATRIX mMatrix;
	
	
public:
	explicit SceneGraphNode(const DirectX::XMMATRIX & pMatrix);
	SceneGraphNode();
	virtual ~SceneGraphNode();
	
	void addChild(std::shared_ptr<SceneGraphNode> & pChild)
	{
		mChildren.push_back(pChild);
	}
	
	virtual void read(std::istream & pIn) = 0;
};

std::istream& operator>>(std::istream& pIn, SceneGraphNode & pNode);
