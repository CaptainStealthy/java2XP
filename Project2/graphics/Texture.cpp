#include "Texture.h"
#include<XPLMUtilities.h>
#include <globalSingletons/GlobalStore.h>

GlobalStore *store = GlobalStore::getInstance();

void Texture::renderToContext(SDL_Renderer *renderer, int x_offset, int y_offset, int width, int height)
{
	SDL_Rect rect{ x_offset,y_offset,width,height };
	
	SDL_RenderCopy(renderer, texture, NULL, &rect);

}

Texture::Texture(std::string fileName,SDL_Renderer *renderer)
{	

	fileName = store->getProperty("dllDirectory") + "resources/" + fileName;
	SDL_Surface* tempData = IMG_Load(fileName.c_str());

	if (!tempData) {
		std::string error = SDL_GetError();
		error += " for file at" + fileName +"\n";

		XPLMDebugString("FAILED TO LOAD TEXTURE: "); XPLMDebugString(error.c_str());
	}

	texture = SDL_CreateTextureFromSurface(renderer, tempData);

	if (texture == NULL) {
		std::string error = SDL_GetError();
		error += " for file at" + fileName + "\n";
		XPLMDebugString("FAILED TO CREATE TEXTURE: "); XPLMDebugString(error.c_str());
	}

	SDL_FreeSurface(tempData);

}


Texture::~Texture()
{
	SDL_DestroyTexture(texture);

}
