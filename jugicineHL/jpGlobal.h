#ifndef PLAYER__GLOBAL_H
#define PLAYER__GLOBAL_H

#include <vector>
#include <array>
#include <string>
#include <box2d/box2d.h>
#include "jmCommon.h"


namespace jugimap {


class Entity;


enum class AppState : int
{
    LOADING_INITIAL_DATA = 1,
    LOADING_SCENE = 2,
    STARTING_SCENE = 3,
    SCENE_FADE_IN = 4,
    SCENE_RUNNING = 5,
    SCENE_FADE_OUT = 6,
    REMOVING_SCENE = 7

};


enum class AppInitializationStatus : int
{

    NONE = 0,
    APP_BASE_CONFIGURATION_DATA_LOADED = 1,
    SCENE_BASE_CONFIGURATION_DATA_LOADED = 2,
    SCENE_LOGIC_CONFIGURATION_DATA_LOADED = 4,
    LOGIC_SCENE_CONFIGURATION_DATA_INITIALIZED = 8,
    SOURCE_LIBRARY_LOADED = 16,
    SOURCE_LIBRARY_ENTIRE_EXTERNAL_DATA_LOADED = 32

};


enum class AppError
{
    NONE,
    PREINIT_ERROR,
    INIT_ERROR,
    RUN_TIME_ERROR,
    DBG_ERROR
};



enum class BaseObjectType : unsigned char
{
    NOT_DEFINED,
    PLAYER_SCENE,
    LOGIC_STATE,
    ACTION,
    ACTION_TRIGGER,
    ACTION_CONDITION,
    ACTION_COMMAND,
    TRIGGER_VARIABLE,
    ITEM,
    USER_PROFILE,
    USERS_DATABASE,

    GFX_OBJECT,
    GFX_OBJECT__SPRITES,
    GFX_OBJECT__TEXT_SPRITE,

    BEHAVIOR,
    BEHAVIOR_STATE,

    COMPONENT,
    OVERLAY_COMPONENT,

    OVERLAY_STATE,

    BEH_ENGINES_CONTROLLER,

    COMPOUND

};


class BaseObject
{
public:
    virtual ~BaseObject(){}
    BaseObjectType baseType(){ return mBaseType; }

protected:
    BaseObjectType mBaseType = BaseObjectType::NOT_DEFINED;
};


//------------------------------------------------------------------------------------

/*
enum class UpdateMode : int
{
    NORMAL = 0,
    MODAL_OVERLAY = 1
};
*/

struct UpdateMode
{
    bool modalOverlay = false;
    bool loopAllStateActions = false;
};


//------------------------------------------------------------------------------------


enum class ActionStatus : int
{
    IDLE = 0,
    STARTED = 1,
    ACTIVE = 2,
    FINISHED = 4,
    DISABLED = 8
};


//------------------------------------------------------------------------------------


enum class GfxObjectType
{
    SPRITE,
    SPRITE_GROUP,
    SPRITE_LAYER,
    MAP,
    NOT_DEFINED,
    NOT_ALLOWED
};


GfxObjectType GetGfxObjectTypeFromString(const std::string &objectType);

GfxObjectType VerifyIfObjectTypeIsAllowed(GfxObjectType objectType, const std::vector<GfxObjectType> &allowedTypes);

//------------------------------------------------------------------------------------

enum class TransitionType
{
    IN,
    OUT,
    NOT_DEFINED
};



//---------------------------------------------------------------------------------------


struct jpSaveSignatures
{

    static const int USER_DATA = 50;
    static const int INPUT_PROFILES = 100;
    static const int KEYBOARD_PROFILES = 101;
    static const int JOYSTICK_PROFILES = 102;
    static const int KEYBOARD_PROFILE = 110;
    static const int JOYSTICK_PROFILE = 111;

};



//------------------------------------------------------------------------------------


struct DebugDrawFlags
{
    static const int NONE = 0;
    static const int SHAPES =       1<<0;

};


//------------------------------------------------------------------------------------

enum class SignalID : unsigned char
{
    UNKNOWN = 0,
    ENTITY_CONTACT_TRIGGER = 1,
    CONTACT_TRIGGER = 2,
    FILTERED_CONTACT_TRIGGER = 3,
    TRANSPORTER_STATUS = 4,
    MOVABLE_OBJECT = 5,
    DIRECTION = 6,
    PASSENGER = 7,
    OPERATING = 8,
    CUSTOM_ENTITY_SIGNAL = 9

};


}




#endif
