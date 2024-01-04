#ifndef PLAYER__LOGIC__LOGIC_ACTION_H
#define PLAYER__LOGIC__LOGIC_ACTION_H

#include <memory>
#include <string>
#include <vector>

#include "jmSignal.h"

#include "data/jpDataUtilities.h"
#include "jpGlobal.h"
#include "jpSettings.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

//class BehaviorAction;
class Item;
class SourceItemsStorage;
class Entity;
class GuiButton;
class PlayedScene;
//class ActionTrigger;
class Action;
class GuiWidget;
class AppConfigurationLoader_xml;
class ActionCommand;
class ActionCondition;
class Component;
class Compound;
class CompoundTransition;
class ShapeDrawer;
class CustomLogicState;

struct ActionCfg;
struct ActionTriggerCfg;
class LogicStateCfg;
class LogicFunctionCfg;


class LogicState;
class LogicFunction;


struct ParentObjects
{
    BaseObject *rootParentObject = nullptr;
    LogicState *parentLogicState = nullptr;
    LogicFunction *parentLogicFunction = nullptr;

};



class Action : public BaseObject
{
public:
    friend class AppConfigurationLoader_xml;


    Action(BaseObject* _parentObject);
    virtual ~Action();

    bool build(ActionCfg & _actionCfg);
    bool initConnections(PlayedScene *_scene);

    //----
    bool update(UpdateMode &_updateMode);

    //----
    BaseObject* parentObject(){ return mParentObject; }
    PlayedScene* parentPlayerScene(){ return mParentPlayerScene; }

    //ParentObjects parentObjects();
    LogicState* parentLogicState();



    bool breakOnExecuted(){ return mBreakOnExecuted; }


protected:
    BaseObject* mParentObject = nullptr;                    // LINK
    PlayedScene* mParentPlayerScene = nullptr;              // LINK


    //LogicFunction* mParentFunction = nullptr;
    //LogicState* mParentState = nullptr;
    //BaseObject* mRootParentObject = nullptr;

    bool mBreakOnExecuted = false;
    bool mDisabled = false;
    std::string mDbgId;


    std::vector<ActionCondition*>mConditions;                // OWNED
    std::vector<ActionCommand*>mCommands;                   // OWNED

    std::vector<Action*>mChildActions;                   // OWNED

};

//-------------------------------------------------------------------------------------------------------------


class ActionTrigger : public BaseObject
{
public:
    ActionTrigger(BaseObject* _parentObject);
    virtual ~ActionTrigger();

    bool build(ActionTriggerCfg & _actionCfg);
    bool initConnections(PlayedScene *_scene);

    //----
    bool isTriggered();

    //----
    BaseObject* parentObject(){ return mParentObject; }
    PlayedScene* parentPlayerScene(){ return mParentPlayerScene; }

    //ParentObjects parentObjects();
    LogicState* parentLogicState();



protected:
    BaseObject* mParentObject = nullptr;                    // LINK
    PlayedScene* mParentPlayerScene = nullptr;              // LINK

    std::vector<ActionCondition*>mConditions;                // OWNED



};


//--------------------------------------------------------------------------------------------------------------


class ActionCondition : public BaseObject
{
public:
    friend class AppConfigurationLoader_xml;

    ActionCondition(Action *_parentAction) : mParentAction(_parentAction) { mBaseType = BaseObjectType::ACTION_CONDITION; }
    ActionCondition(ActionTrigger *_parentAction) : mParentActionTrigger(_parentAction) { mBaseType = BaseObjectType::ACTION_CONDITION; }

    virtual ~ActionCondition(){}

    virtual bool initConnections(PlayedScene *scene){ return true; }

    //---
    virtual bool isTrue() = 0;

protected:
    Action *mParentAction = nullptr;                    // LINK
    ActionTrigger *mParentActionTrigger = nullptr;

    LogicState * mParentState = nullptr;                // LINK
    BaseObject * mRootParentObject = nullptr;           // LINK

    std::string mValue;

};


//-------------------------------------------------------------------------------------------------------------


class ActionCommand : public BaseObject
{
public:
    friend class AppConfigurationLoader_xml;

    ActionCommand(Action *_parentAction) : mParentAction(_parentAction) { mBaseType = BaseObjectType::ACTION_COMMAND; }

    virtual ~ActionCommand(){}

    virtual bool initConnections(PlayedScene *_scene){ return true; }

    //---
    virtual void run(UpdateMode &_updateMode){}


protected:
    Action *mParentAction = nullptr;             // LINK

    std::string mValue;

};




}

#endif // PLAYER__ACTION_COMMON_H
