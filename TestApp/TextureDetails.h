#pragma once
#include <string>
#include "Texture.h"

struct TextureDetail 
{
	TextureDetail() {}

	TextureDetail(Texture* texture, std::string type, std::string path) :
		mTexture(texture),
		mType(type),
		mPath(path) 
	{
	}

	Texture* mTexture;
	std::string mType;
	std::string mPath;
};