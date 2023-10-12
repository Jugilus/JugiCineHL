#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmGuiCommon.h"
#include "jmGuiBar.h"
#include "jmAnimationCommon.h"
#include "map/jmSpriteLayer.h"
#include "map/jmMap.h"
#include "jmGlobal.h"
#include "jmScene.h"
#include "jmSceneLayout.h"
#include "jmUtilities.h"
#include "jpVariables.h"
#include "jpUtilities.h"
#include "jpComponentsCommon.h"
#include "jpPlayedScene.h"
#include "jpObjectFactory.h"
#include "jpActionsCommon.h"



namespace jugimap{



//bool LogicState::longActionActive = false;


LogicState::LogicState(const std::string &_name, BaseObject* _parent ) : mName(_name), mParentObject(_parent)
{
    mType = BaseObjectType::LOGIC_STATE;
}


LogicState::~LogicState()
{

    for(Action* ga : mActions){
        delete ga;
    }

    for(LogicState *gs : mChildStates){
        delete  gs;
    }

    if(mTriggers){
        delete mTriggers;
    }

}


bool LogicState::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("initActionTasksAndEvents for state '" + mName +"'");


    mParentPlayerScene = _scene;

    //---
    for(Action* ga : mActions){
        if(ga->initConnections(_scene)==false){
            return false;
        }
    }

    //---
    if(mComponentNames){
        for(const std::string &cName : *mComponentNames){
            Component *component = _scene->componentsGroup()->getComponent(cName);
            if(component==nullptr){
                return false;
            }
            //if(component->initConnections(_scene)==false){
            //    return false;
            //}
            mUsedComponents.push_back(component);
        }

        delete mComponentNames;
        mComponentNames = nullptr;

    }

    //---
    if(mWidgetNames){
        for(const std::string &s : *mWidgetNames){
            GuiWidget *widget = ObtainGuiWidget( _scene, s, GuiWidgetKind::NOT_DEFINED);
            if(widget==nullptr){
                return false;
            }
            mUsedWidgets.push_back(widget);
        }

        delete mWidgetNames;
        mWidgetNames = nullptr;
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


/*
void LogicState::preUpdate()
{

    if(mTriggers){
        mTriggers->update();
    }
    if(mActiveChildState){
        mActiveChildState->preUpdate();
    }

}
*/


bool LogicState::startingPhaseUpdate()
{

    bool startingPhase = false;

    for(Component* c : mUsedComponents){
        if(c->startingPhaseUpdate()==true){
            startingPhase = true;
            //return true;
        }
    }

    if(mActiveChildState){
        if(mActiveChildState->startingPhaseUpdate()==true){
            //return true;
            startingPhase = true;
        }
    }

    //return false;
    return startingPhase;

}


void LogicState::update(UpdateMode _updateMode)
{

    for(Action* ga : mActions){
        ga->update(_updateMode);
    }
    for(Component* c : mUsedComponents){
        c->update(_updateMode);
    }

    if(mActiveChildState){
        mActiveChildState->update(_updateMode);
    }

    //---
    mStartedBeingActive = false;     // after the first update reset the flag

}



void LogicState::drawShapes(jugimap::ShapeDrawer &_drawer, int _flags)
{

    for(Component* c : mUsedComponents){
        c->drawShapes(_drawer, _flags);
    }

    if(mActiveChildState){
        mActiveChildState->drawShapes(_drawer, _flags);
    }

}



/*
void LogicState::postUpdate()
{


}
*/


void LogicState::start()
{

    mParentPlayerScene->widgetManager()->appendToUsedWidgets(mUsedWidgets);
    for(Component *c : usedComponents()){
        c->start();
    }

    if(mActiveChildState){
        mActiveChildState->start();
    }

}


void LogicState::setActiveChildState(LogicState *_gameState)
{

    if(mActiveChildState==_gameState){
        return;
    }

    //--- finish old current state logic objects
    if(mActiveChildState){
        mActiveChildState->finish();
    }

    //---
    if(StdVector::contains(mChildStates, _gameState)==false){       // debug check
        assert(false);
    }

    mActiveChildState = _gameState;

    //--- start new current state logic objects
    mActiveChildState->start();
    //mParentPlayerScene->widgetManager()->appendToUsedWidgets(mActiveChildState->mUsedWidgets);
    //for(Component *c : mActiveChildState->usedComponents()){
    //    c->start();
    //}

    //--- set one frame flag
    mActiveChildState->mStartedBeingActive = true;

}


void LogicState::finish()
{

    mParentPlayerScene->widgetManager()->removeFromUsedWidgets(mUsedWidgets);
    for(Component *c : usedComponents()){
        c->onStateEnded();
    }

    if(mActiveChildState){
        mActiveChildState->finish();
    }

}


LogicState* LogicState::getChildState(const std::string &_name)
{

    for(LogicState* ls : mChildStates){
        if(ls->name()==_name){
            return ls;
        }
    }

    return nullptr;

}


void LogicState::syncReplacementActiveState(LogicState* _replacementState)
{

    if(mActiveChildState){
        LogicState *lsActiveRepl = _replacementState->getChildState(mActiveChildState->name());
        if(lsActiveRepl){
            _replacementState->setActiveChildState(lsActiveRepl);

        }else if(_replacementState->childStates().empty()==false){
            _replacementState->setActiveChildState(_replacementState->childStates().front());
        }
    }

    //---
    for(LogicState *ls : mChildStates){
        LogicState* lsReplacement = _replacementState->getChildState(ls->name());
        if(lsReplacement){
            ls->syncReplacementActiveState(lsReplacement);
        }
    }

}


void LogicState::_createTriggers()
{
    if(mTriggers==nullptr){
        mTriggers = new TriggerVariableManager();
    }
}


/*
bool LogicState::hasLongActionActive()
{

    for(Action* ga : mActions){
        if(ga->status() != ActionStatus::IDLE){
            return true;
        }
    }

    if(mActiveChildState){
        bool longActionInChildActive = mActiveChildState->hasLongActionActive();
        if(longActionInChildActive){
            return true;
        }
    }

    return false;

}
*/




//---------------------------------------------------------------------------------------------------------


Action::Action(const std::string &_name, BaseObject* _parentObject):
    mName(_name),
    mParentObject(_parentObject)
{
    mType = BaseObjectType::ACTION;
}


Action::~Action()
{

    for(ActionCommand *gat : mCommands){
        delete gat;
    }

    for(ActionTrigger *ge : mTriggers){
        delete ge;
    }

}


bool Action::createTriggersAndCommands(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("createActionTasksAndEvents for action '" + mName +"'");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        // nodeName is the type of the action trigger

        ActionTrigger *e = actionFactory->createActionTrigger(nodeName, this, n);

        if(e){
            mTriggers.push_back(e);


            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                std::string childNodeName = std::string(nChild.name());

                // nodeName is the type of the action command or action condition
                ActionCondition* condition = actionFactory->createActionCondition(childNodeName, this, nChild);
                if(condition){
                    for(pugi::xml_node nChild2 = nChild.first_child(); nChild2; nChild2 = nChild2.next_sibling()){
                        std::string childNodeName2 = std::string(nChild2.name());

                        ActionCommand *a = actionFactory->createActionCommands(childNodeName2, this, nChild2);
                        if(a){
                            a->_setCondition(condition);
                            mCommands.push_back(a);

                        }else{
                            dbgSystem.addMessage("Action command '" + childNodeName2 +"' not found!");
                            return false;
                        }

                    }
                }else{

                    ActionCommand *a = actionFactory->createActionCommands(childNodeName, this, nChild);
                    if(a){
                        mCommands.push_back(a);
                    }else{
                        dbgSystem.addMessage("Action command '" + childNodeName +"' not found!");
                        return false;
                    }
                }
           }

        }else{
            dbgSystem.addMessage("Action trigger '" + nodeName +"' not found!");
            return false;
        }

    }

    dbgSystem.removeLastMessage();

    return true;
}


bool Action::createTriggersAndCommands_anonymousAction(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("createTriggersAndCommands for anonymous action");


    std::string nodeName = std::string(_node.name());

    // nodeName is the type of the action trigger

    ActionTrigger *e = actionFactory->createActionTrigger(nodeName, this, _node);


    if(e){
        mTriggers.push_back(e);

        for(pugi::xml_node nChild = _node.first_child(); nChild; nChild = nChild.next_sibling()){
            std::string childNodeName = std::string(nChild.name());

            // nodeName is the type of the action command or action condition

            ActionCondition* condition = actionFactory->createActionCondition(childNodeName, this, nChild);
            if(condition){
                for(pugi::xml_node nChild2 = nChild.first_child(); nChild2; nChild2 = nChild2.next_sibling()){
                    std::string childNodeName2 = std::string(nChild2.name());

                    ActionCommand *a = actionFactory->createActionCommands(childNodeName2, this, nChild2);
                    if(a){
                        a->_setCondition(condition);
                        mCommands.push_back(a);

                    }else{
                        dbgSystem.addMessage("Action command '" + childNodeName2 +"' not found!");
                        return false;
                    }
                }

            }else{

                ActionCommand *a = actionFactory->createActionCommands(childNodeName, this, nChild);
                if(a){
                    mCommands.push_back(a);
                }else{
                    dbgSystem.addMessage("Action command '" + childNodeName +"' not found!");
                    return false;
                }
            }

            // childNodeName is the type of the action command
            //ActionCommand *a = actionFactory->createActionCommands(childNodeName, this, nChild);

            //if(a){
            //    mCommands.push_back(a);

            //}else{
           //     dbgSystem.addMessage("Action command '" + childNodeName +"' not found!");
           //     return false;
          //  }
        }


    }else{
        dbgSystem.addMessage("Action trigger '" + nodeName +"' not found!");
        return false;
    }


    dbgSystem.removeLastMessage();

    return true;
}


bool Action::createCommands_anonymousAction(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("createCommands for anonymous action");


    for(pugi::xml_node nChild = _node.first_child(); nChild; nChild = nChild.next_sibling()){
        std::string childNodeName = std::string(nChild.name());

        // childNodeName is the type of the action command

        ActionCommand *a = actionFactory->createActionCommands(childNodeName, this, nChild);

        if(a){
            mCommands.push_back(a);

        }else{
            dbgSystem.addMessage("Action command '" + childNodeName +"' not found!");
            return false;
        }
    }


    dbgSystem.removeLastMessage();

    return true;
}


bool Action::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init triggers and commands in action '" + mName +"'");

    mParentPlayerScene = _scene;

    for(ActionCommand *gat : mCommands){

        if(gat->condition()){
            gat->condition()->initConnections(_scene);
        }
        if(gat->initConnections(_scene)==false){
            return false;
        }
    }

    for(ActionTrigger *ge : mTriggers){

        if(ge->initConnections(_scene)==false){
            return false;
        }
    }


    dbgSystem.removeLastMessage();

    return true;
}




/*
void Action::update()
{

    if(mStatus==ActionStatus::DISABLED){
        return;
    }

    if(mStatus == ActionStatus::IDLE){

        // if no trigger is defined the action is executed immediately

        bool doAction = true;

        for(ActionTrigger* ged : mTriggers){
            if(ged->gameEventIsTrue()==false){
                doAction = false;
                break;
            }
        }

        if(doAction){


            int tasksStatus = static_cast<int>(ActionStatus::IDLE);

            for(ActionCommand* gat : mCommands){
                gat->execute();

                tasksStatus |= static_cast<int>(gat->status());
            }

            if(tasksStatus & static_cast<int>(ActionStatus::STARTED)){
                mStatus = ActionStatus::STARTED;
            }
        }


    }else{      // this is the case for long actions


        int tasksStatus = static_cast<int>(ActionStatus::IDLE);

        for(ActionCommand* gat : mCommands){
            gat->update();

            tasksStatus |= static_cast<int>(gat->status());
        }

        if(tasksStatus & static_cast<int>(ActionStatus::ACTIVE)){
            mStatus = ActionStatus::ACTIVE;

        }else if(tasksStatus & static_cast<int>(ActionStatus::FINISHED)){
            mStatus = ActionStatus::FINISHED;

        }else if(tasksStatus == static_cast<int>(ActionStatus::IDLE)){
            mStatus = ActionStatus::IDLE;

        }

    }


}
*/



void Action::update(UpdateMode _updateMode)
{

    if(_updateMode==UpdateMode::MODAL_OVERLAY && mUpdateInModalOverlayMode==false){
        return;
    }

    if(mStatus==ActionStatus::DISABLED){
        return;
    }

    if(mStatus == ActionStatus::IDLE){

        // if no trigger is defined the action is executed immediately

        bool doAction = true;

        for(ActionTrigger* ged : mTriggers){
            if(ged->isTriggered()==false){
                doAction = false;
                break;
            }
        }

        if(doAction){

            mCommandIndex = 0;

            for(int i=mCommandIndex; i<mCommands.size(); i++){

                ActionCommand* gat = mCommands[i];
                if(gat->isConditionTrue()==false){
                    continue;
                }

                gat->execute();
                if(gat->status()!=ActionStatus::IDLE){
                    mStatus = ActionStatus::STARTED;
                    mCommandIndex = i;
                    //---
                    mParentPlayerScene->addLongRunningAction(this);     // make scene aware that a long action is running

                    break;
                }
            }
        }


    }else{      // this is the case for long actions


        mStatus = ActionStatus::IDLE;

        while(mCommandIndex < mCommands.size()){

            ActionCommand* gat = mCommands[mCommandIndex];

            if(gat->status()!=ActionStatus::IDLE){
                gat->update(_updateMode);

                if(gat->status()!=ActionStatus::IDLE){
                    mStatus = gat->status();
                    break;

                }else{
                    mCommandIndex++;
                    continue;
                }
            }

            //--- procced with the next command after the long action finishes

            if(gat->isConditionTrue()==false){
                mCommandIndex++;
                continue;
            }

            gat->execute();
            if(gat->status()!=ActionStatus::IDLE){
                mStatus = ActionStatus::STARTED;
                break;
            }else{
                mCommandIndex++;
                continue;
            }
        }

        if(mStatus == ActionStatus::IDLE){
            mParentPlayerScene->removeLongRunningAction(this);     // make scene aware that a long action is running
        }
    }

}



void Action::reset()
{

    for(ActionCommand* gat : mCommands){
        gat->reset();
    }

}



//-----------------------------------------------------------------------------------


bool ActionCommand::isConditionTrue()
{
    if(mCondition == nullptr){        // no condition
        return true;
    }

    return mCondition->isConditionTrue();

}





}
