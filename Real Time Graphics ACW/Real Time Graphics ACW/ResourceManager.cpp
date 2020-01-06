#include "ResourceManager.h"
#include "ModelLoader.h"

ResourceManager * ResourceManager::mInstance = nullptr;

void ResourceManager::loadModel(std::shared_ptr<Model> & pModel, const std::string & pModelFile)
{
	const auto result = mModels.find(pModelFile);
	
	if (result != mModels.end())
	{
		pModel = result->second;
		return;
	}
	
	ModelLoader::loadModelFromFile(pModelFile, pModel);
	
	if(pModel.get())
	{
		mModels.insert(std::pair<std::string, std::shared_ptr<Model>>(pModelFile, pModel));
	}
}

void ResourceManager::loadTexture(std::shared_ptr<Texture> & pTexture, const std::string & pTextureFile)
{
	const auto result = mTextures.find(pTextureFile);
	
	if (result != mTextures.end())
	{
		pTexture = result->second;
		return;
	}
	
	pTexture = std::make_shared<Texture>();
	
	if (pTexture->loadTexture(pTextureFile))
	{
		mTextures.insert(std::pair<std::string, std::shared_ptr<Texture>>(pTextureFile, pTexture));
		return;
	}
	pTexture.reset();
}

void ResourceManager::loadShader(std::shared_ptr<Shader> & pShader, const std::string & pVertexFile, const std::string & pFragmentFile)
{
	const auto concat = pVertexFile + pFragmentFile;
	const auto result = mVfShaders.find(concat);
	
	if (result != mVfShaders.end())
	{
		pShader = result->second;
		return;
	}
	
	pShader = std::make_shared<Shader>();
	
	if (pShader->loadShader(pVertexFile, pFragmentFile))
	{
		mVfShaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(concat, pShader));
		return;
	}
	pShader.reset();
}

void ResourceManager::loadParticleShader(std::shared_ptr<Shader> & pShader, const std::string& pVertexFile, const std::string& pFragmentFile)
{
	const auto concat = pVertexFile + pFragmentFile;
	const auto result = mParticleShader.find(concat);

	if (result != mParticleShader.end())
	{
		pShader = result->second;
		return;
	}

	pShader = std::make_shared<Shader>();

	if (pShader->loadParticleShader(pVertexFile, pFragmentFile))
	{
		mParticleShader.insert(std::pair<std::string, std::shared_ptr<Shader>>(concat, pShader));
		return;
	}
	pShader.reset();
}

void ResourceManager::loadShader(std::shared_ptr<Shader> & pShader, const std::string & pVertexFile, const std::string & pFragmentFile, const std::string & pGeometryFile)
{
	const auto concat = pVertexFile + pFragmentFile + pGeometryFile;
	const auto result = mVfgShaders.find(concat);
	
	if (result != mVfgShaders.end())
	{
		pShader = result->second;
		return;
	}
	
	pShader = std::make_shared<Shader>();
	
	if (pShader->loadShader(pVertexFile, pFragmentFile, pGeometryFile))
	{
		mVfgShaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(concat, pShader));
		return;
	}
	pShader.reset();
}

void ResourceManager::loadShader(std::shared_ptr<Shader> & pShader, const std::string & pVertexFile, const std::string & pFragmentFile, const std::string & pHullFile, const std::string & pDomainFile)
{
	const auto concat = pVertexFile + pFragmentFile + pHullFile + pDomainFile;
	const auto result = mVfhdShaders.find(concat);
	
	if (result != mVfhdShaders.end())
	{
		pShader = result->second;
		return;
	}
	
	pShader = std::make_shared<Shader>();
	
	if (pShader->loadShader(pVertexFile, pFragmentFile, pHullFile, pDomainFile))
	{
		mVfhdShaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(concat, pShader));
		return;
	}
	pShader.reset();
}

void ResourceManager::loadShader(std::shared_ptr<Shader> & pShader, const std::string& pVertexFile, const std::string& pFragmentFile, const std::string& pGeometryFile, const std::string& pHullFile, const std::string& pDomainFile)
{
	const auto concat = pVertexFile + pFragmentFile + pGeometryFile + pHullFile + pDomainFile;
	const auto result = mVfghdShaders.find(concat);

	if (result != mVfghdShaders.end())
	{
		pShader = result->second;
		return;
	}

	pShader = std::make_shared<Shader>();

	if (pShader->loadShader(pVertexFile, pFragmentFile, pGeometryFile, pHullFile, pDomainFile))
	{
		mVfghdShaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(concat, pShader));
		return;
	}
	pShader.reset();
}

void ResourceManager::loadShader(std::shared_ptr<Shader> & pShader, const std::string & pComputeFile)
{
	const auto result = mCShaders.find(pComputeFile);
	
	if (result != mCShaders.end())
	{
		pShader = result->second;
	}
	
	pShader = std::make_shared<Shader>();
	
	if (pShader->loadShader(pComputeFile))
	{
		mCShaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(pComputeFile, pShader));
		return;
	}
	pShader.reset();
}