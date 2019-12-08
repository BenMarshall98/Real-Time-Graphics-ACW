#pragma once
#include "Technique.h"

class TextureMapping final : public Technique
{
public:
	TextureMapping();
	~TextureMapping();

	void render(std::shared_ptr<Object> & pObject, bool pDeferred) override;
};

