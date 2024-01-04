#include "jpGlobal.h"


namespace jugimap {




/*

EntityBodyType GetEntityBodyTypeFromString(const std::string &type)
{

    if(type=="SOLID"){
        return EntityBodyType::SOLID;

    }else if(type=="SENSOR"){
        return EntityBodyType::SENSOR;

    }else if(type=="NONE"){
        return EntityBodyType::NONE;

    }

    return EntityBodyType::SOLID;

}

*/



/*
EntityBodyType GetEntityBodyTypeFromRole(EntityRole role, bool &sensor)
{


    if(role==EntityRole::SOLID_ACTOR){
        sensor = false;
        return EntityBodyType::DYNAMIC;

    }else if(role==EntityRole::SOLID_OBJECT){
        sensor = false;
        return EntityBodyType::STATIC;

    }else if(role==EntityRole::SOLID_TRAVERSABLE_OBJECT){
        sensor = false;
        return EntityBodyType::STATIC;

    }else if(role==EntityRole::SOLID_TRANSPORTING_OBJECT){
        sensor = true;
        return EntityBodyType::KINEMATIC;

    }else if(role==EntityRole::PASSABLE_OBJECT){
        sensor = true;
        return EntityBodyType::STATIC;

    }else if(role==EntityRole::PASSABLE_AREA){
        sensor = true;
        return EntityBodyType::STATIC;

    }else if(role==EntityRole::PATHWAY){
        sensor = false;
        return EntityBodyType::STATIC;

    }else if(role==EntityRole::GROUND_SENSOR){
        sensor = true;
        return EntityBodyType::BODY_TYPE_FROM_MAIN_SHAPE;

    }else if(role==EntityRole::CUSTOM_SENSOR){
        sensor = true;
        return EntityBodyType::BODY_TYPE_FROM_MAIN_SHAPE;

    }else if(role==EntityRole::OBJECT_WITHOUT_SHAPE){
        sensor = true;
        return EntityBodyType::WITHOUT_BODY;

    }else{
        assert(false);
    }


    return EntityBodyType::UNKNOWN;

}
*/


/*
ShapeRole GetShapeRoleFromString(const std::string &type)
{

    if(type=="mainShape"){
        return ShapeRole::MAIN_SHAPE;

    }else if(type=="extraShape"){
        return ShapeRole::EXTRA_SHAPE;

    }

    return ShapeRole::UNKNOWN;
}
*/


/*

EntityType GetEntityTypeFromString(const std::string &type)
{

    if(type=="eBlockingEnvironment"){
        return EntityType::BLOCKING_ENVIRONMENT;

    }else if(type=="eActor"){
        return EntityType::ACTOR;

    }else if(type=="eTransporter"){
        return EntityType::KINEMATIC_TRANSPORTER;

    }else if(type=="ePathway"){
        return EntityType::PATHWAY;

    }else if(type=="eEnvironmentObject"){
        return EntityType::ENVIRONMENT_OBJECT;

    }else if(type=="eArea"){
        return EntityType::AREA;

    }else if(type=="eBlockableObject"){
        return EntityType::BLOCKABLE_OBJECT;

    }else if(type=="eAnimationObject"){
        return EntityType::ANIMATION_OBJECT;

    }

    return EntityType::UNKNOWN;

}


std::string  GetEntityTypeString(EntityType type)
{

    if(type==EntityType::BLOCKING_ENVIRONMENT){
        return "eBlockingEnvironment";

    }else if(type==EntityType::ACTOR){
        return "eActor";

    }else if(type==EntityType::KINEMATIC_TRANSPORTER){
        return "eTransporter";

    }else if(type==EntityType::PATHWAY){
        return "ePathway";

    }else if(type==EntityType::ENVIRONMENT_OBJECT){
        return "eEnvironmentObject";

    }else if(type==EntityType::AREA){
        return "eArea";

    }else if(type==EntityType::BLOCKABLE_OBJECT){
        return "eBlockableObject";

    }else if(type==EntityType::ANIMATION_OBJECT){
        return "eAnimationObject";

    }

    return "unknown";

}

*/


//------------------------------------------------------------------------------------------



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

std::vector<std::pair<std::string, unsigned int>>gDirectionNamedValues
{
    { "LEFT", static_cast<int>(Direction::LEFT) },
    { "RIGHT", static_cast<int>(Direction::RIGHT) },
    { "UP", static_cast<int>(Direction::UP) },
    { "DOWN", static_cast<int>(Direction::DOWN) },
    { "LEFT_UP", static_cast<int>(Direction::LEFT_UP) },
    { "LEFT_DOWN", static_cast<int>(Direction::LEFT_DOWN) },
    { "RIGHT_UP", static_cast<int>(Direction::RIGHT_UP) },
    { "RIGHT_DOWN", static_cast<int>(Direction::RIGHT_DOWN) },
    { "FORWARD", static_cast<int>(Direction::FORWARD) },
    { "BACKWARD", static_cast<int>(Direction::BACKWARD) }

};


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




//----------------------------------------------------------------------------


GSpritesObjectType GetGfxObjectTypeFromString(const std::string &objectType)
{

    if(objectType=="spriteLayer"){
        return GSpritesObjectType::SPRITE_LAYER;

    }else if(objectType=="sprite"){
        return GSpritesObjectType::SPRITE;

    }else if(objectType=="spriteGroup"){
        return GSpritesObjectType::SPRITE_GROUP;

    }else if(objectType=="screenMap"){
        return GSpritesObjectType::MAP;

    }

    return GSpritesObjectType::NOT_DEFINED;
}


GSpritesObjectType VerifyIfObjectTypeIsAllowed(GSpritesObjectType objectType, const std::vector<GSpritesObjectType> &allowedTypes)
{

    for(GSpritesObjectType ot : allowedTypes){
        if(objectType == ot){
            return objectType;
        }
    }

    return GSpritesObjectType::NOT_DEFINED;
}







}
