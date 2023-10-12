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

#include "jpSourceEntity.h"
#include "jpEntity.h"
#include "jpEntityUtilities.h"
#include "jpBehavior.h"
#include "jpBehCommands.h"



namespace jugimap{



std::string DoPlayAnimation::type = "doPlayAnimation";


DoPlayAnimation::DoPlayAnimation(BehaviorAction *_parentEntityAction, const std::string &_value)
{


    mParentBehaviorAction = _parentEntityAction;
    mValue = _value;

}


bool DoPlayAnimation::initConnections(PlayedScene *scene)
{

    dbgSystem.addMessage("Initializing '" + type + "' ...");


    dbgSystem.removeLastMessage();
    return true;

}


void DoPlayAnimation::update(UpdateMode _updateMode)
{

}



//========================================================================================

/*
std::string DoSetGroundStanding::type = "doSetGroundStanding";


DoSetGroundStanding::DoSetGroundStanding(BehaviorAction *_parentBehaviorAction, const std::string &_value)
{

    mParentBehaviorAction = _parentBehaviorAction;
    mValue = _value;
}


bool DoSetGroundStanding::initConnections(PlayedScene *scene)
{
    dbgSystem.addMessage("init '" + type + "'");


    const std::string &groundStandingName = mValue;


    MovementSpeedConfigurations *movementConfigurations = scene->entitySystem()->movementSpeedConfigurations();
    mGroundStandingCfg = movementConfigurations->getGroundStandingCfg(groundStandingName);
    if(mGroundStandingCfg==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}


void DoSetGroundStanding::update(UpdateMode _updateMode)
{

    mParentBehaviorAction->parentBehaviorState()->parentBehavior()->doSetGroundStanding(*mGroundStandingCfg);

}
*/

//========================================================================================

/*

std::string DoGroundMovement::type = "doGroundMovement";


DoGroundMovement::DoGroundMovement(BehaviorAction *_parentBehaviorAction, const std::string &_value)
{

    mParentBehaviorAction = _parentBehaviorAction;
    mValue = _value;
}


bool DoGroundMovement::initConnections(PlayedScene *scene)
{
    dbgSystem.addMessage("Initializing '" + type + "' ...");


    std::vector<std::string>parts = StdString::splitString(mValue, ",");

    //if(parts.size()<3){
    if(parts.size() != 1){
        dbgSystem.addMessage("Wrong value '" + mValue + "!");
        return false;
    }

    //----
    const std::string &movementName = parts[0];

    Entity *entity = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    mGroundMovementData = entity->groundMovementEngine()->getGroundMovementData(movementName);
    if(mGroundMovementData==nullptr){
        return false;
    }



    dbgSystem.removeLastMessage();
    return true;

}


void DoGroundMovement::update(UpdateMode _updateMode)
{

    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();
    actor->doGroundMovement(mGroundMovementData);

}


//========================================================================================


std::string DoJumpMovement::type = "doJumpMovement";


DoJumpMovement::DoJumpMovement(BehaviorAction *_parentBehaviorAction, const std::string &_value)
{

    mParentBehaviorAction = _parentBehaviorAction;
    mValue = _value;
}


bool DoJumpMovement::initConnections(PlayedScene *scene)
{
    dbgSystem.addMessage("Initializing '" + type + "' ...");


    std::vector<std::string>parts = StdString::splitString(mValue, ",");

    //if(parts.size()<3){
    if(parts.size() != 1){
        dbgSystem.addMessage("Wrong value '" + mValue + "!");
        return false;
    }

    //----
    const std::string &movementName = parts[0];

    Entity *entity = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    mJumpMovementData = entity->jumpMovementEngine()->getJumpMovementData(movementName);
    if(mJumpMovementData==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}


void DoJumpMovement::update(UpdateMode _updateMode)
{

    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();
    actor->doJumpMovement(mJumpMovementData);

}


//========================================================================================


std::string DoPointToPointPathMovement::type = "doPointToPointPathMovement";


DoPointToPointPathMovement::DoPointToPointPathMovement(BehaviorAction *_parentBehaviorAction, const std::string &_value)
{

    mParentBehaviorAction = _parentBehaviorAction;
    mValue = _value;
}


bool DoPointToPointPathMovement::initConnections(PlayedScene *scene)
{
    dbgSystem.addMessage("Initializing '" + type + "' ...");


    std::vector<std::string>parts = StdString::splitString(mValue, ",");

    //if(parts.size()<3){
    if(parts.size() != 1){
        dbgSystem.addMessage("Wrong value '" + mValue + "!");
        return false;
    }

    //----
    const std::string &movementName = parts[0];

    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    mPathMovementData = actor->pathPointToPointMovementEngine()->getPathMovementData(movementName);
    if(mPathMovementData==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}


void DoPointToPointPathMovement::update(UpdateMode _updateMode)
{

    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();
    actor-> doPointToPointPathMovement(mPathMovementData);

}



//========================================================================================


std::string DoPathMovement::type = "doPathMovement";


DoPathMovement::DoPathMovement(BehaviorAction *_parentBehaviorAction, const std::string &_value)
{

    mParentBehaviorAction = _parentBehaviorAction;
    mValue = _value;
}


bool DoPathMovement::initConnections(PlayedScene *scene)
{
    dbgSystem.addMessage("Initializing '" + type + "' ...");


    std::vector<std::string>parts = StdString::splitString(mValue, ",");

    //if(parts.size()<3){
    if(parts.size() != 1){
        dbgSystem.addMessage("Wrong value '" + mValue + "!");
        return false;
    }

    //----
    const std::string &movementName = parts[0];

    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    mPathMovementData = actor->pathMovementEngine()->getPathMovementData(movementName);
    if(mPathMovementData==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}


void DoPathMovement::update(UpdateMode _updateMode)
{

    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();
    actor-> doPathwayMovement(mPathMovementData);

}




//========================================================================================


std::string DoAnimatedBehavior::type = "doAnimatedBehavior";


DoAnimatedBehavior::DoAnimatedBehavior(BehaviorAction *_parentBehaviorAction, const std::string &_value)
{

    mParentBehaviorAction = _parentBehaviorAction;
    mValue = _value;
}


bool DoAnimatedBehavior::initConnections(PlayedScene *scene)
{
    dbgSystem.addMessage("Initializing '" + type + "' ...");


    std::vector<std::string>parts = StdString::splitString(mValue, ",");

    //if(parts.size()<3){
    if(parts.size() != 1){
        dbgSystem.addMessage("Wrong value '" + mValue + "!");
        return false;
    }

    //----
    const std::string &movementName = parts[0];

    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    mAnimatedBehaviorData = actor->animatedBehaviorEngine()->getAnimatedBehaviorData(movementName);
    if(mAnimatedBehaviorData==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}


void DoAnimatedBehavior::update(UpdateMode _updateMode)
{

    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();
    actor->doAnimatedBehavior(mAnimatedBehaviorData);

}

*/

//========================================================================================

/*
std::string DoPointToPointMovementTask::type = "doPointToPointMovementTask";


DoPointToPointMovementTask::DoPointToPointMovementTask(BehaviorAction *_parentBehaviorAction, const std::string &_value)
{

    mParentBehaviorAction = _parentBehaviorAction;
    mValue = _value;
}


bool DoPointToPointMovementTask::initConnections(PlayedScene *scene)
{
    dbgSystem.addMessage("init '" + type + "'");


    std::vector<std::string>parts = StdString::splitString(mValue, ",");

    //if(parts.size()<3){
    if(parts.size() != 1){
        dbgSystem.addMessage("Wrong value '" + mValue + "!");
        return false;
    }

    //----
    const std::string &taskName = parts[0];

    //Behavior *behavior = mParentBehaviorAction->parentBehaviorState()->parentStateManager();
    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();
    //AiController * aiController = actor->aiController();

    //mData = aiController->pointToPointMovementTask()->getDataObject(taskName);
    mData = actor->pointToPointMovemenTask()->getDataObject(taskName);
    if(mData==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}


void DoPointToPointMovementTask::update(UpdateMode _updateMode)
{

    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();
    //AiController * aiController = actor->aiController();
    //aiController->doUsePointToPointMovementTask(mData);
    actor->doPointToPointMovementTask(mData);

}

*/

//========================================================================================


std::string DoSetState::type = "doSetState";


DoSetState::DoSetState(BehaviorAction *_parentEntityAction, const std::string &_value)
{

    mParentBehaviorAction = _parentEntityAction;
    mValue = _value;

}


bool DoSetState::initConnections(PlayedScene *scene)
{

    dbgSystem.addMessage("Initializing '" + type + "' ...");

    // mValue = name of the state

    //Behavior *behavior = mParentBehaviorAction->parentBehaviorState()->parentStateManager();
    EntityController *stateManager = mParentBehaviorAction->parentBehaviorState()->parentStateManager();

    mBehaviorState = stateManager->state(mValue);
    if(mBehaviorState==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}


void DoSetState::update(UpdateMode _updateMode)
{

    mParentBehaviorAction->parentBehaviorState()->parentStateManager()->setCurrentState(mBehaviorState);

}


//========================================================================================


std::string DoSetSignal::type = "doSetSignal";


DoSetSignal::DoSetSignal(BehaviorAction *_parentEntityAction, const std::string &_value)
{

    mParentBehaviorAction = _parentEntityAction;
    mValue = _value;

}


bool DoSetSignal::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing '" + type + "' ...");


    Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    if(actor->sourceEntity()->name()=="wallSwitchB"){
        DummyFunction();
    }

    //ObtainSignalFromPath(mValue, scene, actor, mParentBehaviorAction->parentBehaviorState(), mSignalSetter);
    app->signalParserManager()->parseSignalAccessor(_scene, mValue, mSignalSetter, actor, mParentBehaviorAction->parentBehaviorState());

    if(mSignalSetter.isValid()==false){
        return false;
    }

    if(mSignalSetter.mSignal->id() == static_cast<unsigned char>(SignalID::CUSTOM_ENTITY_SIGNAL)){
        mParentBehaviorAction->parentBehaviorState()->customEntitySignals().push_back(mSignalSetter.mSignal);
        mSignalSetter.mOwnedSignal = true;   // custom signals are owned by signal accessors
    }


    dbgSystem.removeLastMessage();
    return true;

}


void DoSetSignal::update(UpdateMode _updateMode)
{


    mSignalSetter.set();

}



}
