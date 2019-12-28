#include "Texture.h"
#include<XPLMUtilities.h>
#include <globalSingletons/GlobalStore.h>


void Texture::renderToContext(SDL_Renderer *renderer, int x_offset, int y_offset, int width, int height)
{

	if (!texture) {
		throw std::exception("Not able to draw null texture");
	}

	SDL_Rect rect{ x_offset,y_offset,width,height };
	
	SDL_RenderCopy(renderer, texture, NULL, &rect);

}

Texture::Texture(std::string fileName,SDL_Renderer *renderer,int texID)
{	

	loadTexture(fileName, renderer);
	this->textureID = texID;

}


void Texture::loadTexture(std::string & fileName, SDL_Renderer *renderer)
{

	SDL_Surface* tempData = IMG_Load(fileName.c_str());

	if (!tempData) {
		std::string error = SDL_GetError();
		error = "Failed to load texture: " + error;
		error += " for file at" + fileName + "\n";
		throw std::exception(error.c_str());
	}

	texture = SDL_CreateTextureFromSurface(renderer, tempData);

	if (texture == NULL) {
		std::string error = SDL_GetError();
		error = "Failed to create texture: " + error;
		error += " for file at" + fileName + "\n";
		throw std::exception(error.c_str());
	}

	SDL_FreeSurface(tempData);

}

Texture::~Texture()
{
	SDL_DestroyTexture(texture);

}
