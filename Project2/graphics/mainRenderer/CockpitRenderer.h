#pragma once

#include <SDL.h>
#include<memory>
#include <map>
#include "../module/CockpitModule.h"
#include <XPLMDisplay.h>
#include <XPLMGraphics.h>
#include <GL/glew.h>
#include "../textures/TextureStore.h"

class CockpitRenderer
{
public:
	CockpitRenderer(int width, int height);
	
	~CockpitRenderer();

	static std::shared_ptr<CockpitRenderer>& getRendererInstance() {
		return cockpitRenderer;
	}

	static void createCockpitRenderer(int width, int height);

	static int drawCockpit(XPLMDrawingPhase inPhase, int inIsBefore, void *inRefcon);

	int addModule(CockpitModule* module);

	SDL_Renderer* getCockpitRenderer();

	void destroyCockpitRenderer();

	void startRenderingThread();

	TextureStore& getTexStore();



private:

	//WORKER THREAD METHOD TO ASYNCHRONOUSLY RENDER IN THE BACKGROUND 
	void asynchronousRenderAndUpload();
	
	//FOR DEBUGGING 
	CockpitModule* testInstance;

	//TEXTURE SETUP STUFF FOR THE COCKPIT
	int ourTextureID;
	SDL_Surface* cockpitBuffer;
	SDL_Renderer* cockpitBufferRenderer;

	//SHARED POINTER TO OUR INSTANCE
	static std::shared_ptr<CockpitRenderer> cockpitRenderer;

	std::map<int, CockpitModule*> modules;


	//WIDTH AND HEIGHT OF PANEL.PNG
	int cockpitHeight = 0;
	int cockpitWidth = 0;

	//FPS WE RENDER AT 
	int fps = 50;

	//PBO SETUP STUFF
	GLuint pboID;
	GLuint pboIDSecond;
	int nextPBO = 0;
	int currentPBO = 0;

	//TO KILL RENDERING THREAD SET IT TO TRUE
	bool renderingThreadKill = true;

	//POINTERS FOR OUR MAPPED BUFFER
	void* ptrBuffer;
	

	//TO FLAG IF TEXTURE UPLOAD IS COMPLETE
	GLsync tex_upload_done;

	TextureStore texStore;
	

	//TO SIGNAL RENDERING THREAD TO COPY DATA INTO POINTER SPECIFIED
	bool startCopying = false;
	bool copyingFinished = false;

	unsigned int num_bytes = 4096 * 4096 * 32;
};

