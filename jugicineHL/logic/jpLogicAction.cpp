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
#include "jpFunction.h"
#include "jpLogicAction.h"



namespace jugimap{







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

    for(Action *a : mChildActions){
        delete a;
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

    mBreakOnExecuted = _actionCfg.mBreakOnExecuted;
    mDisabled = _actionCfg.mDisabled;
    mDbgId = _actionCfg.mDbgId;


    //----
    for(ActionCfg &cfg : _actionCfg.mChildActions){
        Action *a = new Action(this);
        if(a->build(cfg)==false){
            return false;
        }
        mChildActions.push_back(a);
    }

    //----
    //BaseObject * parentObject = mParentAction->parentObject();
    //LogicState * parentState = nullptr;


    //assert(mParentState);
    //BaseObject * rootParentObject = nullptr;
    //if(parentState){
    //    mRootParentObject = mParentState->rootParentObject();
    //}


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

    //----
    for(Action *a : mChildActions){
        if(a->initConnections(_scene)==false){
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


    for(Action* a : mChildActions){
        if(a->update(_updateMode)==true){
            //if(_updateMode.loopAllStateActions==false){
            if(a->breakOnExecuted()){
                break;
            }
        }
    }

    return true;
}


LogicState* Action::parentLogicState()
{

    BaseObject* parent = mParentObject;

    while(parent){

        if(parent->baseType()==BaseObjectType::LOGIC_STATE){
            return static_cast<LogicState*>(parent);

        }else if(parent->baseType()==BaseObjectType::ACTION){
            Action *action = static_cast<Action *>(parent);
            parent = action->parentObject();

        }else{
            assert(false);
        }

    }

    assert(false);
    return nullptr;

}

/*
ParentObjects Action::parentObjects()
{

    ParentObjects po;

    if(mParentObject->baseType()==BaseObjectType::LOGIC_STATE){
        po.parentLogicState = static_cast<LogicState*>(mParentObject);

    }else if(mParentObject->baseType()==BaseObjectType::ACTION){
         po.parentLogicState = po.parentLogicFunction->parentState();

    }else if(mParentObject->baseType()==BaseObjectType::LOGIC_FUNCTION){
         po.parentLogicFunction = static_cast<LogicFunction*>(mParentObject);
         po.parentLogicState = po.parentLogicFunction->parentState();
    }

    assert(po.parentLogicState);

    po.rootParentObject = po.parentLogicState->rootParentObject();

    return po;
}
*/


//=====================================================================================



ActionTrigger::ActionTrigger(BaseObject* _parentObject) : mParentObject(_parentObject)
{
    mBaseType = BaseObjectType::ACTION;
}


ActionTrigger::~ActionTrigger()
{

    for(ActionCondition *ac : mConditions){
        delete ac;
    }
}


bool ActionTrigger::build(ActionTriggerCfg &_actionCfg)
{

    for(ActionConditionCfg &cfg : _actionCfg.mConditions){

        ActionCondition* condition = actionFactory->createActionTriggerCondition(cfg.name, this, cfg.value);
        if(condition==false){
            dbgSystem.addMessage("Action condition '" + cfg.name +"' not found!");
            return false;
        }
        mConditions.push_back(condition);
    }

    return true;
}


bool ActionTrigger::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing action ...");

    mParentPlayerScene = _scene;


    for(ActionCondition* ac : mConditions){
        if(ac->initConnections(_scene)==false){
            return false;
        }
    }

    dbgSystem.removeLastMessage();
    return true;
}


bool ActionTrigger::isTriggered()
{


    for(ActionCondition* ac : mConditions){
        if(ac->isTrue()==false){
            return false;
        }
    }

    return true;

}


LogicState* ActionTrigger::parentLogicState()
{

    BaseObject* parent = mParentObject;

    while(parent){

        if(parent->baseType()==BaseObjectType::LOGIC_STATE){
            return static_cast<LogicState*>(parent);

        }else if(parent->baseType()==BaseObjectType::ACTION){
            Action *action = static_cast<Action *>(parent);
            parent = action->parentObject();

        }else{
            assert(false);
        }

    }

    assert(false);
    return nullptr;

}


}
