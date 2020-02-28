#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include<string>

class Texture
{
public:

	SDL_Texture *texture;

	void renderToContext(SDL_Renderer *renderer, int x_offset,int y_offset,int width, int height);

	Texture(std::string fileName,SDL_Renderer *renderer,int texID);

	bool texIniitSucceeded;

	int textureID = 0;

	void loadTexture(std::string& fileName, SDL_Renderer *renderer);

	~Texture();
};

