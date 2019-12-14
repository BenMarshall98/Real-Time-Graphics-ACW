#pragma once
#include "Technique.h"

class TextureMapping final : public Technique
{
public:
	TextureMapping();
	~TextureMapping();

	void render(std::shared_ptr<Object> & pObject, bool pDeferred) override;
	void renderDirectionalShadow(std::shared_ptr<Object>& pObject) override;
	void renderOmniDirectionalShadow(std::shared_ptr<Object>& pObject) override;
};

