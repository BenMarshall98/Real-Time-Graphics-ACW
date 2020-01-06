#include "LightingManager.h"
#include "DX11Render.h"

LightingManager * LightingManager::mInstance = nullptr;

void LightingManager::addSpotLight(const std::shared_ptr<SpotLight> & pSpotLight)
{
	if (mSpotLights.size() == 4)
	{
		mSpotLights.erase(mSpotLights.begin());
	}

	mSpotLights.push_back(pSpotLight);
}

void LightingManager::removeDirectionalLight(const std::shared_ptr<DirectionalLight>& pDirectionalLight)
{
	if (mDirectionalLight == pDirectionalLight)
	{
		mDirectionalLight.reset();
	}
}

void LightingManager::removePointLight(const std::shared_ptr<PointLight>& pPointLight)
{
	if (mPointLight == pPointLight)
	{
		mPointLight.reset();
	}
}

void LightingManager::removeSpotLight(const std::shared_ptr<SpotLight>& pSpotLight)
{
	const auto it = std::find(mSpotLights.begin(), mSpotLights.end(), pSpotLight);

	if (it != mSpotLights.end())
	{
		mSpotLights.erase(it);
	}
}

void LightingManager::update() const
{
	const auto render = Dx11Render::instance();
	
	{
		DirectionalLightBuffer db;
		ZeroMemory(&db, sizeof DirectionalLightBuffer);

		if (mDirectionalLight)
		{
			mDirectionalLight->use(db);
		}
		else
		{
			db.mIsUsed = false;
		}

		render->useDirectionalLightBuffer(db);
	}

	{
		PointLightBuffer pb;
		ZeroMemory(&pb, sizeof PointLightBuffer);

		if (mPointLight)
		{
			mPointLight->use(pb);
		}
		else
		{
			pb.mIsUsed = false;
		}

		render->usePointLightBuffer(pb);
	}

	{
		SpotLightBuffer sb;
		ZeroMemory(&sb, sizeof SpotLightBuffer);

		auto i = 0u;

		for (; i < mSpotLights.size(); i++)
		{
			mSpotLights[i]->use(sb, i);
		}

		for (; i < 4; i++)
		{
			sb.mIsUsed[i] = false;
		}

		render->useSpotLightBuffer(sb);
	}
}

bool LightingManager::updateDirectionalLightShadow() const
{
	if (mDirectionalLight)
	{
		mDirectionalLight->releaseShadow(0);
		mDirectionalLight->updateShadow();
		return true;
	}
	return false;
}

bool LightingManager::updatePointLightShadow() const
{
	if (mPointLight)
	{
		mPointLight->releaseShadow(1);
		mPointLight->updateShadow();
		return true;
	}
	return false;
}

void LightingManager::updateSpotLightShadow(const unsigned int pLight) const
{
	mSpotLights[pLight]->releaseShadow(pLight + 2);
	mSpotLights[pLight]->updateShadow();
}

void LightingManager::useShadowTextures() const
{
	mDirectionalLight->useShadow(0);
	mPointLight->useShadow(1);

	for (auto i = 0u; i < mSpotLights.size(); i++)
	{
		mSpotLights[i]->useShadow(2 + i);
	}
}
