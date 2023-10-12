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
#include "jmTextSprite.h"
#include "jmUtilities.h"

#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"

#include "scripting/jpBehavior.h"
#include "jpB2Body.h"
#include "jpB2World.h"
#include "jpEntity.h"
#include "jpEntityUtilities.h"
#include "jpEntityCommon.h"
#include "jpSourceEntity.h"
#include "jpEntitySystem.h"
#include "jpBehConditions.h"



namespace jugimap{



/*
std::string ACOnContact::type_active = "onContact";
std::string ACOnContact::type_activeStart = "onContactStart";
std::string ACOnContact::type_activeEnd = "onContactEnd";





ACOnContact::ACOnContact(BehaviorAction *_parentEntityAction, const std::string &_value, const std::string &_type)
{

    mParentBehaviorAction = _parentEntityAction;
    mValue = _value;

    if(_type==type_active){
        mStyle = Style::ACTIVE;

    }else if(_type==type_activeStart){
        mStyle = Style::ACTIVE_START;

    }else if(_type==type_activeEnd){
        mStyle = Style::ACTIVE_END;

    }else{
        assert(false);
    }


}


bool ACOnContact::initConnections(PlayedScene *scene)
{

    if(mStyle==Style::ACTIVE){
        dbgSystem.addMessage("Initializing '" + type_active +"' ...");

    }else if(mStyle==Style::ACTIVE_START){
        dbgSystem.addMessage("Initializing '" + type_activeStart +"' ...");

    }else if(mStyle==Style::ACTIVE_END){
        dbgSystem.addMessage("Initializing '" + type_activeEnd +"' ...");
    }

     Actor *parentEntity = static_cast<Actor*>(mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor());

    std::vector<std::string>pathParts = StdString::splitString(mValue, ":");

    if(pathParts.size()!=2){
        dbgSystem.addMessage("Wrong path '"+ mValue +"!");
        return false;
    }

    const std::string &contactFixture = pathParts[0];
    const std::string &ccCategories = pathParts[1];

    if(contactFixture=="BODY"){
        mContactFixture = FixtureRole::ACTOR_BODY;

    }else{
        mContactFixture = FixtureRole::CUSTOM;
        // To DO !
    }

    if(ccCategories=="ANY_ACTOR"){

        mContactCategoriesFlags = 1 << 31 ;

    }else{

        ContactCategories &contactCategories = app->entitySystem()->contactCategories();

        std::vector<std::string>ccNames = StdString::splitString(ccCategories, ",");

        for(const std::string &ccName : ccNames){

            int index = contactCategories.getCollisionCategoryBitIndex(ccName);
            if(index==-1){
                return false;
            }

            mContactCategoriesFlags |= index;
        }
    }


    dbgSystem.removeLastMessage();
    return true;
}



bool ACOnContact::isConditionTrue()
{

     Actor *actor = static_cast<Actor*>(mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor());

     std::vector<Entity*>* overlappingEntities = nullptr;

    if(mStyle==Style::ACTIVE_START){
        overlappingEntities = &(actor->overlappedEntitiesStart());

    }else if(mStyle==Style::ACTIVE){
        overlappingEntities = &(actor->overlappedEntities());

    }else if(mStyle==Style::ACTIVE_END){
        overlappingEntities = &(actor->overlappedEntitiesEnd());

    }

    if(overlappingEntities->empty()){
        return false;
    }


    print("ACOnContact TRUE");

    //for(Entity* e : overlappingEntities){
    //    if(a->sourceEntity()->sourceEntityCfg()->contactCategories & mContactCategoriesFlags){
    //        return true;
    //    }
    //}



   return true;

}

*/


//===================================================================================

/*
std::string OnGroundMovementState::type = "onGroundMovementState";


OnGroundMovementState::OnGroundMovementState(BehaviorAction *_parentEntityAction, const std::string &_value)
{

    mParentBehaviorAction = _parentEntityAction;
    mValue = _value;

}


bool OnGroundMovementState::initConnections(PlayedScene *scene)
{

    dbgSystem.addMessage("Initializing '" + type + "' ...");


    std::vector<std::string>parts = StdString::splitString(mValue, "|");

    for(const std::string & s : parts){
       GroundMovementState state = GetGroundMovementStateFromString(s);
       if(state==GroundMovementState::UNKNOWN){
           dbgSystem.addMessage("Wrong ground movement state '" + s + "'!");
           return false;
       }
       mStates.push_back(state);
    }


    dbgSystem.removeLastMessage();
    return true;

}


bool OnGroundMovementState::isConditionTrue()
{

    //Behavior *behavior = mParentBehaviorAction->parentBehaviorState()->parentStateManager();
    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    for(GroundMovementState s : mStates){
        if(actor->groundMovementEngine()->state() == s){
            return true;
        }
    }

   return false;

}

*/


//===================================================================================

/*

std::string OnJumpMovementState::type = "onJumpMovementState";


OnJumpMovementState::OnJumpMovementState(BehaviorAction *_parentEntityAction, const std::string &_value)
{

    mParentBehaviorAction = _parentEntityAction;
    mValue = _value;

}


bool OnJumpMovementState::initConnections(PlayedScene *scene)
{

    dbgSystem.addMessage("Initializing '" + type + "' ...");


    std::vector<std::string>parts = StdString::splitString(mValue, "|");

    for(const std::string & s : parts){
       JumpMovementState state = GetJumpMovementStateFromString(s);
       if(state==JumpMovementState::UNKNOWN){
           dbgSystem.addMessage("Wrong jump movement state '" + s + "'!");
           return false;
       }
       mStates.push_back(state);
    }


    dbgSystem.removeLastMessage();
    return true;

}


bool OnJumpMovementState::isConditionTrue()
{

    Entity *entity = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    for(JumpMovementState s : mStates){
        if(entity->jumpMovementEngine()->state() == s){
            return true;
        }
    }

   return false;
}

*/

//===================================================================================

/*

std::string OnGroundDetected::type = "onGroundDetected";


OnGroundDetected::OnGroundDetected(BehaviorAction *_parentEntityAction, const std::string &_value)
{


    mParentBehaviorAction = _parentEntityAction;
    mValue = _value;

}


bool OnGroundDetected::initConnections(PlayedScene *scene)
{

    dbgSystem.addMessage("Initializing '" + type + "' ...");


    if(mValue=="TRUE"){
        value = true;

    }else{
        value = false;
    }



    dbgSystem.removeLastMessage();
    return true;

}


bool OnGroundDetected::isConditionTrue()
{

    //Behavior *behavior = mParentBehaviorAction->parentBehaviorState()->parentStateManager();
    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    bool groundDetected = (actor->blockedDirectionsRef() & static_cast<int>(Direction::DOWN)) ;

    bool isTrue = (groundDetected == value);


    return isTrue;

}

*/

//===================================================================================
/*

std::string OnPatwayDetected::type = "onPatwayDetected";


OnPatwayDetected::OnPatwayDetected(BehaviorAction *_parentEntityAction, const std::string &_value)
{

    mParentBehaviorAction = _parentEntityAction;
    mValue = _value;

}


bool OnPatwayDetected::initConnections(PlayedScene *scene)
{

    dbgSystem.addMessage("Initializing '" + type + "' ...");


    std::vector<std::string>pathParts = StdString::splitString(mValue, ",");

    if(pathParts.size()==2){

        mPatwayName = pathParts[0];

        const std::string &availableDirection = pathParts[1];
        if(availableDirection=="FORWARD"){
            mAvailableDirection = AvailableDirection::FORWARD;
        }else if(availableDirection=="BACKWARD"){
            mAvailableDirection = AvailableDirection::BACKWARD;
        }
    }

    if(mPatwayName.empty()){
        dbgSystem.addMessage("Wrong value '"+ mValue +"' !");
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}


bool OnPatwayDetected::isConditionTrue()
{

    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();
    Entity *pathway = nullptr;

    //for(Entity* e : actor->overlappedEntities()){
    for(EntityContactTrigger& ect : actor->contactTrigger().contactedEntitiesTriggers()){
        assert(actor == ect.mTwoEntitiesContact.entityShapeA().entity);
        Entity* e = ect.mTwoEntitiesContact.entityShapeB().entity;
        //if(e->sourceEntity()->type()==EntityType::PATHWAY){
        if(e->mainShapeRole()==EntityRole::PATHWAY){
            pathway = static_cast<Entity*>(e);
            break;
        }
    }

    if(pathway==nullptr){
        return false;
    }





    return true;

}

*/


//===================================================================================

/*

std::string OnPatwayAccessed::type = "onPatwayAccessed";


OnPatwayAccessed::OnPatwayAccessed(BehaviorAction *_parentEntityAction, const std::string &_value)
{

    mParentBehaviorAction = _parentEntityAction;
    mValue = _value;

}


bool OnPatwayAccessed::initConnections(PlayedScene *scene)
{

    dbgSystem.addMessage("Initializing '" + type + "' ...");


    std::vector<std::string>pathParts = StdString::splitString(mValue, ",");

    if(pathParts.size()==2){

        mPatwayName = pathParts[0];
        if(mPatwayName.empty()){
            dbgSystem.addMessage("Wrong value '"+ mValue +"' !");
            return false;
        }

        const std::string &patwayData = pathParts[1];

        Entity *actor = static_cast<Entity*>(mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor());

        PathMovementEngine *pathwayMovementEngine = actor->pathMovementEngine();

        if(pathwayMovementEngine==nullptr){
            dbgSystem.addMessage("Actore has no pathway movement engine!");

        }

        for(PathMovementData &d : pathwayMovementEngine->pathMovementDatas()){
            if(d.cfg->name==patwayData){
                mPathMovementData = &d;
                break;
            }
        }

        if(mPathMovementData==nullptr){
            dbgSystem.addMessage("Path movement data '" + patwayData +" ' not found!");

        }

    }

    dbgSystem.removeLastMessage();
    return true;

}


bool OnPatwayAccessed::isConditionTrue()
{

    Entity *actor = static_cast<Entity*>(mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor());


    // check commands for accessing pathway
    if(mPathMovementData->moveForward.active()==false && mPathMovementData->moveBackward.active()==false){
        return false;
    }

     Direction direction = (mPathMovementData->moveForward.active()==true)? Direction::FORWARD : Direction::BACKWARD;

    //EPathway *pathway = nullptr;

    //for(Entity* e : actor->overlappedEntities()){
     for(EntityContactTrigger& ect : actor->contactTrigger().contactedEntitiesTriggers()){
         assert(actor == ect.mTwoEntitiesContact.entityShapeA().entity);
         Entity* e = ect.mTwoEntitiesContact.entityShapeB().entity;

        if(e->mainShapeRole()==EntityRole::PATHWAY && e->sourceEntity()->name()==mPatwayName){
            Entity *pathway = static_cast<Entity*>(e);
            PathwaySourceEntityCfg * pathwayCfg = pathway->sourceEntity()->sourceEntityCfg()->mPathwaySourceEntityCfg.get();

            // check if pathway was accessed
            PathMovementEngine *pathwayMovementEngine = actor->pathMovementEngine();

            mPathMovementData->vectorShape = pathway->vectorShape();
            mPathMovementData->pathwayOrientation = pathwayCfg->orientation;
            //mPathMovementData->centeringMarginM = pathwayCfg->centeringMarginM;
            mPathMovementData->centeringMarginM = pathway->sourceEntity()->sourceEntityCfg()->centeringMargin;
            mPathMovementData->centeringStyle = pathwayCfg->centeringStyle;
            mPathMovementData->actorPathwayContactOffsetM = pathwayCfg->getContactPointOffsetForActor(actor->sourceEntity()->name());
            mPathMovementData->pathway = pathway;

            b2Vec2 actorPosM = actor->body()->B2Body()->GetPosition();

            if(pathwayMovementEngine->initIfPathwayAccessed(mPathMovementData, direction, &actor->animationPlayer(), actorPosM)){
                return true;
            }
        }
    }



    mPathMovementData->vectorShape = nullptr;    // ! ('vectorShape' is used as flag that pathwayMovementEngine is ready to be used)
    mPathMovementData->pathway = nullptr;

    return false;

}

*/


//===================================================================================

/*
std::string OnPatwayEnd::type = "onPathwayEnd";


OnPatwayEnd::OnPatwayEnd(BehaviorAction *_parentEntityAction, const std::string &_value)
{

    mParentBehaviorAction = _parentEntityAction;
    mValue = _value;

}


bool OnPatwayEnd::initConnections(PlayedScene *scene)
{

    dbgSystem.addMessage("Initializing '" + type + "' ...");


    if(mValue=="TRUE"){
        value = true;

    }else{
        value = false;
    }


    dbgSystem.removeLastMessage();
    return true;

}


bool OnPatwayEnd::isConditionTrue()
{

    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    assert(actor->currentEngine()->type()==BehaviorEngineType::PATHWAY_MOVEMENT);
    PathMovementEngine *engine = static_cast<PathMovementEngine*>(actor->currentEngine());
    PathwayOrientation pathwayOrientation = engine->currentData()->pathwayOrientation;
    VectorShape* path = engine->currentData()->vectorShape;



    if(engine->isPathwayFinished(false)){
        return true;
    }

    return false;

}

*/

//===================================================================================

/*

std::string OnPathwayToGroundMovement::type = "onPathwayToGroundMovement";


OnPathwayToGroundMovement::OnPathwayToGroundMovement(BehaviorAction *_parentEntityAction, const std::string &_value)
{

    mParentBehaviorAction = _parentEntityAction;
    mValue = _value;

}


bool OnPathwayToGroundMovement::initConnections(PlayedScene *scene)
{

    dbgSystem.addMessage("Initializing '" + type + "' ...");


    const std::string &groundMovementData = mValue;

    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    GroundMovementEngine *groundMovementEngine = actor->groundMovementEngine();

    if(groundMovementEngine==nullptr){
        dbgSystem.addMessage("Actore has no ground movement engine!");

    }

    for(GroundMovementData &d : groundMovementEngine->groundMovementDatas()){
        if(d.cfg->name==groundMovementData){
            mGroundMovementData = &d;
            break;
        }
    }

    if(mGroundMovementData==nullptr){
        dbgSystem.addMessage("Ground movement data '" + groundMovementData +" ' not found!");

    }


    dbgSystem.removeLastMessage();
    return true;

}


bool OnPathwayToGroundMovement::isConditionTrue()
{

    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    assert(actor->currentEngine()->type()==BehaviorEngineType::PATHWAY_MOVEMENT);
    PathMovementEngine *engine = static_cast<PathMovementEngine*>(actor->currentEngine());

    bool activeGroundMoveCommand = (mGroundMovementData->moveLeft.active() || mGroundMovementData->moveRight.active());

    if(engine->isPathwayFinished(activeGroundMoveCommand)){
        return true;
    }

    return false;

}

*/

//=================================================================================================


std::string ACOnSignal::type = "onSignal";


ACOnSignal::ACOnSignal(BehaviorAction *_parentEntityAction, const std::string &_value)
{

    mParentBehaviorAction = _parentEntityAction;
    mValue = _value;

}


bool ACOnSignal::initConnections(PlayedScene *scene)
{

    //if(mStyle==Style::ACTIVE){
        dbgSystem.addMessage("Initializing '" + type +"' ...");

    //}else if(mStyle==Style::ACTIVE_START){
    //    dbgSystem.addMessage("Initializing '" + type_activeStart +"' ...");

    //}else if(mStyle==Style::ACTIVE_END){
    //    dbgSystem.addMessage("Initializing '" + type_activeEnd +"' ...");
    //}


    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    //if(ObtainTriggersFromPath(mTriggers, mValue, scene, actor)==false){
    if(actor->sourceEntity()->name()=="wallSwitchB"){
        DummyFunction();
    }

    //ObtainSignalFromPath(mValue, scene, actor, mParentBehaviorAction->parentBehaviorState(), mSignalQuery);

    app->signalParserManager()->parseSignalAccessor(scene, mValue, mSignalQuery, actor, mParentBehaviorAction->parentBehaviorState());


    if(mSignalQuery.isValid()==false){
        return false;
    }

    if(mSignalQuery.mSignal->id() == static_cast<unsigned char>(SignalID::CUSTOM_ENTITY_SIGNAL)){
        mParentBehaviorAction->parentBehaviorState()->customEntitySignals().push_back(mSignalQuery.mSignal);
        mSignalQuery.mOwnedSignal = true;   // custom signals are owned by signal accessors
    }


    //if(mTriggers.size()==1){
    //    mTrigger = mTriggers.front();
    //}

    dbgSystem.removeLastMessage();
    return true;

}


bool ACOnSignal::isConditionTrue()
{


    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();


    //if(mTrigger->activeFromStyle(mTriggerStyle)){
    if(mSignalQuery.active()){
        return true;
    }

    /*
    if(mTriggerStyle==TriggerStyle::ACTIVE_STARTED){

        //for(ButtonState* b : mTriggers){
        //    if(b->activeStarted()){
        //        return true;
        //    }
        //}

        if(mTrigger->activeStarted()){
            return true;
        }

    }else if(mTriggerStyle==TriggerStyle::ACTIVE){
        //for(ButtonState* b : mTriggers){
        //    if(b->active()){
        //        return true;
        //    }
        //}

        if(mTrigger->active()){
            return true;
        }

    }else if(mTriggerStyle==TriggerStyle::ACTIVE_ENDED){
        //for(ButtonState* b : mTriggers){
        //    if(b->activeEnded()){
        //        return true;
        //    }
        //}

        if(mTrigger->activeEnded()){
            return true;
        }
    }
    */

   return false;

}





//------------------------------------------------------------------------------------



std::string ACOnSignals::type = "onSignals";


ACOnSignals::ACOnSignals(BehaviorAction *_parentEntityAction, const std::string &_value)
{

    mParentBehaviorAction = _parentEntityAction;
    mValue = _value;

}



bool ACOnSignals::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing '" + type + "' ...");


    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    if(actor->sourceEntity()->sourceEntityCfg()->name=="fancyPlatformA_light"){
        DummyFunction();
    }

    if(mCompositeQuery.parse(mValue)==false){
        return false;
    }

    if(mParentBehaviorAction->parentBehaviorState()->behaviorStateCfg()->name=="standBy"){
        DummyFunction();
    }


    if(mCompositeQuery.initConnections(_scene, actor, mParentBehaviorAction->parentBehaviorState())==false){
        return false;
    }


    dbgSystem.removeLastMessage();
    return true;

}



bool ACOnSignals::isConditionTrue()
{

    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    if(mParentBehaviorAction->parentBehaviorState()->behaviorStateCfg()->name=="standBy"){
        DummyFunction();
    }

    return mCompositeQuery.isConditionTrue();

}



}
