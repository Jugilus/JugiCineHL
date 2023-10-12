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



//------------------------------------------------------------------------------------------





//----------------------------------------------------------------------------


GfxObjectType GetGfxObjectTypeFromString(const std::string &objectType)
{

    if(objectType=="spriteLayer"){
        return GfxObjectType::SPRITE_LAYER;

    }else if(objectType=="sprite"){
        return GfxObjectType::SPRITE;

    }else if(objectType=="spriteGroup"){
        return GfxObjectType::SPRITE_GROUP;

    }else if(objectType=="screenMap"){
        return GfxObjectType::MAP;

    }

    return GfxObjectType::NOT_DEFINED;
}


GfxObjectType VerifyIfObjectTypeIsAllowed(GfxObjectType objectType, const std::vector<GfxObjectType> &allowedTypes)
{

    for(GfxObjectType ot : allowedTypes){
        if(objectType == ot){
            return objectType;
        }
    }

    return GfxObjectType::NOT_DEFINED;
}


//----------------------------------------------------------------------------------------------


SystemParameter GetSystemParameterFromString(const std::string &parameter)
{

    if(parameter=="fullScreen"){
        return SystemParameter::FULL_SCREEN;

    }else if(parameter=="pixelZoom"){
        return SystemParameter::PIXEL_ZOOM;

    }else if(parameter=="musicActive"){
        return SystemParameter::MUSIC_ACTIVE;

    }else if(parameter=="sfxActive"){
        return SystemParameter::SFX_ACTIVE;

    }else if(parameter=="speechActive"){
        return SystemParameter::SPEECH_ACTIVE;

    }else if(parameter=="musicVolume"){
        return SystemParameter::MUSIC_VOLUME;

    }else if(parameter=="sfxVolume"){
        return SystemParameter::SFX_VOLUME;

    }else if(parameter=="speechVolume"){
        return SystemParameter::SPEECH_VOLUME;

    }else if(parameter=="language"){
        return SystemParameter::LANGUAGE;

    }

    return SystemParameter::NOT_DEFINED;

}




}
