#pragma once
#include <DirectXMath.h>
#include <vector>
#include <iostream>

class SceneGraphNode
{
	std::vector<std::unique_ptr<SceneGraphNode>> mChildren;
	DirectX::XMFLOAT4X4 mMatrix;
	
public:
	explicit SceneGraphNode(const DirectX::XMFLOAT4X4 & pMatrix);
	SceneGraphNode();
	virtual ~SceneGraphNode();

	SceneGraphNode(const SceneGraphNode&) = delete;
	SceneGraphNode(SceneGraphNode &&) = delete;
	SceneGraphNode & operator= (const SceneGraphNode &) = delete;
	SceneGraphNode & operator= (SceneGraphNode &&) = delete;
	
	void addChild(std::unique_ptr<SceneGraphNode> & pChild)
	{
		mChildren.push_back(std::move(pChild));
	}

	void setMatrix(const DirectX::XMFLOAT4X4 & pMatrix)
	{
		mMatrix = pMatrix;
	}

	void getMatrix(DirectX::XMFLOAT4X4 & pMatrix)
	{
		pMatrix = mMatrix;
	}
	
	virtual void update(const DirectX::XMFLOAT4X4 & pFullMatrix, const DirectX::XMFLOAT4X4 & pRotationMatrix);
	virtual void read(std::istream & pIn) = 0; 
};
