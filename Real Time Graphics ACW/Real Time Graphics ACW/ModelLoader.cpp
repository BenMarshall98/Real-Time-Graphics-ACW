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

Model * ModelLoader::CreateSphere(float radius, int segments)
{
	std::vector<DirectX::XMFLOAT3> positions;
	std::vector<DirectX::XMFLOAT3> normals;
	std::vector<DirectX::XMFLOAT2> texCoords;
	std::vector<unsigned int> indices;

	float angle = PI / segments;

	for (int i = 0; i <= segments; i++)
	{
		for (int j = 0; j < (segments * 2); j++)
		{
			float x = cos(j * angle) * sin(i * angle);
			float y = sin(j * angle) * sin(i * angle);
			float z = cos(i * angle);

			positions.push_back(DirectX::XMFLOAT3(x * radius, y * radius, z * radius));
			normals.push_back(DirectX::XMFLOAT3(x, y, z));

			float u = 1.0f / (segments * 2) * j;
			float v = 1.0f / segments * i;

			texCoords.push_back(DirectX::XMFLOAT2(u, v));
		}
	}

	int count = 0;
	int limit = (segments * 2) * 2;

	for (int i = 0; i < (segments * 2); i++)
	{
		indices.push_back(count);
		indices.push_back(count + (segments * 2));

		if ((count + (segments * 2) + 1) == limit)
		{
			indices.push_back(count + 1);
		}
		else
		{
			indices.push_back(count + (segments * 2) + 1);
		}
		count++;
	}

	for (int i = 1; i < (segments - 1); i++)
	{
		limit += (segments * 2);

		for (int j = 0; j < (segments * 2); j++)
		{
			indices.push_back(count);
			indices.push_back(count + (segments * 2));

			if (count + (segments * 2) + 1 == limit)
			{
				indices.push_back(count + 1);
			}
			else
			{
				indices.push_back(count + (segments * 2) + 1);
			}

			indices.push_back(count);

			if (count + (segments * 2) + 1 == limit)
			{
				indices.push_back(count + 1);
				indices.push_back(count + 1 - (segments * 2));
			}
			else
			{
				indices.push_back(count + (segments * 2) + 1);
				indices.push_back(count + 1);
			}
			count++;
		}
	}


	for (int i = 0; i < (segments * 2); i++)
	{
		indices.push_back(count);
		indices.push_back(count + (segments * 2));

		if (count + 1 == limit)
		{
			indices.push_back(count + 1 - (segments * 2));
		}
		else
		{
			indices.push_back(count + 1);
		}
		count++;
	}

	return new Model(positions, normals, texCoords, indices);
}

class Model* ModelLoader::LoadModelFromFile(const std::string& pModelFile)
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
			const auto x = mesh->mTextureCoords[i]->x;
			const auto y = mesh->mTextureCoords[i]->y;
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

	//TODO: sort out
	return nullptr;
}
