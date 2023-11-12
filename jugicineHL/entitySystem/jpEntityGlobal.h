#ifndef ENTITY_SYSTEM__ENTITY_GLOBAL__H
#define ENTITY_SYSTEM__ENTITY_GLOBAL__H


#include "string"
#include "vector"
#include "box2d/box2d.h"
#include "jmCommonFunctions.h"

#include "jpGlobal.h"
#include "jpQueries.h"







namespace jugimap{




struct FixtureProperties
{
    static const int NONE = 0;
    static const int PASSTHROUGH_FROM_BELLOW  =  1 << 0;
};



enum class EnumEntityMapElement : unsigned char
{
    UNKNOWN,
    SPRITE,
    SPRITE_LAYER,
    VECTOR_SHAPE
};


enum class EntityRole : unsigned char
{

    ACTOR,
    BLOCKING_OBJECT,
    BLOCKING_ENVIRONMENT,
    MOVABLE_OBJECT,
    TRANSPORTER,
    PASSABLE_OBJECT,
    PASSABLE_AREA,
    PATHWAY,

    GROUND_SENSOR,
    CEILING_SENSOR,
    CUSTOM_SENSOR,

    OBJECT_WITHOUT_SHAPE,

    UNKNOWN
};




EntityRole GetEntityRoleFromString(const std::string &type);
std::string  GetEntityRoleString(EntityRole type);



struct EntityCategory
{

    enum class ShapeRole
    {
        MAIN_SHAPE,
        EXTRA_SHAPE,
        UNKNOWN

    };

    std::string name;
    ShapeRole shapeRole = ShapeRole::MAIN_SHAPE;
    EntityRole role = EntityRole::UNKNOWN;
    unsigned int contactBits = 0;
    unsigned int contactMask = 0;


    //---
    //bool fixedRotation = true;
    bool sensor = false;
    b2BodyType mB2BodyType = b2BodyType::b2_staticBody;
    int updateOrder = 0;


};



struct CustomSensorSourceEntityCfg
{

    CustomSensorSourceEntityCfg(EntityCategory *_category, int _extraVectorShapeData, bool _temporary)
        : category(_category), extraVectorShapeData(_extraVectorShapeData), temporary(_temporary){ }


    EntityCategory *category = nullptr;
    int extraVectorShapeData = 0;           // vector shape used for fixture
    bool temporary = false;

};




struct EntityStatusFlags
{

    static const unsigned int NONE = 0;
    static const unsigned int GROUNDED_ON_START =             1 << 0;
    static const unsigned int PRESTART_PHASE =                1 << 1;
    static const unsigned int STARTED =                       1 << 2;
    static const unsigned int DISABLED_USER_CONTROLLS =       1 << 3;
    static const unsigned int FORCED_ENTITY_TRANSFORMATION =  1 << 4;
    static const unsigned int MOVING_ON_GROUND =              1 << 5;
    static const unsigned int MOVABLE_OBJECT_BELLOW =           1 << 6;
    static const unsigned int GROUP_LEADER =                    1 << 7;


    //GROUP_LEADER =                  1 << 5,
    //GROUP_MEMBER =                  1 << 6,
    //IN_GROUP = static_cast<int>(GROUP_LEADER) | static_cast<int>(GROUP_MEMBER)

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

    ANGLE_BASED =   1 << 10

};


Direction GetDirectionFromString(const std::string &direction);
std::string GetDirectionString(Direction direction);


extern std::vector<NamedValue>gDirectionNamedValues;


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


//------------------------------------------------------------------------------------


struct ContactPointInfo
{

    b2Fixture* fixture;
    int shapeIndex = 0;
    b2Vec2 normal;
    b2Vec2 position;
    b2PointState state;
    float normalImpulse;
    float tangentImpulse;
    float separation;

};


//-------------------------------------------------------------------------------------------


struct EngineUpdateParameters
{

    b2Vec2 groundNormal{0.0f, 1.0f};
    bool contactsWithDifferentGroundNormals = false;
    bool groundContact = false;
    int blockedDirections = 0;
    float timeStep = 0.0f;
    b2Vec2 actorPosM{0.0f, 1.0f};
    b2Vec2 prevEngineVelocity{0.0f, 1.0f};

    Entity *entity = nullptr;

};



struct TaskUpdateParameters
{

    float timeStep = 0.0f;

};



//------------------------------------------------------------------------------------


enum class PathwayOrientation
{
    VERTICAL,
    HORIZONTAL
};

enum class VerticalPathwayDirection
{
    DOWN,
    UP
};

enum class HorizontalPathwayDirection
{
    RIGHT,
    LEFT
};


PathwayOrientation GetPathwayOrientationFromString(const std::string &_pathwayOrientation);


enum class CenteringStyle
{
    NONE,
    SNAP_TO_POSITION,
    MOVE_TO_POSITION

};

CenteringStyle GetCenteringStyleFromString(const std::string &_centeringStyle);



//------------------------------------------------------------------------------------

enum class MovementEngineType
{

    GROUND_MOVEMENT,
    JUMP_MOVEMENT,
    PATH_POINT_TO_POINT_MOVEMENT,
    PATHWAY_MOVEMENT,
    ANIMATED_BEHAVIOR,

    NO_ENGINE_SET,
    UNKNOWN

};


//------------------------------------------------------------------------------------

enum class TaskType
{

    POINT_TO_POINT_TRANSPORTER,

    NO_DECIDER_SET,
    UNKNOWN

};




}


#endif
