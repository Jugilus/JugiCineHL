#include <assert.h>
#include <ncine/FileSystem.h>
#include <ncine/Texture.h>
#include "box2d/box2d.h"

#include "jmSystem.h"
#include "jmVectorShape.h"
#include "jmSprite.h"
#include "jmStandardSprite.h"
#include "jmSourceItem.h"
#include "jmCommonFunctions.h"

#include "jpPlayedApp.h"
#include "jpInput.h"

#include "jpEntityCommon.h"
#include "jpB2Body.h"
#include "jpB2Fixture.h"
#include "jpB2World.h"
#include "scripting/jpBehavior.h"
#include "jpSourceEntity.h"
#include "jpEntity.h"
#include "jpEntitySystem.h"
#include "task/jpTaskBase.h"
#include "movements/jpMovementBase.h"
#include "jpQueries.h"

#include "jpEntityGlobal.h"




namespace jugimap{




EntityRole GetEntityRoleFromString(const std::string &type)
{

    if(type=="ACTOR"){
        return EntityRole::ACTOR;

    }else if(type=="BLOCKING_OBJECT"){
        return EntityRole::BLOCKING_OBJECT;

    }else if(type=="BLOCKING_ENVIRONMENT"){
        return EntityRole::BLOCKING_ENVIRONMENT;

    }else if(type=="MOVABLE_OBJECT"){
        return EntityRole::MOVABLE_OBJECT;

    }else if(type=="TRANSPORTER"){
        return EntityRole::TRANSPORTER;

    }else if(type=="PASSABLE_OBJECT"){
        return EntityRole::PASSABLE_OBJECT;

    }else if(type=="PASSABLE_AREA"){
        return EntityRole::PASSABLE_AREA;

    }else if(type=="PATHWAY"){
        return EntityRole::PATHWAY;

    }else if(type=="GROUND_SENSOR"){
        return EntityRole::GROUND_SENSOR;

    }else if(type=="CEILING_SENSOR"){
        return EntityRole::CEILING_SENSOR;

    }else if(type=="CUSTOM_SENSOR"){
        return EntityRole::CUSTOM_SENSOR;

    }else if(type=="OBJECT_WITHOUT_SHAPE"){
        return EntityRole::OBJECT_WITHOUT_SHAPE;

    }

    return EntityRole::UNKNOWN;

}


std::string  GetEntityRoleString(EntityRole type)
{

    if(type==EntityRole::ACTOR){
        return "ACTOR";

    }else if(type==EntityRole::BLOCKING_OBJECT){
        return "BLOCKING_OBJECT";

    }else if(type==EntityRole::BLOCKING_ENVIRONMENT){
        return "BLOCKING_ENVIRONMENT";

    }else if(type==EntityRole::MOVABLE_OBJECT){
        return "MOVABLE_OBJECT";

    }else if(type==EntityRole::TRANSPORTER){
        return "TRANSPORTER";

    }else if(type==EntityRole::PASSABLE_OBJECT){
        return "PASSABLE_OBJECT";

    }else if(type==EntityRole::PASSABLE_AREA){
        return "PASSABLE_AREA";

    }else if(type==EntityRole::PATHWAY){
        return "PATHWAY";

    }else if(type==EntityRole::GROUND_SENSOR){
        return "GROUND_SENSOR";

    }else if(type==EntityRole::CEILING_SENSOR){
        return "CEILING_SENSOR";

    }else if(type==EntityRole::CUSTOM_SENSOR){
        return "CUSTOM_SENSOR";

    }else if(type==EntityRole::OBJECT_WITHOUT_SHAPE){
        return "OBJECT_WITHOUT_SHAPE";

    }

    return "UNKNOWN";

}




Direction GetDirectionFromString(const std::string &direction)
{

    if(direction == "LEFT"){
        return Direction::LEFT;

    }else if(direction == "RIGHT"){
        return Direction::RIGHT;

    }else if(direction == "UP"){
        return Direction::UP;

    }else if(direction == "DOWN"){
        return Direction::DOWN;

    }else if(direction == "LEFT_UP"){
        return Direction::LEFT_UP;

    }else if(direction == "LEFT_DOWN"){
        return Direction::LEFT_DOWN;

    }else if(direction == "RIGHT_UP"){
        return Direction::RIGHT_UP;

    }else if(direction == "RIGHT_DOWN"){
        return Direction::RIGHT_DOWN;

    }else if(direction == "FORWARD"){
        return Direction::FORWARD;

    }else if(direction == "BACKWARD"){
        return Direction::BACKWARD;
    }

    return Direction::NONE;
}



std::string GetDirectionString(Direction direction)
{

    if(direction == Direction::LEFT){
        return "LEFT";

    }else if(direction == Direction::RIGHT){
        return "RIGHT";

    }else if(direction == Direction::UP){
        return "UP";

    }else if(direction == Direction::DOWN){
        return "DOWN";

    }else if(direction == Direction::LEFT_UP){
        return "LEFT_UP";

    }else if(direction == Direction::LEFT_DOWN){
        return "LEFT_DOWN";

    }else if(direction == Direction::RIGHT_UP){
        return "RIGHT_UP";

    }else if(direction == Direction::RIGHT_DOWN){
        return "RIGHT_DOWN";

    }else if(direction == Direction::FORWARD){
        return "FORWARD";

    }else if(direction == Direction::BACKWARD){
        return "BACKWARD";
    }

    return "NONE";
}
//------------------------------------------------------------------------------------------


RotationDirection GetRotationDirectionFromString(const std::string &direction)
{

    if(direction == "CW"){
        return RotationDirection::CW;

    }else if(direction == "CCW"){
        return RotationDirection::CCW;
    }

    return RotationDirection::NONE;

}


//------------------------------------------------------------------------------------------


PathwayOrientation GetPathwayOrientationFromString(const std::string &_pathwayOrientation)
{

    if(_pathwayOrientation=="VERTICAL"){
        return PathwayOrientation::VERTICAL;

    }else if(_pathwayOrientation=="HORIZONTAL"){
        return PathwayOrientation::HORIZONTAL;
    }


    return PathwayOrientation::VERTICAL;

}



//------------------------------------------------------------------------------------------



CenteringStyle GetCenteringStyleFromString(const std::string &_centeringStyle)
{

    if(_centeringStyle=="NOVE"){
        return CenteringStyle::NONE;

    }else if(_centeringStyle=="SNAP"){
        return CenteringStyle::SNAP_TO_POSITION;

    }else if(_centeringStyle=="MOVE"){
        return CenteringStyle::MOVE_TO_POSITION;
    }


    return CenteringStyle::NONE;

}






}













