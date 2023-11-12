#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmMap.h"
#include "jmCamera.h"
#include "jmSceneLayout.h"
#include "jmCommonFunctions.h"
#include "jmSpriteLayer.h"
#include "jmUtilities.h"

//#include "jpActionsCommon.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"

#include "jpB2Body.h"
#include "jpB2Utilities.h"
#include "jpEntity.h"
#include "jpEntityUtilities.h"
#include "jpSourceEntity.h"
#include "movements/jpMovementBase.h"
#include "jpObjectFactory.h"
#include "jpBehCfg.h"
#include "jpBehavior.h"



namespace jugimap{



using namespace jugimap;



/*

BehaviorState::BehaviorState(EntityBehaviorStateCfg *_behaviorStateCfg, EntityController* _parent ) :
    mBehaviorStateCfg(_behaviorStateCfg),
    mParentObject(_parent)
{
    mBaseType = BaseObjectType::BEHAVIOR_STATE;
}


BehaviorState::~BehaviorState()
{

    //if(mStartAction){
    //    delete mStartAction;
    //}
    for(BehaviorAction* ga : mUpdateActions){
        delete ga;
    }
    //if(mAnimationInstance){
    //    delete mAnimationInstance;
    //}

    if(mStateData){
        delete mStateData;
    }

}



bool BehaviorState::build()
{

    dbgSystem.addMessage("Building behavior state '" + mBehaviorStateCfg->name +"' ...");



    for(ActionCfg & aCfg : mBehaviorStateCfg->mUpdateActionsCfgs){
        BehaviorAction *ba = new BehaviorAction(&aCfg, this);
        if(ba->build()==false){
            delete ba;
            return false;
        }
        mUpdateActions.push_back(ba);
    }

    //mContactsEnabled = mBehaviorStateCfg->contactsEnabled;
    mStateData = new EntitySystemStateData(mBehaviorStateCfg, this);


    dbgSystem.removeLastMessage();
    return true;
}


bool BehaviorState::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing behavior state '" + mBehaviorStateCfg->name +"' ...");


    if(mBehaviorStateCfg->name=="on"){
        DummyFunction();
    }


    if(mStateData){
        if(mStateData->initConnections(_scene)==false){
            return false;
        }
    }


    //if(mStartAction){
    //    if(mStartAction->initConnections(_scene)==false){
    //        return false;
    //    }
    //}

    //----
    for(BehaviorAction* a : mUpdateActions){
        if(a->initConnections(_scene)==false){
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}


void BehaviorState::start(UpdateMode _updateMode)
{

    //if(mStartAction){
    //    mStartAction->update(_updateMode);
    //}

    mStateTrigger.setValue(true);


    if(mStateData){
        mStateData->start();
    }


    //----
    if(mCustomEntitySignals.empty()==false){
        Signal::resetSignals(mCustomEntitySignals);
    }


}


void BehaviorState::finish(UpdateMode _updateMode)
{

    mStateTrigger.setValue(false);
    //mAnimationTrigger.setValue(false);

    if(mStateData){
        mStateData->finish();
    }


    //----
    if(mCustomEntitySignals.empty()==false){
        Signal::resetSignals(mCustomEntitySignals);
    }

}


void BehaviorState::preUpdate()
{


}


void BehaviorState::update(UpdateMode _updateMode)
{

    if(_updateMode==UpdateMode::MODAL_OVERLAY && mUpdateInModalOverlayMode==false){
        return;
    }

    for(BehaviorAction* ea : mUpdateActions){
        if(ea->update(_updateMode)==true){
            return;
        }
    }

    //---
    for(Signal *s : mCustomEntitySignals){

        if(s->type()==SignalType::BOOL){
            CustomEntityBoolSignal* ces = static_cast<CustomEntityBoolSignal*>(s);
            ces->update();

        }else if(s->type()==SignalType::INT){
            CustomEntityIntSignal* ces = static_cast<CustomEntityIntSignal*>(s);
            ces->update();

        }if(s->type()==SignalType::BITSET){
            CustomEntityIntBitsSignal* ces = static_cast<CustomEntityIntBitsSignal*>(s);
            ces->update();
        }

    }

}


void BehaviorState::postUpdate()
{

    //mStateTrigger.postUpdate();
    //mAnimationTrigger.postUpdate();

    //if(mCustomEntitySignals.empty()==false){
    //    Signal::postUpdateSignals(mCustomEntitySignals);
    //}

}


*/




bool EntityLogicStateCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading state '" + mName + "' ...");

    //---- attributes

    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
           mName = a.as_string("");

        }else if(attributeName=="contactsEnabled"){
            contactsEnabled = a.as_bool(true);

        }else if(attributeName=="animation"){
            animation = a.as_string("");

        }else if(attributeName=="movementEngine"){
            movementEngine = a.as_string("");

        }else if(attributeName=="taskEngine"){
            taskEngine = a.as_string("");

        }else{

            dbgSystem.addMessage("Wrong attribute '" + attributeName + "' !");
            return false;
        }
    }


    //---- subnodes  (actions and child states)
    if(LogicStateCfg::initCfg(_node)==false){
        return false;
    }


    dbgSystem.removeLastMessage();
    return true;


    /*

    //---- sub-nodes

    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        ActionCfg * behaviorActionCfg = nullptr;

        //if(nodeName=="aStart"){
        //    behaviorActionCfg = & mStartActionCfg;
        //
        //}else
        if(nodeName=="a"){
            mUpdateActionsCfgs.push_back(ActionCfg());
            behaviorActionCfg = & mUpdateActionsCfgs.back();

        }else{
            dbgSystem.addMessage("Unknown node '"+ nodeName + "'!");
            return false;
        }

        for(pugi::xml_attribute a = n.first_attribute(); a; a=a.next_attribute()){
            std::string aName = std::string(a.name());
            std::string aValue = std::string(a.as_string());

            const std::string prefix = aName.substr(0,2);

            if(prefix=="on"){
                behaviorActionCfg->mConditions.push_back(ActionConditionCfg(aName, aValue));

            }else if(prefix=="do"){
                behaviorActionCfg->mCommands.push_back(ActionCommandCfg(aName, aValue));

            }
        }
    }

    dbgSystem.removeLastMessage();

    return true;
    */
}



LogicStateCfg* EntityLogicStateCfg::createLogicStateCfg(const std::string &_name)
{
    return new EntityLogicStateCfg(_name);
}


CustomLogicState* EntityLogicStateCfg::createCustomState(LogicState *_parentState)
{

    return new EntitySystemStateData(this, _parentState);

}

//==============================================================================================


EntitySystemStateData::EntitySystemStateData(EntityLogicStateCfg *_behaviorStateCfg, LogicState* _parent) :
    mParentState(_parent),
    mBehaviorStateCfg(_behaviorStateCfg)
{

}


EntitySystemStateData::~EntitySystemStateData()
{
    if(mAnimationInstance){
        delete mAnimationInstance;
    }
}



bool EntitySystemStateData::initConnections(PlayedScene *_scene)
{

    //Entity *parentEntity = mParentState->parentStateManager()->parentActor();

    Entity *parentEntity = static_cast<Entity*>(mParentState->rootParentObject());

    if(mBehaviorStateCfg->animation.empty()==false){
        mAnimationInstance = ObtainAnimationInstance(parentEntity->sprite(), mBehaviorStateCfg->animation);
        if(mAnimationInstance==nullptr){
            return false;
        }

    }else{
        // check out for possible animation with the same name
        if(mBehaviorStateCfg->movementEngine.empty()){
            mAnimationInstance = ObtainAnimationInstance(parentEntity->sprite(), mParentState->name(), false);
        }
    }

    //----
    if(mBehaviorStateCfg->movementEngine.empty()==false){
        mMovementEngineData = ObtainMovementEngineData(parentEntity, mBehaviorStateCfg->movementEngine);
        if(mMovementEngineData==nullptr){
            return false;
        }
    }

    //----
    if(mBehaviorStateCfg->taskEngine.empty()==false){
        mTaskEngineData = ObtainTaskEngineData(parentEntity, mBehaviorStateCfg->taskEngine);
        if(mTaskEngineData==nullptr){
            return false;
        }
    }


    mContactsEnabled = mBehaviorStateCfg->contactsEnabled;


    return true;

}


void EntitySystemStateData::start()
{

    //Entity *parentEntity = mParentState->parentStateManager()->parentActor();
    Entity *parentEntity = static_cast<Entity*>(mParentState->rootParentObject());

    if(mAnimationInstance){
        mAnimationTrigger.setValue(true);
    }

    if(mMovementEngineData){
        if(parentEntity->sourceEntity()->name()=="hero"){
            DummyFunction();
        }

        //mParentObject->parentActor()->startMovementEngine(mMovementEngineData);
    }

    if(parentEntity->body()){
        parentEntity->body()->B2Body()->SetEnabled(mContactsEnabled);
    }

}


void EntitySystemStateData::finish()
{
    mAnimationTrigger.setValue(false);
}



//==============================================================================================

/*

BehaviorAction::BehaviorAction(ActionCfg *_behaviorActionCfg, BehaviorState* _parentObject) :
    mBehaviorActionCfg(_behaviorActionCfg),
    mParentBehaviorState(_parentObject)
{
    //mType = BaseObjectType::ACTION;
}


BehaviorAction::~BehaviorAction()
{

    for(ActionCommand *ac : mCommands){
        delete ac;
    }

    for(ActionCondition *ac : mConditions){
        delete ac;
    }

}


bool BehaviorAction::build()
{

    for(ActionConditionCfg &cCfg : mBehaviorActionCfg->mConditions){

        //ActionCondition *ac = actionFactory->createActionConditionForEntityAction(cCfg.name, this, cCfg.value);
        ActionCondition *ac = actionFactory->createActionCondition(cCfg.name, nullptr, cCfg.value);
        if(ac==nullptr){
            dbgSystem.addMessage("Unknown behavior condition '"+ cCfg.name + "'!");
            return false;
        }

        mConditions.push_back(ac);

    }

    for(ActionCommandCfg &cCfg : mBehaviorActionCfg->mCommands){

        ActionCommand *ac = actionFactory->createActionCommandsForEntityAction(cCfg.name, this, cCfg.value);
        if(ac==nullptr){
            dbgSystem.addMessage("Unknown behavior command '"+ cCfg.name + "'!");
            return false;
        }
        mCommands.push_back(ac);

    }

    return true;
}


bool BehaviorAction::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initialize conditions and commands ...");

    //mParentPlayerScene = _scene;

    for(ActionCommand *ac : mCommands){
        if(ac->initConnections(_scene)==false){
            return false;
        }
    }

    for(ActionCondition *ac : mConditions){
        ac->setParentState(mParentBehaviorState);
        ac->setThisSignalOrigin(mParentBehaviorState->parentStateManager()->parentActor());
        if(ac->initConnections(_scene)==false){
            return false;
        }
    }


    dbgSystem.removeLastMessage();

    return true;
}



bool BehaviorAction::update(UpdateMode _updateMode)
{

    if(mStatus==ActionStatus::DISABLED){
        return false;
    }


    for(ActionCondition* ac : mConditions){
        if(ac->isConditionTrue()==false){
            return false;
        }
    }

    for(ActionCommand* ac : mCommands){
        ac->update(_updateMode);
    }

    return true;

}

*/


//==============================================================================================

/*

EntityController::EntityController(EntityControllerCfg *_enginesControllerCfg, Entity *_parentActor) :
    mEnginesControllerCfg(_enginesControllerCfg),
    mParentActor(_parentActor)
{

    mBaseType = BaseObjectType::BEH_ENGINES_CONTROLLER;

}



EntityController::~EntityController()
{
    for(BehaviorState* bs : mStates){
        delete  bs;
    }

}


bool EntityController::build()
{

    std::vector<EntityBehaviorStateCfg> *statesCfgs = nullptr;

    if(mEnginesControllerCfg){
        statesCfgs = &mEnginesControllerCfg->statesCfgs;

    //}else if(mTaskHandlerCfg){
    //    statesCfgs = &mTaskHandlerCfg->mStatesCfgs;

    }else{
        assert(false);
    }


    for(EntityBehaviorStateCfg & s : *statesCfgs){
        BehaviorState * bs = new BehaviorState(&s, this);
        if(bs->build()==false){
            return false;
        }
        mStates.push_back(bs);
    }


    return true;
}



bool EntityController::initConnections(PlayedScene *_scene)
{

    //----
    for(BehaviorState* bs : mStates){
        if(bs->initConnections(_scene)==false){
            return false;
        }
    }

    return true;

}


void EntityController::start()
{


}

void EntityController::preUpdate(UpdateMode _updateMode)
{

    if(mNextState){

        if(mCurrentState){
            mCurrentState->finish(_updateMode);
        }
        mPreviousState = mCurrentState;

        mCurrentState = mNextState;
        mCurrentState->start(_updateMode);
        mNextState = nullptr;
    }


    //if(mCurrentState->customEntitySignals().empty()==false){
    //    Signal::preUpdateSignals(mCurrentState->customEntitySignals());
    //}


}


void EntityController::update(UpdateMode _updateMode)
{

    if(mCurrentState == nullptr){
        return;
    }

    //if(mStateStarted){
    //    mCurrentState->start(_updateMode);
    //    mStateStarted = false;
    //}

    //if(mParentActor->sourceEntity()->name()=="fancyPlatformA_light"){
    //    print("Current state = '" + mCurrentState->behaviorStateCfg()->name);
    //}

    mCurrentState->update(_updateMode);



}





void EntityController::setCurrentState(BehaviorState *_state)
{

    if(_state==mCurrentState) return;

    mNextState = _state;

    if(mParentActor->sourceEntity()->name()=="hero"){
        if(mNextState->behaviorStateCfg()->name != "groundMovement"){
            DummyFunction();
        }
    }
    //mCurrentState = _state;
    //mCurrentState->start(UpdateMode::NORMAL);


}






void EntityController::postUpdate(UpdateMode _updateMode)
{

    //if(mCurrentState){
    //    mCurrentState->switchState().ResetPerUpdateFlags();
    //}

    if(mCurrentState){
        mCurrentState->postUpdate();
    }
    if(mPreviousState){
        mPreviousState->postUpdate();
        mPreviousState = nullptr;
    }

    //for(BehaviorState* bs : mStates){
    //    bs->postUpdate();
    //}

}



BehaviorState * EntityController::findState(const std::string &_stateName,  bool _setErrorMessage)
{

    for(BehaviorState* bs : mStates){
        if(bs->behaviorStateCfg()->name==_stateName){
            return bs;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A state with name '" + _stateName + "' not found!");
    }

    return nullptr;
}

*/


}
