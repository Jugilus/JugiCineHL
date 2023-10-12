#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmMap.h"
#include "jmCamera.h"
#include "jmSceneLayout.h"
#include "jmCommonFunctions.h"
#include "jmStandardSprite.h"
#include "jmCompositeSprite.h"
#include "jmSpriteLayer.h"
#include "jmVectorLayer.h"
#include "jmVectorShape.h"
#include "jmUtilities.h"

#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"

#include "movements/jpMovementBase.h"
#include "scripting/jpBehavior.h"
#include "scripting/jpBehCfg.h"
#include "jpEntitySystem.h"
#include "jpB2Body.h"
#include "jpB2Fixture.h"
#include "jpEntityCommon.h"
#include "jpB2World.h"
#include "jpEntity.h"
#include "jpSourceEntity.h"



namespace jugimap{



using namespace jugimap;


/*
SourceEntityCfg::~SourceEntityCfg()
{
    if(ownedEnginesControllerCfg && enginesControllerCfg){
        delete  enginesControllerCfg;
    }

}
*/



bool SourceEntityCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading entity '" + name +"' ...");


    for(pugi::xml_node nChild = _node.first_child(); nChild; nChild = nChild.next_sibling()){
        std::string childNodeName = std::string(nChild.name());

        if(childNodeName=="parameters"){

            for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="passableFromBellow"){
                    passableFromBellow = a.as_bool(false);

                }else if(attributeName=="centerMarginP"){

                    std::vector<std::string>vCenteringMargin = StdString::splitString(std::string(a.as_string("")), ",");
                    if(vCenteringMargin.size()==1){
                        centeringMargin.left = gWorldInfo.pixelsToMeters(StdString::stringToInt(vCenteringMargin[0], 0));
                        centeringMargin.right = centeringMargin.left;

                    }else if(vCenteringMargin.size()==2){
                        centeringMargin.left = gWorldInfo.pixelsToMeters(StdString::stringToInt(vCenteringMargin[0], 0));
                        centeringMargin.right = gWorldInfo.pixelsToMeters(StdString::stringToInt(vCenteringMargin[1], 0));
                    }

                    centeringMargin.xMarginActive = true;

                }else if(attributeName=="centerMarginM"){

                    std::vector<std::string>vCenteringMargin = StdString::splitString(std::string(a.as_string("")), ",");
                    if(vCenteringMargin.size()==1){
                        centeringMargin.left = StdString::stringToFloat(vCenteringMargin[0], 0.0f);
                        centeringMargin.right = centeringMargin.left;

                    }else if(vCenteringMargin.size()==2){
                        centeringMargin.left = StdString::stringToFloat(vCenteringMargin[0], 0.0f);
                        centeringMargin.right = StdString::stringToFloat(vCenteringMargin[1], 0.0f);
                    }

                    centeringMargin.xMarginActive = true;

                }else if(attributeName=="fixedRotation"){
                    fixedRotation = a.as_bool(true);

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                    return false;

                }
            }

        }else if(childNodeName=="pathwayParameters"){

            if(category->role != EntityRole::PATHWAY){
                dbgSystem.addMessage("Node  '" + childNodeName + "' defined but the entity role is not 'pathway' !");
                return false;
            }

            mPathwaySourceEntityCfg.reset(new PathwaySourceEntityCfg());

            if(mPathwaySourceEntityCfg->initCfg(nChild)==false){
                return false;
            }



        }else if(childNodeName=="behaviorController"){

            for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="use"){
                    std::string sEnginesControllerCfg = a.as_string("");
                    enginesControllerCfg = entitySystem->enginesControllersCfsGroup()->getEnginesControllerCfg(sEnginesControllerCfg);
                    if(enginesControllerCfg==nullptr){
                        return false;
                    }

                }else if(attributeName=="startState"){
                    startState = a.as_string("");
                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                    return false;

                }
            }

            if(enginesControllerCfg==nullptr){
                enginesControllerCfg = entitySystem->enginesControllersCfsGroup()->addEnginesControllerCfg(name + " controller");
                if(enginesControllerCfg->initCfg(nChild)==false){
                    return false;
                }

                if(enginesControllerCfg->statesCfgs.empty()){
                    dbgSystem.addMessage("Entity '" + name + "' has no defined behavior controller states!");
                    return false;
                }
                //startState = enginesControllerCfg->statesCfgs.front().name;
            }

        }else if(childNodeName=="taskController"){

            for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="use"){
                    std::string taskController = a.as_string("");
                    if(taskController.empty()==false){
                        taskControllerCfg = entitySystem->taskControllersCfgsGroup()->getEnginesControllerCfg(taskController);
                        if(taskControllerCfg==nullptr){
                            return false;
                        }
                    }

                }else if(attributeName=="startState"){
                    taskHandlerStartState = a.as_string("");

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                    return false;

                }
            }

            if(taskControllerCfg==nullptr){
                taskControllerCfg = entitySystem->taskControllersCfgsGroup()->addEnginesControllerCfg(name + " controller");
                if(taskControllerCfg->initCfg(nChild)==false){
                    return false;
                }

                if(taskControllerCfg->statesCfgs.empty()){
                    dbgSystem.addMessage("Entity '" + name + "' has no defined task controller states!");
                    return false;
                }
            }


        }else{
            dbgSystem.addMessage("Unknown node '" + childNodeName + "' !");
            return false;
        }
    }



    //--- custom sensor cfgs
    if(enginesControllerCfg){

        EntityCategoriesGroup & ecg = entitySystem->entityCategoriesGroup();
        MovementEnginesManager *mem = entitySystem->movementEnginesManager();


        bool hasGroundMovement = false;
        bool addCeilingSensor = false;
        for(BehaviorStateCfg& stateCfg : enginesControllerCfg->statesCfgs){
            if(stateCfg.movementEngine.empty()==false){
                MovementEngineCfg* cfg = mem->getMovementEngineCfg(stateCfg.movementEngine);
                assert(cfg);
                if(cfg->factory->type()==MovementEngineType::GROUND_MOVEMENT){
                    hasGroundMovement = true;
                }
                if(cfg->factory->type()==MovementEngineType::JUMP_MOVEMENT){
                    JumpMovementCfgBase * jumpCfg = static_cast<JumpMovementCfgBase*>(cfg);
                    if(jumpCfg->addCeilingSensor){
                        addCeilingSensor = true;
                    }
                }

                std::vector<CustomSensorCfg>customSensorCfgs;
                cfg->collectCustomSensors(customSensorCfgs);
                for(CustomSensorCfg & csCfg : customSensorCfgs){

                    EntityCategory *ec = ecg.getEntityCategory(csCfg.customSensorCategory);
                    if(ec==nullptr){
                        return false;
                    }
                    bool temporary = csCfg.extraShapeActiveOnAnimationKeyData != -1 ;
                    mCustomSensorSourceEntityCfgs.push_back(CustomSensorSourceEntityCfg(ec, csCfg.customSensorExtraShapeID, temporary));
                }
            }
        }


        if(hasGroundMovement){
            // add ground sensor
            EntityCategory *ec = ecg.getEntityCategory(EntityRole::GROUND_SENSOR);
            if(ec==nullptr){
                return false;
            }
            mCustomSensorSourceEntityCfgs.push_back(CustomSensorSourceEntityCfg(ec, 0, false));
        }

        if(addCeilingSensor){
            // add ceiling sensor
            EntityCategory *ec = ecg.getEntityCategory(EntityRole::CEILING_SENSOR);
            if(ec==nullptr){
                return false;
            }
            mCustomSensorSourceEntityCfgs.push_back(CustomSensorSourceEntityCfg(ec, 0, false));
        }

        // .
        // .
        // .


    }


    dbgSystem.removeLastMessage();
    return true;

}



//=============================================================================================


bool PathwaySourceEntityCfg::initCfg(const pugi::xml_node &_node)
{


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="orientation"){
            orientation = GetPathwayOrientationFromString(a.as_string("VERTICAL"));

        }else if(attributeName=="centeringStyle"){
            centeringStyle = GetCenteringStyleFromString(a.as_string("NONE"));

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) + "' !");
            return false;
        }
    }


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="actorContactingPointOffset"){

            actorsPathwayContactPointOffsets.push_back(ActorPathwayContactPointOffset());
            ActorPathwayContactPointOffset &o = actorsPathwayContactPointOffsets.back();

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="entity"){
                    o.actorName = a.as_string("");

                }else if(attributeName=="xOffsetP"){
                    o.offsetM.x = gWorldInfo.pixelsToMeters(a.as_float(0.0f));

                }else if(attributeName=="xOffsetM"){
                    o.offsetM.x = a.as_float(0.0f);

                }else if(attributeName=="yOffsetP"){
                    o.offsetM.y = gWorldInfo.pixelsToMeters(a.as_float(0.0f));

                }else if(attributeName=="yOffsetM"){
                    o.offsetM.y = a.as_float(0.0f);

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(n.name()) + "' !");
                    return false;
                }
            }

        }else{

            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;
        }
    }


    return true;

}


b2Vec2 PathwaySourceEntityCfg::getContactPointOffsetForActor(const std::string &_actorName)
{

    for(ActorPathwayContactPointOffset &o : actorsPathwayContactPointOffsets){
        if(o.actorName == _actorName){
            return o.offsetM;
        }
    }

    return b2Vec2{0.0f, 0.0f};
}



//=============================================================================================


SourceEntity::SourceEntity(SourceEntityCfg *_sourceEntityCfg) :
    mSourceEntityCfg(_sourceEntityCfg)
{
}


EntityRole SourceEntity::entityBodyKind()
{
    return mSourceEntityCfg->category->role;
}


bool SourceEntity::createSourceBody(PlayedScene *_scene, SourceSprite *_sourceSprite, SpriteLayer *_spriteLayer)
{

    dbgSystem.addMessage("Initializing source entity '" + mSourceEntityCfg->name+"' ...");

    //if(mMovementConfigurations->initConnections(_scene)==false){
    //    return false;
    //}

    //for(SourceEntityCfg *se : mSourceEntitiesCfgs){
    //    if(se->initConnections(_scene)==false){
    //        return false;
    //    }
    //}
    mSourceSprite = _sourceSprite;


    //EntityRole bodyKind = entityBodyKind();


    if(sourceEntityCfg()->category->role==EntityRole::OBJECT_WITHOUT_SHAPE){
        dbgSystem.removeLastMessage();
        return true;
    }



    mSourceBody.reset(new SourceBody());


    b2Vec2 bodyAnchor(0.0f, 0.0f);
    if(_sourceSprite){
        if(_sourceSprite->kind()==SpriteType::STANDARD){
            SourceImage *si = mSourceSprite->sourceImages().front();
            Vec2f handle = Vec2iToVec2f(si->handle());
            Vec2f anchorPoint(handle.x - si->width() * 0.5f, handle.y - si->height()*0.5f);

            bodyAnchor.x = gWorldInfo.pixelsToMeters(anchorPoint.x);
            bodyAnchor.y = gWorldInfo.pixelsToMeters(anchorPoint.y);

        }else if(_sourceSprite->kind()==SpriteType::COMPOSITE){
            Vec2f handle = _sourceSprite->sourceCompositeSprite()->handleForMapCompositeSprites();
            Vec2f anchorPoint = handle;
            bodyAnchor.x = gWorldInfo.pixelsToMeters(anchorPoint.x);
            bodyAnchor.y = gWorldInfo.pixelsToMeters(anchorPoint.y);
        }
    }

    if(mSourceBody->initBody(mSourceEntityCfg, bodyAnchor, _sourceSprite, _spriteLayer)==false){
        return false;
    }





    /*
    if(bodyKind == EntityBodyKind::DYNAMIC_SOLID_BODY){

        if(mSourceBody->addBodyFixture(FixtureRole::ACTOR_BODY, mSourceSprite)==false){
            return false;
        }
        if(mSourceBody->addSensorsFixtures({FixtureRole::ACTOR_GROUND_SENSOR})==false){
            return false;
        }

    }else if(mSourceEntityCfg->type == EntityType::KINEMATIC_TRANSPORTER){

        if(mSourceBody->addBodyFixture(FixtureRole::BLOCKING_ENVIRONMENT, mSourceSprite)==false){
            return false;
        }

    }else if(mSourceEntityCfg->type == EntityType::BLOCKING_ENVIRONMENT){

        if(mSourceBody->addSpriteLayerBodyFixtures(FixtureRole::BLOCKING_ENVIRONMENT, _spriteLayer)==false){
            return false;
        }

    }else if(mSourceEntityCfg->type == EntityType::PATHWAY){

        if(mSourceBody->addSensorsFixtures({FixtureRole::PATHWAY_SENSOR})==false){
            return false;
        }

    }else if(mSourceEntityCfg->type == EntityType::AREA){

        if(mSourceBody->addSensorsFixtures({FixtureRole::AREA_SENSOR})==false){
            return false;
        }

    }else if(mSourceEntityCfg->type == EntityType::ENVIRONMENT_OBJECT){

        FixtureRole fixtureRole = FixtureRole::ENVIRONMENT_OBJECT_SENSOR;
        if(mSourceEntityCfg->bodyType==EntityBodyType::SOLID){
            fixtureRole = FixtureRole::ENVIRONMENT_OBJECT_SOLID;
        }

        if(mSourceEntityCfg->name=="doorA"){
            DummyFunction();
        }

        if(mSourceBody->addBodyFixture(fixtureRole, mSourceSprite)==false){
            return false;
        }


    }else{
        dbgSystem.addMessage("Unknown entity type '" + GetEntityTypeString(mSourceEntityCfg->type) + "'!");
        return false;

    }

    */


    //---
    dbgSystem.removeLastMessage();
    return true;

}


//=============================================================================================


SourceEntitiesGroup::SourceEntitiesGroup()
{
}


SourceEntitiesGroup::~SourceEntitiesGroup()
{
}


bool SourceEntitiesGroup::create(PlayedScene *_scene)
{

    dbgSystem.addMessage("Creating source entitites ...");
    DbgSystem & dbgSystemTMP = dbgSystem;


    /*

    //--- create SourceEntitiesCfg for sprite layer entities as these were not credated via configuration loader
    createSourceEntitiesCfgsForSpriteLayerEntities(_scene, EntityType::BLOCKING_ENVIRONMENT, EntityBodyType::SOLID);


    if(createSingleSpriteSourceEntities(_scene, EntityType::ACTOR)==false){
        return false;
    }

    if(createSingleSpriteSourceEntities(_scene, EntityType::KINEMATIC_TRANSPORTER)==false){
        return false;
    }

    if(createSingleSpriteSourceEntities(_scene, EntityType::ENVIRONMENT_OBJECT)==false){
        return false;
    }

    if(createSpriteLayerSourceEntities(_scene, EntityType::BLOCKING_ENVIRONMENT)==false){
        return false;
    }

    if(createVectorShapeSourceEntities(_scene, EntityType::PATHWAY)==false){
        return false;
    }

    if(createVectorShapeSourceEntities(_scene, EntityType::AREA)==false){
        return false;
    }

    */


    if(createSingleSpriteSourceEntities(_scene)==false){
        return false;
    }

    if(createSpriteLayerSourceEntities(_scene)==false){
        return false;
    }

    if(createVectorShapeSourceEntities(_scene)==false){
        return false;
    }


    //---
    dbgSystem.removeLastMessage();
    return true;
}


bool SourceEntitiesGroup::createSingleSpriteSourceEntities(PlayedScene *_scene)
{

    //const std::string _eType = GetEntityTypeString(_entityType);

    //dbgSystem.addMessage("Creating single sprite entitites of type '" + _eType + "' ...");
    dbgSystem.addMessage("Creating single sprite entitites ...");
    DbgSystem & dbgSystemTMP = dbgSystem;

    std::vector<Sprite*>sprites;

    for(SceneMap *sm : _scene->sceneMaps()){
        CollectSpritesWithParameter(sm->map(), sprites, "entity");
    }


    for(Sprite *s : sprites){


        const std::string &actorName = s->parameters().value("entity");

        if(actorName.empty()){
            dbgSystem.addMessage("Missing 'entity' parameter in a sprite with source sprite '" + s->sourceSprite()->name() + "'!");
            return false;
        }

        SourceEntity *se = getSourceEntitiy(actorName, false);
        if(se){
            continue;       // source entity was already created for this source sprite
        }

        SourceEntityCfg *cfg = getSourceEntitiyCfg(actorName);
        if(cfg==nullptr){
            return false;
        }
        cfg->entityMapElement = EnumEntityMapElement::SPRITE;


        if(cfg->category->role != EntityRole::OBJECT_WITHOUT_SHAPE){

            if(s->type()==SpriteType::STANDARD){

                if(s->sourceSprite()->sourceImages().front()->spriteShapes().empty()){
                    dbgSystem.addMessage("Source sprite '" + s->sourceSprite()->name() + "' has no sprite shapes!");
                    return false;
                }



            }else if(s->type()==SpriteType::COMPOSITE){
                assert(s->sourceSprite()->sourceCompositeSprite());

                std::vector<Layer*>spriteLayers;
                std::string _parameter = "eMainShape";
                CollectLayersWithParameter(s->sourceSprite()->sourceCompositeSprite()->map(), spriteLayers, _parameter, "", LayerKind::SPRITE_LAYER);
                if(spriteLayers.empty()){
                    dbgSystem.addMessage("Source composite sprite '" + s->sourceSprite()->name() + "' is missing a layer with a parameter '" + _parameter +"'! This must be a layer where sprites which define the body shape are placed!" );
                    return false;
                }

            }else{
                continue;

            }
        }


        mSourceEntities.push_back(SourceEntity(cfg));
        se = & mSourceEntities.back();

        if(se->createSourceBody(_scene, s->sourceSprite(), nullptr)==false){
            return false;
        }
    }




    //---
    dbgSystem.removeLastMessage();
    return true;
}


bool SourceEntitiesGroup::createSpriteLayerSourceEntities(PlayedScene *_scene)
{

   // const std::string _eType = GetEntityTypeString(_entityType);

    dbgSystem.addMessage("Creating sprite layer source entitites ...");


    std::vector<Layer*>spriteLayers;

    for(SceneMap *sm : _scene->sceneMaps()){
        CollectLayersWithParameter(sm->map(), spriteLayers, "entity", "", LayerKind::SPRITE_LAYER);
    }

    for(Layer *l : spriteLayers){
        SpriteLayer *sl = static_cast<SpriteLayer*>(l);

        const std::string &entityName = sl->parameters().value("entity");

        if(entityName.empty()){
            dbgSystem.addMessage("Missing 'entity' parameter in the sprite layer '" + sl->name() + "'!");
            return false;
        }

        SourceEntity *se = getSourceEntitiy(entityName, false);
        if(se){
            continue;       // source entity was already created for this source sprite
        }

        SourceEntityCfg *cfg = getSourceEntitiyCfg(entityName);
        if(cfg==nullptr){
            return false;
        }
        cfg->entityMapElement = EnumEntityMapElement::SPRITE_LAYER;


        mSourceEntities.push_back(SourceEntity(cfg));
        se = & mSourceEntities.back();

        if(se->createSourceBody(_scene, nullptr, sl)==false){
            return false;
        }
    }


    //---
    dbgSystem.removeLastMessage();
    return true;
}



bool SourceEntitiesGroup::createVectorShapeSourceEntities(PlayedScene *_scene)
{

    //const std::string _eType = GetEntityTypeString(_entityType);

    dbgSystem.addMessage("Creating vector shape entitites ...");
    DbgSystem & dbgSystemTMP = dbgSystem;

    std::vector<VectorShape*>shapes;

    for(SceneMap *sm : _scene->sceneMaps()){
        CollectVectorShapesWithParameter(sm->map(), shapes, "entity");
    }

    for(VectorShape* vs : shapes){

        const std::string &entityName = vs->parameters().value("entity");

        if(entityName.empty()){
            dbgSystem.addMessage("Missing 'entity' parameter in a vector shape '" + vs->name() + "'!");
            return false;
        }

        SourceEntity *se = getSourceEntitiy(entityName, false);
        if(se){
            continue;       // source entity was already created
        }

        SourceEntityCfg *cfg = getSourceEntitiyCfg(entityName);
        if(cfg==nullptr){
            return false;
        }
        cfg->entityMapElement = EnumEntityMapElement::VECTOR_SHAPE;


        mSourceEntities.push_back(SourceEntity(cfg));
        se = & mSourceEntities.back();

        if(se->createSourceBody(_scene, nullptr, nullptr)==false){
            return false;
        }
    }




    //---
    dbgSystem.removeLastMessage();
    return true;
}


/*
void SourceEntitiesGroup::createSourceEntitiesCfgsForSpriteLayerEntities(PlayedScene *_scene, EntityBodyKind _entityType)
{

    const std::string _eType = GetEntityTypeString(_entityType);


    std::vector<Layer*>spriteLayers;
    for(SceneMap *sm : _scene->sceneMaps()){
        CollectLayersWithParameter(sm->map(), spriteLayers, _eType, "", LayerKind::SPRITE_LAYER);
    }

    for(Layer *l : spriteLayers){
        SpriteLayer *sl = static_cast<SpriteLayer*>(l);

        const std::string &entityName = sl->parameters().value(_eType);

        if(entityName.empty()){
            continue;
        }

        mSourceEntitiesCfgs.emplace_back(SourceEntityCfg(_entityType, entityName, _bodyType));
    }

}
*/



/*
void SourceEntitiesGroup::createSourceEntitiesCfgs_VectorShapeEntities(PlayedScene *_scene, EntityType _entityType)
{

    const std::string _eType = GetEntityTypeString(_entityType);


    std::vector<Layer*>spriteLayers;
    for(SceneMap *sm : _scene->sceneMaps()){
        for(VectorLayer* vl : sm->map()->vectorLayers()){

            for(VectorShape* vs : vl->shapes()){
                const std::string &entityName = vs->parameters().value(_eType);

                if(entityName.empty()){
                    continue;
                }

                mSourceEntitiesCfgs.emplace_back(SourceEntityCfg(_entityType, entityName));
            }

        }
    }

}
*/


SourceEntity* SourceEntitiesGroup::getSourceEntitiy(const std::string &_name, bool _setErrorMessage)
{


    for(SourceEntity & se : mSourceEntities){
        if(se.sourceEntityCfg()->name==_name){
            return &se;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A source entity with name '" + _name + "' not found!");
    }

    return nullptr;

}


SourceEntityCfg* SourceEntitiesGroup::getSourceEntitiyCfg(const std::string &_name, bool _setErrorMessage)
{


    for(SourceEntityCfg & se : mSourceEntitiesCfgs){
        if(se.name==_name){
            return &se;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A source entity cfg with name '" + _name + "' not found!");
    }

    return nullptr;

}

}
