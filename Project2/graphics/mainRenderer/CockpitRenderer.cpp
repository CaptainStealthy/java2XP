#include "CockpitRenderer.h"
#include <thread>
#include <chrono>

std::shared_ptr<CockpitRenderer> CockpitRenderer::cockpitRenderer = nullptr;



CockpitRenderer::CockpitRenderer(int width, int height)
{

	cockpitBuffer = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
	cockpitBufferRenderer = SDL_CreateSoftwareRenderer(cockpitBuffer);
}

CockpitRenderer::~CockpitRenderer()
{
	
}

void CockpitRenderer::createCockpitRenderer(int width, int height)
{
	cockpitRenderer = std::make_shared<CockpitRenderer>(width, height);
	XPLMRegisterDrawCallback(drawCockpit, xplm_Phase_Gauges, 1, NULL);
	XPLMGenerateTextureNumbers(&getRendererInstance()->ourTextureID, 1);

	XPLMBindTexture2d(getRendererInstance()->ourTextureID, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4096, 4096, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	XPLMBindTexture2d(0, 0);

	cockpitRenderer->cockpitHeight = height;
	cockpitRenderer->cockpitWidth = width;

	//cockpitRenderer->testInstance = new CockpitModule(0, 0, 4096, 4096);

	/*SDL_Rect testRect{ 10,10,4000,4000 };

	SDL_SetRenderDrawColor(cockpitRenderer->cockpitBufferRenderer, 255, 0, 0, 255);
	
	SDL_RenderFillRect(cockpitRenderer->cockpitBufferRenderer, &testRect);

	SDL_SaveBMP(cockpitRenderer->cockpitBuffer, "Gay.bmp");*/

}

bool firstTime = true;
bool copyCommandIssued = false;


int CockpitRenderer::drawCockpit(XPLMDrawingPhase inPhase, int inIsBefore, void * inRefcon)
{
	std::shared_ptr<CockpitRenderer> rendererInstance = CockpitRenderer::getRendererInstance();

	if (firstTime) {
		
		glGenBuffers(1, &rendererInstance->pboID);
		glGenBuffers(1, &rendererInstance->pboIDSecond);
		rendererInstance->num_bytes = 4096 * rendererInstance->cockpitBuffer->pitch;
		rendererInstance->startRenderingThread();
		firstTime = false;
		return 1;
	}

	if (!copyCommandIssued) {
		GLuint pboToUse = 0;

		if (rendererInstance->nextPBO == 0) {
			pboToUse = rendererInstance->pboID;
		}
		else {
			pboToUse = rendererInstance->pboIDSecond;
		}


		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboToUse);

			glBufferData(GL_PIXEL_UNPACK_BUFFER, rendererInstance->num_bytes,NULL, GL_STREAM_DRAW);
			
			rendererInstance->ptrBuffer = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

			rendererInstance->startCopying = true;

			copyCommandIssued = true;

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

		

	}

	if (copyCommandIssued && rendererInstance->copyingFinished) {
		rendererInstance->copyingFinished = false;
		
		GLuint pboToUse = 0;

		if (rendererInstance->nextPBO == 0) {
			pboToUse = rendererInstance->pboID;
		}
		else {
			pboToUse = rendererInstance->pboIDSecond;
		}


		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboToUse);

		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

		rendererInstance->tex_upload_done = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	}

	

	
	if (glClientWaitSync(rendererInstance->tex_upload_done, 0, 0) != GL_TIMEOUT_EXPIRED) {
		//THE NEXT PBO HAS BEEN RENDERED TO. SETUP RENDERING TO THE NEXT ONE
		if (rendererInstance->nextPBO == 0) {
			rendererInstance->currentPBO = 0;
			rendererInstance->nextPBO = 1;
		}
		else {
			rendererInstance->currentPBO = 1;
			rendererInstance->nextPBO = 0;
		}
		copyCommandIssued = false;
	}

	GLuint pboToUse = 0;

	if (rendererInstance->currentPBO == 0) {
		pboToUse = rendererInstance->pboID;
	}
	else {
		pboToUse = rendererInstance->pboIDSecond;
	}

	XPLMBindTexture2d(rendererInstance->ourTextureID, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboToUse);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 4096, 4096, GL_RGBA, GL_UNSIGNED_BYTE, NULL);




	XPLMSetGraphicsState(
		0,        // No fog, equivalent to glDisable(GL_FOG);
		1,        // One texture, equivalent to glEnable(GL_TEXTURE_2D);
		0,        // No lighting, equivalent to glDisable(GL_LIGHT0);
		0,        // No alpha testing, e.g glDisable(GL_ALPHA_TEST);
		1,        // Use alpha blending, e.g. glEnable(GL_BLEND);
		0,        // No depth read, e.g. glDisable(GL_DEPTH_TEST);
		0);        // No depth write, e.g. glDepthMask(GL_FALSE);


	//glColor3f(1, 1, 1);        // Set color to white.
	int x1 = 0;
	int y1 = 0;
	int x2 = x1 + rendererInstance->cockpitWidth;
	int y2 = y1 + rendererInstance->cockpitHeight;
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);        glVertex2f(x1, y1);        // We draw one textured quad.  Note: the first numbers 0,1 are texture coordinates, which are ratios.
	glTexCoord2f(0, 1);        glVertex2f(x1, y2);        // lower left is 0,0, upper right is 1,1.  So if we wanted to use the lower half of the texture, we
	glTexCoord2f(1, 1);        glVertex2f(x2, y2);        // would use 0,0 to 0,0.5 to 1,0.5, to 1,0.  Note that for X-Plane front facing polygons are clockwise
	glTexCoord2f(1, 0);        glVertex2f(x2, y1);        // unless you change it; if you change it, change it back!
	glEnd();


	XPLMBindTexture2d(0, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	


	return 1;
}

int CockpitRenderer::addModule(CockpitModule * module)
{
	int idToUse = modules.size() + 1;
	modules.insert(std::make_pair(idToUse, module));
	return idToUse;
}

SDL_Renderer * CockpitRenderer::getCockpitRenderer()
{
	return cockpitBufferRenderer;
}

void CockpitRenderer::destroyCockpitRenderer()
{
	renderingThreadKill = true;

	for (auto module : modules) {
		delete module.second;
	}

	SDL_FreeSurface(cockpitBuffer);
	SDL_DestroyRenderer(cockpitBufferRenderer);

	cockpitBufferRenderer = nullptr;

}

void CockpitRenderer::startRenderingThread()
{
	this->renderingThreadKill = false;
	std::thread t1(std::bind(&CockpitRenderer::asynchronousRenderAndUpload, this));
	t1.detach();
}

TextureStore & CockpitRenderer::getTexStore()
{
	// TODO: insert return statement here
	return texStore;
}



void CockpitRenderer::asynchronousRenderAndUpload()
{
	

	while (!renderingThreadKill) {

		for (auto module : modules) {
			CockpitModule* drawingModule = module.second;
			drawingModule->draw2D();

			int textureOriginX = drawingModule->getX();
			int textureOriginY = drawingModule->getY();
			int textureWidth = drawingModule->getHeight();
			int textureHeight = drawingModule->getWidth();

			SDL_Rect modulePlacement = { textureOriginX, textureOriginY , textureWidth, textureHeight };

			int result = SDL_SetRenderTarget(cockpitBufferRenderer, NULL);

			result = SDL_RenderCopy(cockpitBufferRenderer, drawingModule->getModuleTexture(), NULL, &modulePlacement);

			//Uncomment for debugging
			//SDL_SaveBMP(cockpitRenderer->cockpitBuffer, "Test.bmp");
		}


		if (startCopying) {

			memcpy(ptrBuffer, cockpitBuffer->pixels,num_bytes);
			startCopying = false;
			copyingFinished = true;
		}

		std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(1 / fps));

	}

}
