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
#include "jmUtilities.h"

#include "jpActionsCommon.h"
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




BehaviorState::BehaviorState(BehaviorStateCfg *_behaviorStateCfg, EntityController* _parent ) :
    mBehaviorStateCfg(_behaviorStateCfg),
    mParentObject(_parent)
{
    mType = BaseObjectType::BEHAVIOR_STATE;
}


BehaviorState::~BehaviorState()
{

    if(mStartAction){
        delete mStartAction;
    }
    for(BehaviorAction* ga : mUpdateActions){
        delete ga;
    }
    if(mAnimationInstance){
        delete mAnimationInstance;
    }

}



bool BehaviorState::build()
{

    dbgSystem.addMessage("Building behavior state '" + mBehaviorStateCfg->name +"' ...");


    if(mBehaviorStateCfg->mStartActionCfg.mCommands.empty()==false || mBehaviorStateCfg->mStartActionCfg.mConditions.empty()==false){
        mStartAction = new BehaviorAction(&mBehaviorStateCfg->mStartActionCfg, this);
        if(mStartAction->build()==false){
            delete mStartAction;
            mStartAction = nullptr;
            return false;
        }
    }

    for(BehaviorActionCfg & aCfg : mBehaviorStateCfg->mUpdateActionsCfgs){
        BehaviorAction *ba = new BehaviorAction(&aCfg, this);
        if(ba->build()==false){
            delete ba;
            return false;
        }
        mUpdateActions.push_back(ba);
    }

    mContactsEnabled = mBehaviorStateCfg->contactsEnabled;


    dbgSystem.removeLastMessage();
    return true;
}


bool BehaviorState::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing behavior state '" + mBehaviorStateCfg->name +"' ...");


    if(mBehaviorStateCfg->name=="on"){
        DummyFunction();
    }


    //----
    if(mBehaviorStateCfg->animation.empty()==false){
        mAnimationInstance = ObtainAnimationInstance(mParentObject->parentActor()->sprite(), mBehaviorStateCfg->animation);
        if(mAnimationInstance==nullptr){
            return false;
        }

    }else{
        // check out for possible animation with the same name
        if(mBehaviorStateCfg->movementEngine.empty()){
            mAnimationInstance = ObtainAnimationInstance(mParentObject->parentActor()->sprite(), mBehaviorStateCfg->name, false);
        }
    }

    //----
    if(mBehaviorStateCfg->movementEngine.empty()==false){
        mMovementEngineData = ObtainMovementEngineData(mParentObject->parentActor(), mBehaviorStateCfg->movementEngine);
        if(mMovementEngineData==nullptr){
            return false;
        }
    }

    //----
    if(mBehaviorStateCfg->taskEngine.empty()==false){
        mTaskEngineData = ObtainTaskEngineData(mParentObject->parentActor(), mBehaviorStateCfg->taskEngine);
        if(mTaskEngineData==nullptr){
            return false;
        }
    }



    if(mStartAction){
        if(mStartAction->initConnections(_scene)==false){
            return false;
        }
    }

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

    if(mStartAction){
        mStartAction->update(_updateMode);
    }

    mStateTrigger._Set(true);

    if(mAnimationInstance){
        mAnimationTrigger._Set(true);
    }

    if(mMovementEngineData){
        if(mParentObject->parentActor()->sourceEntity()->name()=="hero"){
            DummyFunction();
        }

        //mParentObject->parentActor()->startMovementEngine(mMovementEngineData);
    }

    if(mParentObject->parentActor()->body()){
        mParentObject->parentActor()->body()->B2Body()->SetEnabled(mContactsEnabled);
    }

    //----
    if(mCustomEntitySignals.empty()==false){
        Signal::resetSignals(mCustomEntitySignals);
    }


}


void BehaviorState::finish(UpdateMode _updateMode)
{

    mStateTrigger._Set(false);
    mAnimationTrigger._Set(false);

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

        if(s->type()==ValueStateType::BOOL){
            CustomEntityBoolSignal* ces = static_cast<CustomEntityBoolSignal*>(s);
            ces->update();

        }else if(s->type()==ValueStateType::INT){
            CustomEntityIntSignal* ces = static_cast<CustomEntityIntSignal*>(s);
            ces->update();

        }if(s->type()==ValueStateType::INT_BITS){
            CustomEntityIntBitsSignal* ces = static_cast<CustomEntityIntBitsSignal*>(s);
            ces->update();
        }

    }

}


void BehaviorState::postUpdate()
{

    mStateTrigger.postUpdate();
    mAnimationTrigger.postUpdate();

    if(mCustomEntitySignals.empty()==false){
        Signal::postUpdateSignals(mCustomEntitySignals);
    }

}





//==============================================================================================



BehaviorAction::BehaviorAction(BehaviorActionCfg *_behaviorActionCfg, BehaviorState* _parentObject) :
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

        ActionCondition *ac = actionFactory->createActionConditionForEntityAction(cCfg.name, this, cCfg.value);
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



//==============================================================================================



EntityController::EntityController(EntityControllerCfg *_enginesControllerCfg, Entity *_parentActor) :
    mEnginesControllerCfg(_enginesControllerCfg),
    mParentActor(_parentActor)
{

    mType = BaseObjectType::BEH_ENGINES_CONTROLLER;

}

/*
EntityController::EntityController(EntityControllerCfg* _taskHandlerCfg, Entity *_parentActor) :
    mTaskHandlerCfg(_taskHandlerCfg),
    mParentActor(_parentActor)
{

    mType = BaseObjectType::BEH_ENGINES_CONTROLLER;

}
*/


EntityController::~EntityController()
{
    for(BehaviorState* bs : mStates){
        delete  bs;
    }

}


bool EntityController::build()
{

    std::vector<BehaviorStateCfg> *statesCfgs = nullptr;

    if(mEnginesControllerCfg){
        statesCfgs = &mEnginesControllerCfg->statesCfgs;

    //}else if(mTaskHandlerCfg){
    //    statesCfgs = &mTaskHandlerCfg->mStatesCfgs;

    }else{
        assert(false);
    }


    for(BehaviorStateCfg & s : *statesCfgs){
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

    //if(mCurrentState){
    //    mCurrentState->preUpdate();
    //}
    if(mCurrentState->customEntitySignals().empty()==false){
        Signal::preUpdateSignals(mCurrentState->customEntitySignals());
    }

    //for(BehaviorState* bs : mStates){
    //    bs->preUpdate();
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


/*

void BehEnginesController::setCurrentState(BehaviorState *_state)
{

    if(_state==mCurrentState) return;

    mCurrentState = _state;
    mCurrentState->start(UpdateMode::NORMAL);

    //mStateStarted = true;

}
*/


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



BehaviorState * EntityController::state(const std::string &_stateName,  bool _setErrorMessage)
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



}
