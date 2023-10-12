#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmMap.h"
#include "jmCamera.h"
#include "jmSceneLayout.h"
#include "jmCommonFunctions.h"
#include "jmGuiText.h"
#include "jmSpriteLayer.h"
#include "jmStandardSprite.h"
#include "jmCompositeSprite.h"
#include "jmVectorLayer.h"
#include "jmVectorShape.h"
#include "jmSprite.h"
#include "jmSourceSprite.h"
#include "jmUtilities.h"

#include "items/jpItemsCommon.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpItemsTable.h"
#include "jpUtilities.h"
#include "jpActionsCommon.h"
#include "jpGlobal.h"
#include "gfxObjects/jpAnimationObjects.h"

#include "jpB2Body.h"
#include "jpB2Utilities.h"
#include "jpB2World.h"
#include "jpB2DebugDraw.h"
#include "jpSourceEntity.h"
#include "jpEntityCommon.h"
#include "jpEntity.h"
#include "jpEntityUtilities.h"
#include "jpB2Callbacks.h"
#include "movements/jpMovementBase.h"
#include "movements/jpMovementCommon.h"
#include "movements/jpGroundMovement.h"
#include "movements/jpJumpMovement.h"
#include "movements/jpPathMovement.h"
#include "movements/jpPathPtPMovement.h"
#include "movements/jpAnimatedBehavior.h"
#include "scripting/jpBehavior.h"
#include "scripting/jpBehCfg.h"
#include "task/jpTaskBase.h"
#include "task/jpTaskCommon.h"
#include "task/jpTransporter.h"
#include "jpEntitySystem.h"



namespace jugimap{





EntitySystem::EntitySystem()
{

    mName = "entitySystem";

    //mSourceEntitiesGroup.reset((new SourceEntitiesGroup()));
    //mSourceEntitiesCfgsGroup.reset(new SourceEntitiesCfgsGroup());
    //mEnginesCfgsGroup.reset(new EnginesCfgsGroup());
    mSourceEntitiesGroup.reset(new SourceEntitiesGroup());
    mEnginesControllersCfgsGroup.reset(new EnginesControllersCfgsGroup());

    //mTasksCfgsGroup.reset(new TasksCfgsGroup());
    mTaskControllerCfgsGroup.reset(new EnginesControllersCfgsGroup());

    mExtraGroundTypesCfgsGroup.reset(new ExtraGroundTypesCfgsGroup());
    //mFixtureUserDataGroup.reset(new FixtureUserDataGroup());

    mB2World.reset(new b2World({0.0f, -10.0f}));
    mDebugDraw.reset(new DebugDraw());
    mWorldCameraManager.reset(new WorldCameraManager());

    mEntityContactListener.reset(new EntityContactListener());
    mEntityContactFilter.reset(new EntityContactFilter());

    //mAnimationManager.reset(new AnimationManager());

    mMovementEnginesManager.reset(new MovementEnginesManager());
    mTaskEngineManager.reset(new TaskEngineManager());

    //---
    mMovementEnginesManager->addMovementEngineCreator(new GroundMovementFactory("groundMovement", MovementEngineType::GROUND_MOVEMENT));
    mMovementEnginesManager->addMovementEngineCreator(new JumpMovementFactory("jumpMovement", MovementEngineType::JUMP_MOVEMENT));
    mMovementEnginesManager->addMovementEngineCreator(new PathMovementFactory("pathMovement", MovementEngineType::PATHWAY_MOVEMENT));
    mMovementEnginesManager->addMovementEngineCreator(new PathPTPMovementFactory("pathPointToPointMovement", MovementEngineType::PATH_POINT_TO_POINT_MOVEMENT));
    mMovementEnginesManager->addMovementEngineCreator(new AnimatedMovementFactory("animatedBehavior", MovementEngineType::ANIMATED_BEHAVIOR));

    //---
    mTaskEngineManager->addTaskEngineFactory(new TransporterFactory("transporter", TaskType::POINT_TO_POINT_TRANSPORTER));

    //---
    mEntitySignalsParser.reset(new EntitySignalsParser({"ENTITY"}));
    app->signalParserManager()->addSignalParser(mEntitySignalsParser.get());

}


EntitySystem::~EntitySystem()
{
}


bool EntitySystem::initCfg(PlayedScene *playerScene, const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading entity system ...");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){

        std::string nodeName = std::string(n.name());

        if(nodeName=="entityCategories"){

            if(mContactCategories.initCfg(n)==false){
                return false;
            }

         //------
        }else if(nodeName=="extraGroundTypes"){

            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                std::string childNodeName = std::string(nChild.name());

                std::string name = nChild.attribute("name").as_string("");

                if(childNodeName=="slidingGround"){
                    SlidingGroundCfg * cfg = new SlidingGroundCfg(name);
                    mExtraGroundTypesCfgsGroup->slidingGroundCfgs().push_back(cfg);
                    if(cfg->initCfg(nChild)==false){
                        return false;
                    }

                }else if(childNodeName=="conveyorGround"){
                    ConveyorGroundCfg * cfg = new ConveyorGroundCfg(name);
                    mExtraGroundTypesCfgsGroup->conveyorGroundCfgs().push_back(cfg);
                    if(cfg->initCfg(nChild)==false){
                        return false;
                    }
                }
            }

        //------
        }else if(nodeName=="behaviorEngines"){

            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                std::string childNodeName = std::string(nChild.name());

                std::string name = nChild.attribute("name").as_string("");

                MovementEngineCfg *cfg = mMovementEnginesManager->addMovementEngineCfg(childNodeName, name);
                if(cfg==nullptr){
                    return false;
                }
                if(cfg->initCfg(nChild)==false){
                    return false;
                }
            }


         //------
        }else if(nodeName=="taskEngines"){

            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                std::string childNodeName = std::string(nChild.name());

                std::string name = nChild.attribute("name").as_string("");

                TaskEngineCfg *cfg = mTaskEngineManager->addTaskEngineCfg(childNodeName, name);
                if(cfg==nullptr){
                    return false;
                }
                if(cfg->initCfg(nChild)==false){
                    return false;
                }
            }


        //------
        }else if(nodeName=="behaviorControllers"){

            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                std::string childNodeName = std::string(nChild.name());

                if(childNodeName=="behaviorController"){
                    std::string controllerName = nChild.attribute("name").as_string("");
                    EntityControllerCfg * controllercCfg = mEnginesControllersCfgsGroup->addEnginesControllerCfg(controllerName);
                    if(controllercCfg->initCfg(nChild)==false){
                        return false;
                    }
                }
            }


        //------
        }else if(nodeName=="taskControllers"){

            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                std::string childNodeName = std::string(nChild.name());

                if(childNodeName=="taskController"){
                    std::string controllerName = nChild.attribute("name").as_string("");
                    EntityControllerCfg * controllerCfg = mTaskControllerCfgsGroup->addEnginesControllerCfg(controllerName);
                    if(controllerCfg->initCfg(nChild)==false){
                        return false;
                    }
                }
            }


        }else if(nodeName=="entities"){

            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                std::string childNodeName = std::string(nChild.name());

                if(childNodeName=="entity"){
                    std::string name = nChild.attribute("name").as_string("");
                    std::string mainShapeCategory = nChild.attribute("mainCategory").as_string("");

                    EntityCategory *ec = mContactCategories.getEntityCategory(mainShapeCategory);
                    if(ec==nullptr){
                        return false;
                    }
                    if(ec->shapeRole!=EntityCategory::ShapeRole::MAIN_SHAPE){
                        dbgSystem.addMessage(" Entity '" + name + "' has assigned category '" + mainShapeCategory +"' which is not a main category!" );
                        return false;
                    }


                    mSourceEntitiesGroup->sourceEntitiesCfgs().push_back(SourceEntityCfg(name, ec));
                    SourceEntityCfg & seCfg = mSourceEntitiesGroup->sourceEntitiesCfgs().back();
                    seCfg.entitySystem = this;

                    if(seCfg.initCfg(nChild)==false){
                        return false;
                    }

                }
            }
        }
    }


    dbgSystem.removeLastMessage();

    return true;

}



bool EntitySystem::createMapEntities(PlayedScene *_scene)
{

    dbgSystem.addMessage("Creating map entitites ...");

    //SourceEntitiesGroup *seg = _scene->sourceEntitiesGroup();

    /*
    for(SourceEntity &se :  seg->sourceEntities()){

        const std::string &sourceEntityName = se.sourceEntityCfg()->name;
        const std::string &entityType = GetEntityTypeString(se.type());


        if(se.type()==EntityType::ACTOR){

            std::vector<Sprite*>sprites;
            for(SceneMap *sm : _scene->sceneMaps()){
                CollectSpritesWithParameter(sm->map(), sprites, GetEntityTypeString(EntityType::ACTOR), sourceEntityName);
            }

            for(Sprite *s : sprites){
                if(s->type()!=SpriteType::STANDARD){
                    continue;
                }
                StandardSprite *stdSprite = static_cast<StandardSprite*>(s);
                //assert(s->sourceSprite()==se.sourceSprite());
                if(s->sourceSprite() != se.sourceSprite()){
                    dbgSystem.addMessage("Actor '" + se.sourceEntityCfg()->name +"' source sprite error !");
                    dbgSystem.addMessage("Different source sprites can not have assigned the same 'actor' entity !");
                    return false;
                }

                Actor *a = new Actor(&se);

                if(a->build(stdSprite)==false){
                    return false;
                }
                mActors.push_back(a);
            }

        }else if(se.type()==EntityType::KINEMATIC_TRANSPORTER){

            std::vector<Sprite*>sprites;
            for(SceneMap *sm : _scene->sceneMaps()){
                CollectSpritesWithParameter(sm->map(), sprites, GetEntityTypeString(EntityType::KINEMATIC_TRANSPORTER), sourceEntityName);
            }

            for(Sprite *s : sprites){
                //if(s->type()!=SpriteType::STANDARD){
                //    continue;
                //}
                if(s->type()!=SpriteType::STANDARD && s->type()!=SpriteType::COMPOSITE){
                    continue;
                }
                //StandardSprite *stdSprite = static_cast<StandardSprite*>(s);
                //assert(s->sourceSprite()==se.sourceSprite());
                if(s->sourceSprite() != se.sourceSprite()){
                    dbgSystem.addMessage("Transporter '" + se.sourceEntityCfg()->name +"' source sprite error !");
                    dbgSystem.addMessage("Different source sprites can not have assigned the same 'transporter' entity !");
                    return false;
                }

                Actor *a = new Actor(&se);

                if(a->build(s)==false){
                    return false;
                }
                mActors.push_back(a);
            }


        }else if(se.type()==EntityType::BLOCKING_ENVIRONMENT){

            if(se.sourceSprite()==nullptr){         // sprite layer

                std::vector<Layer*>spriteLayers;
                for(SceneMap *sm : _scene->sceneMaps()){
                    CollectLayersWithParameter(sm->map(), spriteLayers, GetEntityTypeString(EntityType::BLOCKING_ENVIRONMENT),
                                               se.name(), LayerKind::SPRITE_LAYER);
                }

                for(Layer *l : spriteLayers){
                    SpriteLayer *sl = static_cast<SpriteLayer*>(l);

                    EStaticTerrain *terrainEntity = new EStaticTerrain(&se);
                    if(terrainEntity->build()==false){
                        return false;
                    }

                    mTerrainEntities.push_back(terrainEntity);
                }
            }


        }else if(se.type()==EntityType::PATHWAY){

            assert(se.sourceSprite()==nullptr);

            std::vector<VectorShape*>collectedShapes;

            for(SceneMap *sm : _scene->sceneMaps()){
                CollectVectorShapesWithParameter(sm->map(), collectedShapes, GetEntityTypeString(EntityType::PATHWAY), se.name());
            }

            for(VectorShape* vs : collectedShapes){

                EPathway *pathwayEntity = new EPathway(&se, vs);
                if(pathwayEntity->build()==false){
                    return false;
                }
                mPathways.push_back(pathwayEntity);
            }

            // connect pathways
            for(EPathway* pathway1 : mPathways){
                for(EPathway* pathway2 : mPathways){
                    if(pathway1==pathway2) continue;
                    PathwayOrientation orientation1 = pathway1->sourceEntity()->sourceEntityCfg()->mPathwaySourceEntityCfg->orientation;
                    PathwayOrientation orientation2 = pathway2->sourceEntity()->sourceEntityCfg()->mPathwaySourceEntityCfg->orientation;
                    if(orientation1 != orientation2) continue;

                    std::vector<PathPoint> &path1 = pathway1->vectorShape()->pathPoints();
                    std::vector<PathPoint> &path2 = pathway2->vectorShape()->pathPoints();

                    if(orientation1==PathwayOrientation::VERTICAL){
                        if(path1.back().y > path1.front().y != path2.back().y > path2.front().y) continue;  // shapes must be created into the same direction

                    }else if(orientation1==PathwayOrientation::HORIZONTAL){
                        if(path1.back().x > path1.front().x != path2.back().x > path2.front().x) continue;  // shapes must be created into the same direction

                    }

                    float dist = DistanceTwoPoints(path1.back(), path2.front());
                    if(dist == 0.0f){
                        pathway1->_setConnectedPathwayNext(pathway2);
                        continue;
                    }
                    dist = DistanceTwoPoints(path1.front(), path2.back());
                    if(dist == 0.0f){
                        pathway1->_setConnectedPathwayPrev(pathway2);
                        continue;
                    }

                }
            }

        }
    }
    */


    for(SourceEntity &se : mSourceEntitiesGroup->sourceEntities()){

        const std::string &sourceEntityName = se.sourceEntityCfg()->name;
        //const std::string &entityType = GetEntityTypeString(se.type());


        if(se.sourceSprite()){

            // sprite based entities

            std::vector<Sprite*>sprites;
            for(SceneMap *sm : _scene->sceneMaps()){
                CollectSpritesWithParameter(sm->map(), sprites, "entity", sourceEntityName);
            }

            for(Sprite *s : sprites){
                //if(s->type()!=SpriteType::STANDARD){
                if(s->type()!=SpriteType::STANDARD && s->type()!=SpriteType::COMPOSITE){
                    continue;
                }
                //StandardSprite *stdSprite = static_cast<StandardSprite*>(s);
                if(s->sourceSprite() != se.sourceSprite()){
                    dbgSystem.addMessage("Entity '" + se.sourceEntityCfg()->name +"' source sprite error !");
                    dbgSystem.addMessage("Different source sprites can not have assigned the same entity !");
                    return false;
                }

                Entity *a = new Entity(&se);

                if(a->build(s, nullptr, nullptr)==false){
                    return false;
                }
                mActors.push_back(a);
            }


        //}else if(se.type()==EntityType::BLOCKING_ENVIRONMENT){
        }else if(se.sourceEntityCfg()->entityMapElement==EnumEntityMapElement::SPRITE_LAYER){

            //if(se.sourceSprite()==nullptr){         // sprite layer

                std::vector<Layer*>spriteLayers;
                for(SceneMap *sm : _scene->sceneMaps()){
                    CollectLayersWithParameter(sm->map(), spriteLayers, "entity", se.name(), LayerKind::SPRITE_LAYER);
                }

                for(Layer *l : spriteLayers){
                    SpriteLayer *sl = static_cast<SpriteLayer*>(l);

                    Entity *terrainEntity = new Entity(&se);
                    if(terrainEntity->build(nullptr, nullptr, sl)==false){
                        return false;
                    }

                    mActors.push_back(terrainEntity);
                }
            //}


        //}else if(se.type()==EntityType::PATHWAY){
        }else if(se.sourceEntityCfg()->entityMapElement==EnumEntityMapElement::VECTOR_SHAPE){

            assert(se.sourceSprite()==nullptr);

            std::vector<VectorShape*>collectedShapes;

            for(SceneMap *sm : _scene->sceneMaps()){
                CollectVectorShapesWithParameter(sm->map(), collectedShapes, "entity", se.name());
            }

            for(VectorShape* vs : collectedShapes){

                Entity *pathwayEntity = new Entity(&se);
                if(pathwayEntity->build(vs->parentLayer()->parentCompositeSprite(), vs, nullptr)==false){
                    return false;
                }
                mActors.push_back(pathwayEntity);
            }

            // connect pathways
            /*
            for(EPathway* pathway1 : mPathways){
                for(EPathway* pathway2 : mPathways){
                    if(pathway1==pathway2) continue;
                    PathwayOrientation orientation1 = pathway1->sourceEntity()->sourceEntityCfg()->mPathwaySourceEntityCfg->orientation;
                    PathwayOrientation orientation2 = pathway2->sourceEntity()->sourceEntityCfg()->mPathwaySourceEntityCfg->orientation;
                    if(orientation1 != orientation2) continue;

                    std::vector<PathPoint> &path1 = pathway1->vectorShape()->pathPoints();
                    std::vector<PathPoint> &path2 = pathway2->vectorShape()->pathPoints();

                    if(orientation1==PathwayOrientation::VERTICAL){
                        if(path1.back().y > path1.front().y != path2.back().y > path2.front().y) continue;  // shapes must be created into the same direction

                    }else if(orientation1==PathwayOrientation::HORIZONTAL){
                        if(path1.back().x > path1.front().x != path2.back().x > path2.front().x) continue;  // shapes must be created into the same direction

                    }

                    float dist = DistanceTwoPoints(path1.back(), path2.front());
                    if(dist == 0.0f){
                        pathway1->_setConnectedPathwayNext(pathway2);
                        continue;
                    }
                    dist = DistanceTwoPoints(path1.front(), path2.back());
                    if(dist == 0.0f){
                        pathway1->_setConnectedPathwayPrev(pathway2);
                        continue;
                    }

                }
            }
            */

        }

        /*
        else if(se.type()==EntityType::AREA){

            assert(se.sourceSprite()==nullptr);

            std::vector<VectorShape*>collectedShapes;

            for(SceneMap *sm : _scene->sceneMaps()){
                CollectVectorShapesWithParameter(sm->map(), collectedShapes, GetEntityTypeString(EntityType::AREA), se.name());
            }

            for(VectorShape* vs : collectedShapes){

                Actor * a = new Actor(&se);
                //EPathway *pathwayEntity = new EPathway(&se, vs);
                //if(pathwayEntity->build()==false){
                //    return false;
                //}
                //mPathways.push_back(pathwayEntity);

                if(a->build(vs->parentLayer()->parentCompositeSprite(), vs)==false){
                    return false;
                }
                mActors.push_back(a);
            }
        }
        */
    }


    std::sort(mActors.begin(), mActors.end(),
              [](const Entity* e1, const Entity* e2) -> bool
                { return e1->sourceEntity()->sourceEntityCfg()->category->updateOrder < e2->sourceEntity()->sourceEntityCfg()->category->updateOrder; });


    /*
    // standalone animation objects
    std::vector<Sprite*>sprites;
    for(SceneMap *sm : _scene->sceneMaps()){
        CollectSpritesWithParameter(sm->map(), sprites, "aniObject");
    }


    typedef std::pair<std::string, std::vector<Sprite*>> SpritesGroup;
    std::vector<SpritesGroup>spritesGroups;

    for(Sprite *s : sprites){
        const std::string &name = s->parameters().value("aniObject");

        SpritesGroup *spritesGroup = nullptr;
        for(SpritesGroup &sg : spritesGroups){
            if(name == sg.first){
                spritesGroup = &sg;
                break;
            }
        }

        if(spritesGroup==nullptr){
            spritesGroups.emplace_back(SpritesGroup(name, std::vector<Sprite*>({s})));

        }else{
            spritesGroup->second.push_back(s);
        }
    }


    for(SpritesGroup &group : spritesGroups){
        mAnimationManager->animationObjects().push_back(new AnimationObject(group.first, group.second));
    }
    */

    //---
    dbgSystem.removeLastMessage();
    return true;
}


bool EntitySystem::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing entity system ... ");

    mParentPlayerScene = _scene;


    if(mSourceEntitiesGroup->create(_scene)==false){
        return false;
    }

    //---
    if(createMapEntities(_scene)==false){
        return false;
    }

    //if(entitySystem->initConections(_scene)==false){
    //    return false;
    //}



    //---
    //dbgSystem.removeLastMessage();
    //return true;

    //dbgSystem.addMessage("Initializing entity system ...");

    //mParentPlayerScene = _scene;


    //createMapEntities(_scene);


    for(Entity *a : mActors){
        if(a->initConnections(_scene)==false){
            return false;
        }
    }

    if(mWorldCameraManager->initConnections(_scene)==false){
        return false;
    }
    mWorldCameraManager->setFocusEntity(getActor("hero"));


    mB2World->SetDebugDraw(mDebugDraw.get());
    mB2World->SetContactListener(mEntityContactListener.get());
    mB2World->SetContactFilter(mEntityContactFilter.get());

    //



        // standalone animation objects
    //if(mAnimationManager->initConnections(_scene)==false){
    //    return false;
    //}


    //---
    dbgSystem.removeLastMessage();
    return true;
}


void EntitySystem::start()
{
    for(Entity *a : mActors){
        if(a->sourceEntity()->sourceEntityCfg()->name=="hero"){
            DummyFunction();
        }
        a->start();
    }

}


bool EntitySystem::startingPhaseUpdate()
{

    bool startingPhase = false;


    //for(Entity *a : mActors){
    //    a->preUpdate(UpdateMode::NORMAL);
    //}

    for(Entity *a : mActors){
        if(a->sourceEntity()->sourceEntityCfg()->name=="hero"){
            DummyFunction();
        }
        if(a->startingPhaseUpdate()==true){
            startingPhase = true;
        }
    }

    //for(Entity *a : mActors){
    //    a->postUpdate(UpdateMode::NORMAL);
    //}

    mB2World->Step(mTimeStepS, mVelocityIterations, mPositionIterations);


    for(Entity *a : mActors){
        a->updateSpriteTransform();
    }

    mWorldCameraManager->update();

    return startingPhase;
}


void EntitySystem::update(UpdateMode _updateMode)
{

    if(_updateMode==UpdateMode::MODAL_OVERLAY){
        return;
    }


    //--- PRE UPDATE
    for(Entity *a : mActors){
        if(a->sourceEntity()->sourceEntityCfg()->category->mB2BodyType==b2BodyType::b2_dynamicBody){
            a->preUpdate_resolveContacts();
        }
    }

    for(Entity *a : mActors){
        a->preUpdate(_updateMode);
    }
    mFilteredContactTriggersGroup.preUpdate_postContactTriggePreUpdate();

    for(Entity *a : mActors){
        if(a->mainShapeRole()==EntityRole::ACTOR){
            a->preUpdate_CheckActorGrouping();
        }
    }


    for(Entity *a : mActors){
        if(a->entityMovingGroup()) continue;
        a->update_Movement(_updateMode);
    }
    for(EntityMovingGroup *eg : mEntityMovingGroupsManager.entityMovingGroups()){
        eg->update_Movement(_updateMode);
    }


    //--- UPDATE
    for(Entity *a : mActors){
        a->update_Controller(_updateMode);
    }


    //--- POST UPDATE
    for(Entity *a : mActors){
        a->postUpdate(_updateMode);
    }
    mFilteredContactTriggersGroup.postUpdate();



    //----
    mB2World->Step(mTimeStepS, mVelocityIterations, mPositionIterations);


    //----
    for(Entity *a : mActors){
        a->updateSpriteTransform();
    }

    //mAnimationManager->update(mTimeStepS);
    mWorldCameraManager->update();

    //player->resolveContacts();

    //EBehavior_Player_Gravity *b = static_cast<EBehavior_Player_Gravity*>(player->behavior());
    //print("Player velocity x=" + std::to_string(b->velocity().x) + " y=" + std::to_string(b->velocity().y) +
    //      " body vel x=" + std::to_string(player->body()->GetLinearVelocity().x) + " y=" + std::to_string(player->body()->GetLinearVelocity().y) +
    //      " rel vel x=" + std::to_string(b->relVelocity().x) + " y=" + std::to_string(b->relVelocity().y) +
    //     " press vel x=" + std::to_string(b->pressureVelocity().x) + " y=" + std::to_string(b->pressureVelocity().y));

}


void EntitySystem::_update(UpdateMode _updateMode, std::vector<Entity *> &_entities)
{


    //---
    for(Entity *a : _entities){
        if(a->sourceEntity()->sourceEntityCfg()->category->mB2BodyType==b2BodyType::b2_dynamicBody){
            a->preUpdate_resolveContacts();
        }
    }

    //----
    for(Entity *a : _entities){
        a->preUpdate(_updateMode);
    }

    //---
    for(Entity *a : _entities){
        a->update_Controller(_updateMode);
    }

    //---
    for(Entity *a : _entities){
        a->postUpdate(_updateMode);
    }

}


void EntitySystem::drawShapes(ShapeDrawer &_drawer, int _flags)
{

    //if(mDoDebugDraw){
        //mDrawer.draw(mMap.get(), mCameraManager.matrix(), mCameraManager.scale());
    if(_flags & DebugDrawFlags::SHAPES){

        if(mWorldCameraManager->camera()){
            mDebugDraw->mMCamera = mWorldCameraManager->camera()->GetMatrix();
            mDebugDraw->mCameraScale =  mWorldCameraManager->camera()->GetScale();
        }

        uint32 flags = 0;
        flags |= b2Draw::e_shapeBit;
        flags |= b2Draw::e_jointBit;
        //flags |= b2Draw::e_pairBit;
        //flags |= b2Draw::e_aabbBit;
        //flags |= b2Draw::e_centerOfMassBit;
        mDebugDraw->SetFlags(flags);

        mDebugDraw->Begin();
        mB2World->DebugDraw();
        mDebugDraw->Flush();
    }
}


void EntitySystem::onStateEnded()
{

}


Entity* EntitySystem::getActor(const std::string &_name, bool _setErrorMessage)
{

    for(Entity *a : mActors){
        if(a->sourceEntity()->name()==_name){
            return a;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("An actor '" + _name + "' not found!");
    }

    return nullptr;
}


Entity* EntitySystem::getActor(const std::string &_name, int _linkedGroupID, int _linkedEntityID, CompositeSprite *_rootCompositeSprite, bool _setErrorMessage)
{

    for(Entity *a : mActors){
        if(a->sourceEntity()->name()==_name){
            if(_rootCompositeSprite){
                // entities with the same root composite sprite are automatically grouped without need for '_linkedGroupID' paramater
                if(_rootCompositeSprite == a->mapElement().rootCompositeSprite()){
                    if(a->linkedEntityID()==_linkedEntityID){
                        return a;
                    }
                }
            }
            if(a->linkedGroupID()==_linkedGroupID && a->linkedEntityID()==_linkedEntityID){
                return a;
            }


        }
        //if(a->sourceEntity()->name()==_name && a->linkedGroupID()==_linkedGroupID && a->linkedEntityID()==_linkedEntityID){
        //    return a;
        //}
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("An actor '" + _name + "' with linkedGroupID '" + std::to_string(_linkedGroupID)+
                             "' and linkedEntityID '" + std::to_string(_linkedEntityID) + " not found!");
    }

    return nullptr;
}


void EntitySystem::collectActorsViaLinkedGroupID(std::vector<Entity *> &_collectedActors, const std::string &_name, int _linkedGroupID)
{


    for(Entity *a : mActors){
        if(a->sourceEntity()->name()==_name && a->linkedGroupID()==_linkedGroupID){
            _collectedActors.push_back(a);
        }
    }

}



}
