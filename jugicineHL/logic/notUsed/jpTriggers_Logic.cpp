#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmAnimationCommon.h"
#include "map/jmSpriteLayer.h"
#include "jmGlobal.h"
#include "jmSystem.h"

#include "jpUtilities.h"
#include "jpTriggers_Logic.h"


namespace jugimap{


//-------------------------------------------------------------------------------

/*

std::string EGameStateStarted::type = "onStateStarted";


EGameStateStarted::EGameStateStarted(Action *_parentGameAction, const pugi::xml_node &_node)
{

    mParentGameAction = _parentGameAction;
    mStatePath = _node.attribute("state").as_string("");

}


bool EGameStateStarted::initConnections(PlayedScene *scene)
{

    dbgSystem.addMessage("init '" + type + "'");

    mGameState = ObtainLogicStateFromPath(scene, mParentGameAction, mStatePath);

    if(mGameState==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true ;
}


bool EGameStateStarted::isTriggered()
{

    bool result = mGameState->startedBeingActive();

    return result;

}

*/

//-------------------------------------------------------------------------------

/*

std::string EActionFinished::type = "onActionFinished";

EActionFinished::EActionFinished(Action *_parentGameAction, const pugi::xml_node &_node)
{

    mParentGameAction = _parentGameAction;
    mActionName = _node.attribute("action").as_string("");

}


bool EActionFinished::initConnections(PlayedScene *scene)
{

    dbgSystem.addMessage("init '" + type + "'");

    mGameAction = ObtainActionFromPath(scene, mParentGameAction, mActionName);

    if(mGameAction==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}


bool EActionFinished::isTriggered()
{

    bool result = (mGameAction->status()==ActionStatus::FINISHED);

    return result;

}

*/

//-------------------------------------------------------------------------------

/*
std::string TOnTrigger::type = "onTrigger";

TOnTrigger::TOnTrigger(Action *_parentGameAction, const pugi::xml_node &_node)
{

    mParentGameAction = _parentGameAction;
    mTriggerName = _node.attribute("trigger").as_string("");

}


bool TOnTrigger::initConnections(PlayedScene *scene)
{

    dbgSystem.addMessage("init '" + type + "'");

    mTrigger = ObtainTrigger(mParentGameAction, mTriggerName);

    if(mTrigger==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}


bool TOnTrigger::isTriggered()
{

    bool result = mTrigger->isTriggered();

    return result;

}

*/

//-------------------------------------------------------------------------------


/*

std::string TOnVariableChanged::type = "onVariableChanged";

TOnVariableChanged::TOnVariableChanged(Action *_parentGameAction, const pugi::xml_node &_node)
{

    mParentGameAction = _parentGameAction;
    mVariableName = _node.attribute("variable").as_string("");

}


bool TOnVariableChanged::initConnections(PlayerScene *scene)
{

    dbgSystem.addMessage("init '" + type + "'");


    mVariable = ObtainVariableFromPath(mVariableName, scene);

    if(mVariable==nullptr){
        return false;
    }

    mStored = Variable::copy(mVariable);


    dbgSystem.removeLastMessage();
    return true;

}


bool TOnVariableChanged::isTriggered()
{

    bool result = false;

    if(mVariable->type()==VariableType::INTEGER){
        IntVar *var =static_cast<IntVar*>(mVariable);
        IntVar *stored =static_cast<IntVar*>(mStored);
        result = var->value()==stored->value();
        stored->setValue(var->value());


    }else if(mVariable->type()==VariableType::FLOAT){
        FloatVar *var =static_cast<FloatVar*>(mVariable);
        FloatVar *stored =static_cast<FloatVar*>(mStored);
        result = var->value()==stored->value();
        stored->setValue(var->value());

    }else if(mVariable->type()==VariableType::BOOLEAN){
        BoolVar *var =static_cast<BoolVar*>(mVariable);
        BoolVar *stored =static_cast<BoolVar*>(mStored);
        result = var->value()==stored->value();
        stored->setValue(var->value());

    }else if(mVariable->type()==VariableType::STRING){
        StringVar *var =static_cast<StringVar*>(mVariable);
        StringVar *stored =static_cast<StringVar*>(mStored);
        result = var->value()==stored->value();
        stored->setValue(var->value());

    }
    return result;

}

*/


//-------------------------------------------------------------------------------

/*

std::string EDoAction::type = "do";

EDoAction::EDoAction(Action *_parentGameAction, const pugi::xml_node &_node)
{
    mParentGameAction = _parentGameAction;
}


bool EDoAction::initConnections(PlayedScene *scene)
{
    return true;
}


bool EDoAction::isTriggered()
{
    return true;
}


//-------------------------------------------------------------------------------


std::string EDoActionNew::type = "a";

EDoActionNew::EDoActionNew(Action *_parentGameAction, const pugi::xml_node &_node)
{

    mParentGameAction = _parentGameAction;
}


bool EDoActionNew::initConnections(PlayedScene *scene)
{
    return true;
}


bool EDoActionNew::isTriggered()
{
    return true;
}

*/


}
