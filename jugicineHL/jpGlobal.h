#ifndef PLAYER__GLOBAL_H
#define PLAYER__GLOBAL_H

#include <vector>
#include <array>
#include <string>
#include <box2d/box2d.h>
#include "jmCommon.h"


namespace jugimap {


//class Entity;
class SignalStorage;
class Item;


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
    DATA,
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

    ITEM_DATA_ACTION,
    LOGIC_FUNCTION,

    COMPOUND

};


class BaseObject
{
public:
    virtual ~BaseObject(){}
    BaseObjectType baseType(){ return mBaseType; }

    virtual SignalStorage* signalStorage(){ return nullptr; }

    virtual Item* item(){ return nullptr; }

    virtual Vec2f position(){ return Vec2f(); }


protected:
    BaseObjectType mBaseType = BaseObjectType::NOT_DEFINED;
};


//------------------------------------------------------------------------------------


class OriginObjectObtainer
{
public:

    virtual ~OriginObjectObtainer(){}
    virtual BaseObject* findOriginObject() = 0;
    virtual OriginObjectObtainer *copy() = 0;


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


enum class GSpritesObjectType
{
    NOT_DEFINED,
    SPRITE,
    SPRITE_GROUP,
    SPRITE_LAYER,
    MAP,
    TEXT_SPRITE,
    ANIMATED_SPRITE

};


GSpritesObjectType GetGfxObjectTypeFromString(const std::string &objectType);

GSpritesObjectType VerifyIfObjectTypeIsAllowed(GSpritesObjectType objectType, const std::vector<GSpritesObjectType> &allowedTypes);

//------------------------------------------------------------------------------------

enum class TransitionType
{
    IN,
    OUT,
    NOT_DEFINED
};


//------------------------------------------------------------------------------------


enum class Direction
{

    NONE = 0,
    LEFT =          1 << 0,
    RIGHT =         1 << 1,
    UP =            1 << 2,
    DOWN =          1 << 3,
    LEFT_UP =       1 << 4,
    LEFT_DOWN =     1 << 5,
    RIGHT_UP =      1 << 6,
    RIGHT_DOWN =    1 << 7,

    FORWARD =       1 << 8,
    BACKWARD =      1 << 9,

    ANGLE_BASED =   1 << 10,

    INHERITED =     1 << 11

};


Direction GetDirectionFromString(const std::string &direction);
std::string GetDirectionString(Direction direction);


extern std::vector<std::pair<std::string, unsigned int>>gDirectionNamedValues;


enum class DirectionMode
{
    CARDINAL,
    PATH,
    ANGLE_BASED
};


enum class RotationDirection
{
    NONE,
    CW,
    CCW,
};


RotationDirection GetRotationDirectionFromString(const std::string &direction);





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



}




#endif
