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

		if (mPointLight && mLightingMode != 2)
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

		for (; i < mSpotLights.size() && mLightingMode == 2; i++)
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
		mDirectionalLight->releaseSimpleShadow(6);
		mDirectionalLight->releaseMappingShadow(0);
		
		if (mShadowMode == 0u)
		{
			mDirectionalLight->updateSimpleShadow();
		}
		else
		{
			mDirectionalLight->updateMappingShadow();
		}
		return true;
	}
	return false;
}

bool LightingManager::updatePointLightShadow() const
{
	if (mPointLight && mLightingMode != 2)
	{
		mPointLight->releaseSimpleShadow(7);
		mPointLight->releaseMappingShadow(1);
		
		if (mShadowMode == 0u)
		{
			mPointLight->updateSimpleShadow();
		}
		else
		{
			mPointLight->updateMappingShadow();
		}
		return true;
	}
	return false;
}

void LightingManager::updateSpotLightShadow(const unsigned int pLight) const
{
	mSpotLights[pLight]->releaseSimpleShadow(pLight + 8);
	mSpotLights[pLight]->releaseMappingShadow(pLight + 2);
	if (mShadowMode == 0u)
	{
		
		mSpotLights[pLight]->updateSimpleShadow();
	}
	else
	{
		
		mSpotLights[pLight]->updateMappingShadow();
	}
}

void LightingManager::useShadowTextures() const
{
	if (mShadowMode == 0u)
	{
		mDirectionalLight->useSimpleShadow(6);

		if (mLightingMode != 2)
		{
			mPointLight->useSimpleShadow(7);
		}
		else
		{
			for (auto i = 0u; i < mSpotLights.size(); i++)
			{
				mSpotLights[i]->useSimpleShadow(8 + i);
			}
		}
	}
	else
	{
		mDirectionalLight->useMappingShadow(0);

		if (mLightingMode != 2)
		{
			mPointLight->useMappingShadow(1);
		}
		else
		{
			for (auto i = 0u; i < mSpotLights.size(); i++)
			{
				mSpotLights[i]->useMappingShadow(2 + i);
			}
		}
	}
}
