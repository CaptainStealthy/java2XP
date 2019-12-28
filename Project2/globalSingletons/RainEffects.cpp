#include "RainEffects.h"
#include <librain.h>
#include <obj8.h>
#include <acfutils/geom.h>
#include<string>
#include <globalSingletons/GlobalStore.h>
#include <XPLMDisplay.h>
#include <XPLMUtilities.h>
using namespace std;


//GLOBAL STORE INSTANCE
static GlobalStore *store = GlobalStore::getInstance();

string librainDLLDirectory;
HMODULE librainDLL;
int errorMessage;

//OBJ DIRECTORIES
string shaderPath;
string centerLeftDir;
string centerRightDir;
string leftDir;
string rightDir;
string cockpit;
string cockpitInteriorDir;
string yoke;
string dome;
string hudDir;

//LIBRAIN OBJECTS
obj8_t *centerLeftWindow;
obj8_t *centerRightWindow;
obj8_t *leftWindow;
obj8_t *rightWindow;
obj8_t *yoke_obj;
obj8_t *dome_obj;
obj8_t *cockpit_obj;
obj8_t *cockpitInterior;
obj8_t *hudObject;

librain_glass_t leftGlass;
librain_glass_t centerLeftGlass;
librain_glass_t centerRightGlass;
librain_glass_t rightGlass;

librain_glass_t * glasses = new librain_glass_t[4];

//LIBRAIN FUNCTION POINTERS
typedef obj8_t*(WINAPI* loadObject)(const char *filename, vect3_t pos_offset);
typedef bool_t(WINAPI* initializer)(const char *the_shaderpath, const librain_glass_t *glass, size_t num);
typedef void(WINAPI *drawPrepare)(bool_t force);
typedef void(WINAPI *z_buffer)(obj8_t *obj, const char **z_depth_group_ids);
typedef void(WINAPI* actual_draw)(void);
typedef void(WINAPI *finishDraw)(void);
typedef void(WINAPI* unloadLib)(void);
typedef void(WINAPI*setDebug)(bool_t flag);
typedef void(WINAPI*setWiperTest)(bool_t flag);
typedef void(WINAPI*setWiperAngle)(const librain_glass_t *glass, unsigned wiper_nr, double angle_radians, bool_t is_moving);

loadObject load_object;
initializer init_rainEffects;
drawPrepare prepare_effects;
z_buffer prepare_buffer;
actual_draw execute_draw;
finishDraw draw_finish;
unloadLib before_unloading;
setDebug debugDraw;
setWiperTest debugWiper;
setWiperAngle angleSetWiper;
bool loadingSuccess = false;

RainEffects::RainEffects()
{
	
}


void RainEffects::initLibrain()
{

	vect3_t no_offset{ 0,0,0 };
	//INITIALIZE DIRECTORIES
	shaderPath = store->getProperty("dllDirectory") + "librain-shaders\\";
	centerLeftDir = shaderPath + "WindshieldFrontL.obj";
	centerRightDir = shaderPath + "WindshieldFrontR.obj";
	leftDir = shaderPath + "WindshieldLeft.obj";
	rightDir = shaderPath + "windshieldRight.obj";
	cockpit = shaderPath + "789cockpit.obj";
	cockpitInteriorDir = shaderPath + "789cockpitInterior.obj";
	yoke = shaderPath + "78yoke.obj";
	dome = shaderPath + "78Dome.obj";
	hudDir = shaderPath + "78hud.obj";


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
	debugWiper = (setWiperTest)GetProcAddress(librainDLL, "librain_set_wipers_visible");
	angleSetWiper = (setWiperAngle)GetProcAddress(librainDLL, "librain_set_wiper_angle");

	leftWindow = load_object(leftDir.c_str(), no_offset);
	centerLeftWindow = load_object(centerLeftDir.c_str(), no_offset);
	centerRightWindow = load_object(centerRightDir.c_str(), no_offset);
	rightWindow = load_object(rightDir.c_str(), no_offset);
	yoke_obj = load_object(yoke.c_str(), no_offset);
	dome_obj = load_object(dome.c_str(), no_offset);
	cockpit_obj = load_object(cockpit.c_str(), no_offset);
	cockpitInterior = load_object(cockpitInteriorDir.c_str(), no_offset);
	hudObject = load_object(hudDir.c_str(), no_offset);

	initGlassVariables();

	glasses[0] = leftGlass;
	glasses[1] = centerLeftGlass;
	glasses[2] = centerRightGlass;
	glasses[3] = rightGlass;

	loadingSuccess = init_rainEffects(shaderPath.c_str(), glasses, 4);


	//debugDraw(true);
	debugWiper(true);
	

}



int drawEffects (XPLMDrawingPhase     inPhase,
	int                  inIsBefore,
	void *               inRefcon)
{
	if (loadingSuccess) {
		//angleSetWiper(&glasses[1], 1, 3.14/2.0, false);
		prepare_effects(false);
		prepare_buffer(cockpit_obj, NULL);
		prepare_buffer(yoke_obj, NULL);
		prepare_buffer(dome_obj, NULL);
		prepare_buffer(cockpitInterior,NULL);
		prepare_buffer(hudObject, NULL);

		execute_draw();
		draw_finish();
	}
	return 1.0;
}
void RainEffects::cleanup()
{
	XPLMUnregisterDrawCallback(drawEffects, xplm_Phase_LastScene, 0, NULL);
	before_unloading();
	delete[] glasses;
	FreeLibrary(librainDLL);
}

void RainEffects::registerCallBack()
{
	XPLMRegisterDrawCallback(drawEffects, xplm_Phase_LastScene, 0, NULL);
}
void RainEffects::initGlassVariables()
{
	
	leftGlass.obj = leftWindow;
	leftGlass.group_ids = NULL;
	leftGlass.slant_factor = 0.96;
	leftGlass.thrust_point = vect2_t{ 0.5,-2 };
	leftGlass.thrust_factor = 0.0;
	leftGlass.max_thrust = 0.0;
	leftGlass.gravity_point = vect2_t{ 0.5,1.2 };
	leftGlass.gravity_factor = 0.3;
	leftGlass.wind_point = vect2_t{ 0.5,-2 };
	leftGlass.wind_factor = 0.96;
	leftGlass.wind_normal = 0.3;
	leftGlass.max_tas = 94;


	rightGlass.obj = rightWindow;
	rightGlass.group_ids = NULL;
	rightGlass.slant_factor = 0.96;
	rightGlass.thrust_point = vect2_t{ 0.5,-2 };
	rightGlass.thrust_factor = 0.0;
	rightGlass.max_thrust = 0.0;
	rightGlass.gravity_point = vect2_t{ 0.5,1.2 };
	rightGlass.gravity_factor = 0.3;
	rightGlass.wind_point = vect2_t{ 0.5,-2 };
	rightGlass.wind_factor = 0.96;
	rightGlass.wind_normal = 0.3;
	rightGlass.max_tas = 94;

	

	centerLeftGlass.obj = centerLeftWindow;
	centerLeftGlass.group_ids = NULL;
	centerLeftGlass.slant_factor = 0.96;
	centerLeftGlass.thrust_point = vect2_t{ 0.5,-2 };
	centerLeftGlass.thrust_factor = 0.0;
	centerLeftGlass.max_thrust = 0.0;
	centerLeftGlass.gravity_point = vect2_t{ 0.5,1.2 };
	centerLeftGlass.gravity_factor = 0.3;
	centerLeftGlass.wind_point = vect2_t{ 0.5,-2 };
	centerLeftGlass.wind_factor = 0.96;
	centerLeftGlass.wind_normal = 0.3;
	centerLeftGlass.max_tas = 94;
	centerLeftGlass.wiper_pivot[1] = vect2_t{ 0.5,0.5 };
	centerLeftGlass.wiper_radius_inner[1] = 0.0;
	centerLeftGlass.wiper_radius_outer[1] = 0.5;



	centerRightGlass.obj = centerRightWindow;
	centerRightGlass.group_ids = NULL;
	centerRightGlass.slant_factor = 0.96;
	centerRightGlass.thrust_point = vect2_t{ 0.5,-2 };
	centerRightGlass.thrust_factor = 0.0;
	centerRightGlass.max_thrust = 0.0;
	centerRightGlass.gravity_point = vect2_t{ 0.5,1.2 };
	centerRightGlass.gravity_factor = 0.3;
	centerRightGlass.wind_point = vect2_t{ 0.5,-2 };
	centerRightGlass.wind_factor = 0.96;
	centerRightGlass.wind_normal = 0.3;
	centerRightGlass.max_tas = 94;

}
RainEffects::~RainEffects()
{
}
