#include "TextureStore.h"
#include "../mainRenderer/CockpitRenderer.h"


TextureStore::TextureStore()
{
}


TextureStore::~TextureStore()
{
}

bool TextureStore::loadTextureFromFile(std::string & path)
{

	try {
		int textureID = textures.size() + 1;
		Texture tex = Texture(path, CockpitRenderer::getRendererInstance()->getCockpitRenderer(),textureID);
		textures.insert(std::make_pair(textureID, tex));
		return true;
	}catch(std::exception ex){
		logger.setLogLevel(Logger::log_error);
		logger.logString(ex.what());
		return false;
	}

}

Texture& TextureStore::getTexture(int texID)
{
	try {
		return textures.at(texID);
	}
	catch (std::out_of_range ex) {
		logDoesNotExist(texID);
	}
}

bool TextureStore::removeTexture(int texID)
{
	try {
		textures.erase(texID);
		return true;
	}
	catch (std::out_of_range ex) {
		logDoesNotExist(texID);
		return false;
	}
}

void TextureStore::logDoesNotExist(int texID)
{
	string error = "Texture ID: " + to_string(texID) + " does not exist!";
	logger.setLogLevel(Logger::log_error);
	logger.logString(error);
}

