#include "pugixml/pugixml.hpp"
#include "jmSystem.h"

//#include "jmGuiBar.h"
#include "jmAnimationCommon.h"
#include "map/jmSpriteLayer.h"
#include "map/jmMap.h"
#include "jmGlobal.h"
#include "jmScene.h"
#include "jmSceneLayout.h"
#include "jmUtilities.h"

#include "gui/jpGuiCommon.h"
#include "gui/jpGuiSystem.h"

#include "jpQueries.h"
#include "jpVariables.h"
#include "jpUtilities.h"
#include "jpCompound.h"
#include "jpComponentsCommon.h"
#include "jpPlayedScene.h"
#include "jpObjectFactory.h"
#include "jpLogicStateCfg.h"
#include "jpLogicState.h"



namespace jugimap{




LogicState::LogicState(LogicStateCfg *_cfg, BaseObject* _parent)
{

    mCfg = _cfg;
    mParentObject = _parent;
    mName = _cfg->name();

    mBaseType = BaseObjectType::LOGIC_STATE;

    mCustomState = _cfg->createCustomState(this);

}



LogicState::~LogicState()
{

    for(Action* ga : mActions){
        delete ga;
    }

    for(LogicState *gs : mChildStates){
        delete  gs;
    }

    delete mCustomState;

}



bool LogicState::build(LogicStateCfg* _logicStateCfg)
{

    for(ActionCfg &cfg : _logicStateCfg->actionsCfgs()){
        Action *a = new Action(this);
        if(a->build(cfg)==false){
            return false;
        }
        mActions.push_back(a);
    }

    if(mCustomState->build()==false){
        return false;
    }

    for(LogicStateCfg* cfg : _logicStateCfg->childStatesCfgs()){
        LogicState* childState = new LogicState(cfg, this);
        if(childState->build(cfg)==false){
            return false;
        }
        mChildStates.push_back(childState);
    }

    return true;

}



bool LogicState::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing logic state '" + mName + "' ...");


    mParentPlayerScene = _scene;

    //---
    for(Action* ga : mActions){
        if(ga->initConnections(_scene)==false){
            return false;
        }
    }

    if(mCustomState->initConnections(_scene)==false){
        return false;
    }


    //---
    for(LogicState *gs : mChildStates){
        if(gs->initConnections(_scene)==false){
            return false;
        }
    }

    dbgSystem.removeLastMessage();
    return true;
}



bool LogicState::startingPhaseUpdate()
{

    if(mNextChildState){

        if(mActiveChildState){
            mActiveChildState->preUpdate_finishState();

            mNextChildState->customState()->initTransitionFrom(mActiveChildState->customState());
        }else{
            mNextChildState->customState()->initTransitionFrom(nullptr);
        }

        mPreviousChildState = mActiveChildState;
        mActiveChildState = mNextChildState;

        mNextChildState = nullptr;

    }


    bool startingPhase = false;

    if(mCustomState->startingPhaseUpdate()==true){
        startingPhase = true;
    }

    if(mActiveChildState){
        if(mActiveChildState->startingPhaseUpdate()==true){
            startingPhase = true;
        }
    }

    mSigState.setValue(true);

    return startingPhase;

}



// PRE UPDATE

void LogicState::preUpdate(UpdateMode &_updateMode)
{

    if(mNextChildState){

        //---
        if(mActiveChildState){
            mActiveChildState->preUpdate_finishState();
            mNextChildState->customState()->initTransitionFrom(mActiveChildState->customState());
        }else{
            mNextChildState->customState()->initTransitionFrom(nullptr);
        }

        mPreviousChildState = mActiveChildState;
        mActiveChildState = mNextChildState;
        mNextChildState = nullptr;

        mActiveChildState->preUpdate_startState();
    }


    mCustomState->preUpdate(_updateMode);


    if(mActiveChildState){
        mActiveChildState->preUpdate(_updateMode);
    }

}




void LogicState::preUpdate_startState()
{


    mCustomState->start();

    if(mCustomUpdatedSignals.empty()==false){
        Signal::resetSignals(mCustomUpdatedSignals);
    }

    if(mActiveChildState){
        mActiveChildState->preUpdate_startState();
    }

    mSigState.setValue(true);

}


void LogicState::preUpdate_finishState()
{

    mCustomState->finish();

    if(mCustomUpdatedSignals.empty()==false){
        Signal::resetSignals(mCustomUpdatedSignals);
    }

    //---
    if(mActiveChildState){
        mActiveChildState->preUpdate_finishState();
    }

    mSigState.setValue(false);

}


void LogicState::update(UpdateMode &_updateMode)
{

    for(Action* ga : mActions){
        if(ga->update(_updateMode)==true){
            if(_updateMode.loopAllStateActions==false){
                break;
            }
        }
    }

    mCustomState->update(_updateMode);

    //---
    for(Signal *s : mCustomUpdatedSignals){
        if(s->type()==SignalType::BOOL){
            UpdatedBoolSignal* ces = static_cast<UpdatedBoolSignal*>(s);
            ces->update();
        }
    }

    //---
    if(mActiveChildState){
        mActiveChildState->update(_updateMode);
    }

}


void LogicState::postUpdate(UpdateMode &_updateMode)
{

    mCustomState->postUpdate(_updateMode);

    if(mActiveChildState){
        mActiveChildState->postUpdate(_updateMode);
    }

    mSigState.postUpdate();

}


void LogicState::drawShapes(jugimap::ShapeDrawer &_drawer, int _flags)
{

    mCustomState->drawShapes(_drawer, _flags);

    if(mActiveChildState){
        mActiveChildState->drawShapes(_drawer, _flags);
    }

}


void LogicState::setNextChildState(LogicState *_gameState)
{

    if(mActiveChildState==_gameState){
        return;
    }

    //---
    //if(StdVector::contains(mChildStates, _gameState)==false){       // debug check
    //    assert(false);
    //}

    mNextChildState = _gameState;

}


LogicState * LogicState::findChildState(const std::string &_stateName,  bool _setErrorMessage)
{

    for(LogicState* ls : mChildStates){
        if(ls->name()==_stateName){
            return ls;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A state with name '" + _stateName + "' not found!");
    }

    return nullptr;
}


BaseObject* LogicState::rootParentObject()
{

    BaseObject* parent = mParentObject;

    while(parent){

        if(parent->baseType()==BaseObjectType::LOGIC_STATE){
            LogicState *state = static_cast<LogicState *>(parent);
            parent = state->parentObject();

        }else{
            return parent;
        }

    }

    assert(false);
    return nullptr;

}


LogicState* LogicState::rootLogicState()
{

    BaseObject* object = this;

    while(true){

        LogicState *state = static_cast<LogicState *>(object);

        assert(state->parentObject());

        if(state->parentObject()->baseType()!=BaseObjectType::LOGIC_STATE){
            return state;

        }else{
            object = state->parentObject();

        }
    }

    assert(false);
    return nullptr;
}


LogicState* LogicState::findChildStateViaPath(const std::string &_path, bool _setErrorMessage)
{

    std::vector<std::string> statePathParts = StdString::splitString(_path, ":");

    return findChildStateViaPath(statePathParts, _setErrorMessage);

}


LogicState* LogicState::findChildStateViaPath(std::vector<std::string>& statePathParts, bool _setErrorMessage)
{

    LogicState *state = this;

    bool errorInPath = true;
    int i=0;

    while(i<statePathParts.size()){

        const std::string &stateName = statePathParts.at(i);
        errorInPath = false;
        bool stateFound = false;

        for(LogicState* s : state->childStates()){
            if(s->name() == stateName){
                state = s;
                stateFound = true;
                break;
            }
        }

        if(stateFound==false){
            errorInPath = true;
            break;
        }

        i++;
    }

    if(errorInPath){
        //if(_setErrorMessage){
        //    dbgSystem.addMessage("State with path '" + _path + "' not found!");
        //}
        return nullptr;
    }

    return state;

}



//==================================================================================================



Action::Action(BaseObject* _parentObject) : mParentObject(_parentObject)
{
    mBaseType = BaseObjectType::ACTION;
}


Action::~Action()
{

    for(ActionCondition *ac : mConditions){
        delete ac;
    }

    for(ActionCommand *ac : mCommands){
        delete ac;
    }

}


bool Action::build(ActionCfg &_actionCfg)
{

    for(ActionConditionCfg &cfg : _actionCfg.mConditions){

        ActionCondition* condition = actionFactory->createActionCondition(cfg.name, this, cfg.value);
        if(condition==false){
            dbgSystem.addMessage("Action condition '" + cfg.name +"' not found!");
            return false;
        }
        mConditions.push_back(condition);
    }

    for(ActionCommandCfg &cfg : _actionCfg.mCommands){

        ActionCommand *command = actionFactory->createActionCommands(cfg.name, this, cfg.value);
        if(command==false){
            dbgSystem.addMessage("Action command '" + cfg.name +"' not found!");
            return false;
        }
        mCommands.push_back(command);
    }

    mDisabled = _actionCfg.mDisabled;
    mDbgId = _actionCfg.mDbgId;


    return true;
}


bool Action::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing action ...");

    mParentPlayerScene = _scene;


    for(ActionCondition* ac : mConditions){
        if(ac->initConnections(_scene)==false){
            return false;
        }
    }

    for(ActionCommand* ac : mCommands){
        if(ac->initConnections(_scene)==false){
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;
}




bool Action::update(UpdateMode &_updateMode)
{

    if(_updateMode.modalOverlay){
        return false;
    }

    if(mDisabled){
        return false;
    }

    for(ActionCondition* ac : mConditions){
        if(ac->isTrue()==false){
            return false;
        }
    }

    for(ActionCommand* ac : mCommands){
        ac->run(_updateMode);
    }


    return true;
}


//-----------------------------------------------------------------------------------------


OverlayState::OverlayState(Compound *_compound)
{
    mBaseType = BaseObjectType::OVERLAY_STATE;

    mCompound = _compound;
    mCompoundTransition.reset(new CompoundTransition());

}


}
