#include "RainEffects.h"
#include <librain.h>
#include <obj8.h>
#include <acfutils/geom.h>
#include<string>
#include <globalSingletons/GlobalStore.h>
#include <XPLMDisplay.h>
#include <XPLMUtilities.h>
using namespace std;

RainEffects* RainEffects::rainEffects = nullptr;

//GLOBAL STORE INSTANCE
static GlobalStore *store = GlobalStore::getInstance();

string librainDLLDirectory;
HMODULE librainDLL;
int errorMessage;

//OBJ DIRECTORIES
string shaderPath;
string glassDir;
string cockpit;
string yoke;
string dome;

//LIBRAIN OBJECTS
obj8_t *glassObj;
obj8_t *yoke_obj;
obj8_t *dome_obj;
obj8_t *cockpit_obj;

librain_glass_t mainGlass;

//LIBRAIN FUNCTION POINTERS
typedef obj8_t*(WINAPI* loadObject)(const char *filename, vect3_t pos_offset);
typedef bool_t(WINAPI* initializer)(const char *the_shaderpath, const librain_glass_t *glass, size_t num);
typedef void(WINAPI *drawPrepare)(bool_t force);
typedef void(WINAPI *z_buffer)(obj8_t *obj, const char **z_depth_group_ids);
typedef void(WINAPI* actual_draw)(void);
typedef void(WINAPI *finishDraw)(void);
typedef void(WINAPI* unloadLib)(void);
typedef void(WINAPI*setDebug)(bool_t flag);

loadObject load_object;
initializer init_rainEffects;
drawPrepare prepare_effects;
z_buffer prepare_buffer;
actual_draw execute_draw;
finishDraw draw_finish;
unloadLib before_unloading;
setDebug debugDraw;
bool loadingSuccess = false;

RainEffects::RainEffects()
{
	rainEffects = nullptr;
}


RainEffects * RainEffects::getInstance()
{
	if (!rainEffects) {
		rainEffects = new RainEffects();
	}
	return rainEffects;
}

void RainEffects::initLibrain()
{

	vect3_t no_offset{ 0,0,0 };
	//INITIALIZE DIRECTORIES
	shaderPath = store->getProperty("dllDirectory") + "librain-shaders\\";
	glassDir = shaderPath + "Windshield_out.obj";
	cockpit = shaderPath + "B7879_cockpit.obj";
	yoke = shaderPath + "yoke.obj";
	dome = shaderPath + "Dome.obj";

	librainDLLDirectory = store->getProperty("dllDirectory") + "rain.dll";
	LPCSTR directo = librainDLLDirectory.c_str();
	librainDLL = LoadLibrary(directo);
	if (!librainDLL) {
		XPLMDebugString("[787 UDP] FAILED TO LOAD LIBRARY\nError Code: ");
		errorMessage = (unsigned int)GetLastError();
		XPLMDebugString(to_string(errorMessage).c_str()); XPLMDebugString("\n");
		return;
	}
	else {
		XPLMDebugString("[787 UDP] Succesfully loaded library\n");
		loadingSuccess = true;
	}
	load_object = (loadObject)GetProcAddress(librainDLL, "obj8_parse");
	init_rainEffects = (initializer)GetProcAddress(librainDLL, "librain_init");
	prepare_effects = (drawPrepare)GetProcAddress(librainDLL, "librain_draw_prepare");
	prepare_buffer = (z_buffer)GetProcAddress(librainDLL, "librain_draw_z_depth");
	execute_draw = (actual_draw)GetProcAddress(librainDLL, "librain_draw_exec");
	draw_finish = (finishDraw)GetProcAddress(librainDLL, "librain_draw_finish");
	before_unloading = (unloadLib)GetProcAddress(librainDLL, "librain_fini");
	debugDraw = (setDebug)GetProcAddress(librainDLL, "librain_set_debug_draw");

	glassObj = load_object(glassDir.c_str(), no_offset);
	yoke_obj = load_object(yoke.c_str(), no_offset);
	dome_obj = load_object(dome.c_str(), no_offset);
	cockpit_obj = load_object(cockpit.c_str(), no_offset);

	mainGlass.obj = glassObj;
	mainGlass.group_ids = NULL;
	mainGlass.slant_factor = 0.96;
	mainGlass.thrust_point = vect2_t{ 0.5,-2 };
	mainGlass.thrust_factor = 0.0;
	mainGlass.max_thrust = 0.0;
	mainGlass.gravity_point = vect2_t{ 0.5,1.2 };
	mainGlass.gravity_factor = 0.3;
	mainGlass.wind_point = vect2_t{ 0.5,-2 };
	mainGlass.wind_factor = 0.96;
	mainGlass.wind_normal = 0.3;
	mainGlass.max_tas = 94;

	loadingSuccess = init_rainEffects(shaderPath.c_str(), &mainGlass, 1);
	

}



int drawEffects (XPLMDrawingPhase     inPhase,
	int                  inIsBefore,
	void *               inRefcon)
{
	if (loadingSuccess) {
		prepare_effects(false);
		prepare_buffer(cockpit_obj, NULL);
		prepare_buffer(yoke_obj, NULL);
		prepare_buffer(dome_obj, NULL);

		execute_draw();
		draw_finish();
	}
	return 1.0;
}
void RainEffects::cleanup()
{
	XPLMUnregisterDrawCallback(drawEffects, xplm_Phase_LastScene, 0, NULL);
	before_unloading();
	FreeLibrary(librainDLL);
}

void RainEffects::registerCallBack()
{
	XPLMRegisterDrawCallback(drawEffects, xplm_Phase_LastScene, 0, NULL);
}
RainEffects::~RainEffects()
{
}
