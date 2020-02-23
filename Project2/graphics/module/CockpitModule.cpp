#include "CockpitModule.h"
#include "../mainRenderer/CockpitRenderer.h"
#include "SDL2_framerate.h"



CockpitModule::CockpitModule(std::string moduleName, int moduleID, int x, int y, int width, int height) : Module(moduleName, moduleID)
{
	this->width = width;
	this->height = height;
	cockpitRenderer = CockpitRenderer::getRendererInstance()->getCockpitRenderer();
	moduleTexture = SDL_CreateTexture(cockpitRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
	this->moduleID = CockpitRenderer::getRendererInstance()->addModule(this);
}

CockpitModule::~CockpitModule()
{
	SDL_DestroyTexture(moduleTexture);
}

void CockpitModule::draw2D()
{

	SDL_SetRenderTarget(cockpitRenderer, moduleTexture);

	SDL_SetRenderDrawColor(cockpitRenderer, 255, 0, 0, 255);

	SDL_Rect testRect = { 0,0,4000,4000 };

	SDL_RenderFillRect(cockpitRenderer, &testRect);

}
