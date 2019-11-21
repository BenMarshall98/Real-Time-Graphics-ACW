#pragma once
#include <DirectXMath.h>
#include <vector>
#include <iostream>
#include "Object.h"

class SceneGraphNode
{
	std::vector<std::shared_ptr<SceneGraphNode>> mChildren;
	
protected:
	DirectX::XMFLOAT4X4 mMatrix;
	
	
public:
	explicit SceneGraphNode(const DirectX::XMFLOAT4X4 & pMatrix);
	SceneGraphNode();
	virtual ~SceneGraphNode();

	SceneGraphNode(const SceneGraphNode&) = delete;
	SceneGraphNode(SceneGraphNode &&) = delete;
	SceneGraphNode & operator= (const SceneGraphNode &) = delete;
	SceneGraphNode & operator= (SceneGraphNode &&) = delete;
	
	void addChild(std::shared_ptr<SceneGraphNode> & pChild)
	{
		mChildren.push_back(pChild);
	}
	
	virtual void read(std::istream & pIn) = 0;
};

std::istream& operator>>(std::istream& pIn, SceneGraphNode & pNode);
