#define NOMINMAX

#include "ModelLoader.h"

#include <DirectXMath.h>
#include <cmath>
#include <vector>

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"


#define PI 3.14159265358979323846

ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{
}

class Model * ModelLoader::LoadModelFromFile(const std::string& pModelFile)
{
	Assimp::Importer importer;
	const auto scene = importer.ReadFile(pModelFile,
		aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	const auto mesh = scene->mMeshes[0];

	std::vector<DirectX::XMFLOAT3> vertex;
	std::vector<DirectX::XMFLOAT2> texCoords;
	std::vector<DirectX::XMFLOAT3> normals;
	std::vector<DirectX::XMFLOAT3> tangent;
	std::vector<DirectX::XMFLOAT3> biTangent;

	for (auto i = 0u; i < mesh->mNumVertices; i++)
	{
		{
			const auto x = mesh->mVertices[i].x;
			const auto y = mesh->mVertices[i].y;
			const auto z = mesh->mVertices[i].z;
			vertex.emplace_back(DirectX::XMFLOAT3(x, y, z));
		}

		{
			const auto x = mesh->mTextureCoords[0][i].x;
			const auto y = mesh->mTextureCoords[0][i].y;
			texCoords.emplace_back(DirectX::XMFLOAT2(x, y));
		}

		{
			const auto x = mesh->mNormals[i].x;
			const auto y = mesh->mNormals[i].y;
			const auto z = mesh->mNormals[i].z;
			normals.emplace_back(DirectX::XMFLOAT3(x, y, z));
		}

		{
			const auto x = mesh->mTangents[i].x;
			const auto y = mesh->mTangents[i].y;
			const auto z = mesh->mTangents[i].z;
			tangent.emplace_back(DirectX::XMFLOAT3(x, y, z));
		}

		{
			const auto x = mesh->mBitangents[i].x;
			const auto y = mesh->mBitangents[i].y;
			const auto z = mesh->mBitangents[i].z;
			biTangent.emplace_back(DirectX::XMFLOAT3(x, y, z));
		}
	}

	std::vector<WORD> indices;

	for (auto i = 0u; i < mesh->mNumFaces; i++)
	{
		const auto face = mesh->mFaces[i];

		for (auto j = 0u; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	return new Model(vertex, normals, texCoords, tangent, biTangent, indices);
}
