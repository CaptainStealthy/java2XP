#pragma once

#include <SDL.h>

class CockpitModule

{
public:
	CockpitModule(int x,int y,int width,int height);
	~CockpitModule();

	int getX() {
		return x;
	}

	int getY() {
		return y;
	}

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

	void draw2D();

	SDL_Texture* getModuleTexture() {
		return this->moduleTexture;
	}


private:
	SDL_Texture* moduleTexture;
	SDL_Renderer* cockpitRenderer;


	int moduleID;

	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
};

