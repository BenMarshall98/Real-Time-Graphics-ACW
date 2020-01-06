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
	std::map<std::string, std::shared_ptr<Shader>> mParticleShader;
	
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
	
	void loadModel(std::shared_ptr<Model> & pModel, const std::string & pModelFile);
	void loadTexture(std::shared_ptr<Texture> & pTexture, const std::string & pTextureFile);
	void loadShader(std::shared_ptr<Shader> & pShader, const std::string & pVertexFile, const std::string & pFragmentFile);
	void loadShader(std::shared_ptr<Shader> & pShader, const std::string & pVertexFile, const std::string & pFragmentFile, const std::string & pGeometryFile);
	void loadShader(std::shared_ptr<Shader> & pShader, const std::string & pVertexFile, const std::string & pFragmentFile, const std::string & pHullFile, const std::string & pDomainFile);
	void loadShader(std::shared_ptr<Shader> & pShader, const std::string & pVertexFile, const std::string & pFragmentFile, const std::string & pGeometryFile, const std::string & pHullFile, const std::string & pDomainFile);
	void loadShader(std::shared_ptr<Shader> & pShader, const std::string & pComputeFile);
	void loadParticleShader(std::shared_ptr<Shader> & pShader, const std::string & pVertexFile, const std::string & pFragmentFile);
};