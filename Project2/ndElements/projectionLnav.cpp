#include "projectionLnav.h"

#include<Windows.h>


//DEFINE COMMANDS BASED ON INTEGERS(LUA WILL SET THESE)
#define COMMAND_CREATE_SEGMENT "create_line" //NEEDS TO BE FOLLOWED BY SEGMENT TYPE SET IN A DREF
#define COMMAND_ADD_BEGINNING_POINT "start_point"
#define COMMAND_ADD_END_POINT "end_point"

//DEFINE COMMANDS FOR CREATING INTERIOR TURN
#define COMMAND_CREATE_INTERIOR_TURN "create_interior_turn"
#define COMMAND_SET_TURN_BEGINNING "set_turn_beginning"
#define COMMAND_SET_TURN_CURVE_POINT "set_turn_curve_point"
#define COMMAND_SET_TURN_CURVE_END "set_curve_end"

//DEFINE COMMANDS FOR CREATING ARC
#define COMMAND_CREATE_ARC "create_arc"
#define COMMAND_SET_ARC_FIX "set_arc_fix"
#define COMMAND_SET_ARC_RADIUS "set_arc_radius"
#define COMMAND_SET_ARC_INITIAL_FIX "set_arc_ifix"
#define COMMAND_SET_ARC_END_FIX "set_arc_efix"



HMODULE acftUtilsDll;



typedef fpp_t(WINAPI* init_projection)(geo_pos2_t center, double rot, const ellip_t *ellip, bool_t allow_inv);

init_projection setProjection;


projectionLnav::projectionLnav()
{
}


void projectionLnav::initialize(void)
{


}

projectionLnav::~projectionLnav()
{
}

