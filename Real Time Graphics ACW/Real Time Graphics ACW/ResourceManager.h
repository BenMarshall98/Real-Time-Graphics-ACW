#pragma once

#include <map>
#include <string>

#include "Model.h"
#include "Texture.h"
#include "Shader.h"

class ResourceManager
{
	static ResourceManager * mInstance;
	
	ResourceManager() = default;
	
	std::map<std::string, std::shared_ptr<Model>> mModels;
	std::map<std::string, std::shared_ptr<Texture>> mTextures;
	std::map<std::string, std::shared_ptr<Shader>> mVfShaders;
	std::map<std::string, std::shared_ptr<Shader>> mVfgShaders;
	std::map<std::string, std::shared_ptr<Shader>> mVfhdShaders;
	std::map<std::string, std::shared_ptr<Shader>> mVfghdShaders;
	std::map<std::string, std::shared_ptr<Shader>> mCShaders;
	
public:
	~ResourceManager() = default;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager &&) = delete;
	ResourceManager & operator= (const ResourceManager &) = delete;
	ResourceManager & operator= (ResourceManager &&) = delete;
	
	static ResourceManager * instance()
	{
		if (!mInstance)
		{
			mInstance = new ResourceManager();
		}
		return mInstance;
	}
	
	std::shared_ptr<Model> loadModel(const std::string & pModelFile);
	std::shared_ptr<Texture> loadTexture(const std::string & pTextureFile);
	std::shared_ptr<Shader> loadShader(const std::string & pVertexFile, const std::string & pFragmentFile);
	std::shared_ptr<Shader> loadShader(const std::string & pVertexFile, const std::string & pFragmentFile, const std::string & pGeometryFile);
	std::shared_ptr<Shader> loadShader(const std::string & pVertexFile, const std::string & pFragmentFile, const std::string & pHullFile, const std::string & pDomainFile);
	std::shared_ptr<Shader> loadShader(const std::string & pVertexFile, const std::string & pFragmentFile, const std::string & pGeometryFile, const std::string & pHullFile, const std::string & pDomainFile);
	std::shared_ptr<Shader> loadShader(const std::string & pComputeFile);
};