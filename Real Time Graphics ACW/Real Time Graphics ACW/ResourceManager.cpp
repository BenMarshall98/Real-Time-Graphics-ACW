#include "ResourceManager.h"
#include "ModelLoader.h"

ResourceManager * ResourceManager::mInstance = nullptr;

std::shared_ptr<Model> ResourceManager::loadModel(const std::string & pModelFile)
{
	const auto result = mModels.find(pModelFile);
	
	if (result != mModels.end())
	{
		return result->second;
	}
	
	auto model = ModelLoader::loadModelFromFile(pModelFile);
	
	if(model.get())
	{
		mModels.insert(std::pair<std::string, std::shared_ptr<Model>>(pModelFile, model));
	}
	
	return model;
}

std::shared_ptr<Texture> ResourceManager::loadTexture(const std::string & pTextureFile)
{
	const auto result = mTextures.find(pTextureFile);
	
	if (result != mTextures.end())
	{
		return result->second;
	}
	
	auto texture = std::make_shared<Texture>();
	
	if (texture->loadTexture(pTextureFile))
	{
		mTextures.insert(std::pair<std::string, std::shared_ptr<Texture>>(pTextureFile, texture));
		return texture;
	}
	return nullptr;
}

std::shared_ptr<Shader> ResourceManager::loadShader(const std::string & pVertexFile, const std::string & pFragmentFile)
{
	const auto concat = pVertexFile + pFragmentFile;
	const auto result = mVfShaders.find(concat);
	
	if (result != mVfShaders.end())
	{
		return result->second;
	}
	
	auto shader = std::make_shared<Shader>();
	
	if (shader->loadShader(pVertexFile, pFragmentFile))
	{
		mVfShaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(concat, shader));
		return shader;
	}
	return nullptr;
}

std::shared_ptr<Shader> ResourceManager::loadShader(const std::string & pVertexFile, const std::string & pFragmentFile, const std::string & pGeometryFile)
{
	const auto concat = pVertexFile + pFragmentFile + pGeometryFile;
	const auto result = mVfgShaders.find(concat);
	
	if (result != mVfgShaders.end())
	{
		return result->second;
	}
	
	auto shader = std::make_shared<Shader>();
	
	if (shader->loadShader(pVertexFile, pFragmentFile, pGeometryFile))
	{
		mVfgShaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(concat, shader));
		return shader;
	}
	return nullptr;
}

std::shared_ptr<Shader> ResourceManager::loadShader(const std::string & pVertexFile, const std::string & pFragmentFile, const std::string & pHullFile, const std::string & pDomainFile)
{
	const auto concat = pVertexFile + pFragmentFile + pHullFile + pDomainFile;
	const auto result = mVfhdShaders.find(concat);
	
	if (result != mVfhdShaders.end())
	{
		return result->second;
	}
	
	auto shader = std::make_shared<Shader>();
	
	if (shader->loadShader(pVertexFile, pFragmentFile, pHullFile, pDomainFile))
	{
		mVfhdShaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(concat, shader));
		return shader;
	}
	return nullptr;
}

std::shared_ptr<Shader> ResourceManager::loadShader(const std::string& pVertexFile, const std::string& pFragmentFile, const std::string& pGeometryFile, const std::string& pHullFile, const std::string& pDomainFile)
{
	const auto concat = pVertexFile + pFragmentFile + pGeometryFile + pHullFile + pDomainFile;
	const auto result = mVfghdShaders.find(concat);

	if (result != mVfghdShaders.end())
	{
		return result->second;
	}

	auto shader = std::make_shared<Shader>();

	if (shader->loadShader(pVertexFile, pFragmentFile, pGeometryFile, pHullFile, pDomainFile))
	{
		mVfghdShaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(concat, shader));
		return shader;
	}
	return nullptr;
}

std::shared_ptr<Shader> ResourceManager::loadShader(const std::string & pComputeFile)
{
	const auto result = mCShaders.find(pComputeFile);
	
	if (result != mCShaders.end())
	{
		return result->second;
	}
	
	auto shader = std::make_shared<Shader>();
	
	if (shader->loadShader(pComputeFile))
	{
		mCShaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(pComputeFile, shader));
		return shader;
	}
	return nullptr;
}