#pragma once

#include <utility/Logger.h>
#include <string>
#include <map>
#include "../Texture.h"

class TextureStore
{
public:
	TextureStore();
	~TextureStore();

	bool loadTextureFromFile(std::string& path);

	Texture& getTexture(int texID);

	bool removeTexture(int texID);

private:
	Logger logger = Logger("Texture Handler");
	
	std::map<int, Texture> textures;

	void logDoesNotExist(int texID);

};

