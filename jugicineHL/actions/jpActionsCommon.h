#ifndef PLAYER__ACTION_COMMON_H
#define PLAYER__ACTION_COMMON_H

#include <memory>
#include <string>
#include <vector>

#include "jpGlobal.h"
#include "jpSettings.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

class BehaviorAction;
class Entity;
class GuiButton;
class PlayedScene;
class ActionTrigger;
class Action;
class GuiWidget;
class AppConfigurationLoader_xml;
class ActionCommand;
class ActionCondition;
class Component;
class ShapeDrawer;




class LogicState : public BaseObject
{
public:
    friend class AppConfigurationLoader_xml;


    LogicState(const std::string &_name, BaseObject* _parent );
    bool initConnections(PlayedScene *_scene);
    virtual ~LogicState();

    //---
    //virtual void preUpdate();
    virtual bool startingPhaseUpdate();
    virtual void update(UpdateMode _updateMode);
    virtual void drawShapes(jugimap::ShapeDrawer &_drawer, int _flags=0);
    //virtual void postUpdate();

    void start();
    void finish();


    //---
    const std::string &name(){ return mName;}
    BaseObject* parentObject(){ return mParentObject; }
    PlayedScene* parentPlayerScene(){ return mParentPlayerScene; }

    //---
    void setActiveChildState(LogicState* _gameState);
    LogicState* activeChildState(){ return mActiveChildState; }
    std::vector<LogicState*>& childStates(){ return mChildStates; }
    LogicState* getChildState(const std::string &_name);


    //---
    void syncReplacementActiveState(LogicState* _replacementState);

    bool startedBeingActive(){ return mStartedBeingActive; }

    //---
    std::vector<Action*>& actions(){ return mActions; }
    std::vector<GuiWidget*> &usedWidgets() { return mUsedWidgets; }
    TriggerVariableManager* triggers(){ return  mTriggers; }
    std::vector<Component*> &usedComponents(){ return mUsedComponents; }


    //---
    void _createTriggers();


private:


    std::string mName;
    BaseObject *mParentObject = nullptr;                // LINK
    PlayedScene *mParentPlayerScene = nullptr;          // LINK

    std::vector<Action*>mActions;                       // OWNED
    std::vector<LogicState*>mChildStates;               // OWNED
    LogicState* mActiveChildState = nullptr;            // LINK

    std::vector<GuiWidget*>mUsedWidgets;                // LINKS
    TriggerVariableManager* mTriggers = nullptr;        // OWNED
    std::vector<Component*>mUsedComponents;             // LINKS

    bool mStartedBeingActive = false;

    //---
    std::vector<std::string> *mWidgetNames = nullptr;       // temporary object for init
    std::vector<std::string> *mComponentNames = nullptr;     // temporary object for init
};


//---------------------------------------------------------------------------------------

class Action : public BaseObject
{
public:
    friend class AppConfigurationLoader_xml;


    Action(const std::string &_name, BaseObject* _parentObject);
    virtual ~Action();

    bool createTriggersAndCommands(const pugi::xml_node &_node);
    bool createTriggersAndCommands_anonymousAction(const pugi::xml_node &_node);
    bool createCommands_anonymousAction(const pugi::xml_node &_node);

    bool initConnections(PlayedScene *_scene);

    //----
    void update(UpdateMode _updateMode);
    void reset();


    void _setUpdateInModalOverlayMode(bool _update){ mUpdateInModalOverlayMode = _update; }
    bool updateInModalOverlayMode(){ return mUpdateInModalOverlayMode; }

    //----
    ActionStatus status(){ return mStatus; }
    const std::string& name(){ return mName; }
    BaseObject* parentObject(){ return mParentObject; }
    PlayedScene* parentPlayerScene(){ return mParentPlayerScene; }


protected:
    std::string mName;
    BaseObject* mParentObject = nullptr;                    // LINK
    PlayedScene* mParentPlayerScene = nullptr;              // LINK

    ActionStatus mStatus = ActionStatus::IDLE;

    std::vector<ActionTrigger*>mTriggers;                   // OWNED
    std::vector<ActionCommand*>mCommands;                   // OWNED

    int mCommandIndex = 0;

    //----
    bool mUpdateInModalOverlayMode = false;

};


//--------------------------------------------------------------------------------------------------------------


class ActionTrigger : public BaseObject
{
public:
    friend class AppConfigurationLoader_xml;


    ActionTrigger(){ mType = BaseObjectType::ACTION_TRIGGER; }
    virtual ~ActionTrigger(){}

    virtual bool initConnections(PlayedScene *scene){ return true; }

    //---
    virtual bool isTriggered() = 0;


    //---
    Action* parentGameAction(){ return mParentGameAction; }

protected:
    Action *mParentGameAction = nullptr;             // LINK

};

//--------------------------------------------------------------------------------------------------------------


class ActionCondition : public BaseObject
{
public:
    friend class AppConfigurationLoader_xml;


    ActionCondition(){ mType = BaseObjectType::ACTION_CONDITION; }
    //ActionCondition(ActionCommand *_actionCommand) : mActionCommand(_actionCommand){ mType = BaseObjectType::ACTION_CONDITION; }
    virtual ~ActionCondition(){}

    virtual bool initConnections(PlayedScene *scene){ return true; }

    //---
    virtual bool isConditionTrue() = 0;

    //---
    Action* parentGameAction(){ return mParentGameAction; }
    //ActionCommand* parentActionCommand(){ return mActionCommand; }

    const std::string & value(){ return mValue; }

protected:
    Action *mParentGameAction = nullptr;             // LINK
    BehaviorAction *mParentBehaviorAction = nullptr;             // LINK if parent is entity action

    std::string mValue;

};


//-------------------------------------------------------------------------------------------------------------


class ActionCommand : public BaseObject
{
public:
    friend class AppConfigurationLoader_xml;


    ActionCommand(){ mType = BaseObjectType::ACTION_COMMAND; }
    ActionCommand(BehaviorAction *_entityAction) : mParentBehaviorAction(_entityAction)
    { mType = BaseObjectType::ACTION_COMMAND; }

    virtual ~ActionCommand(){}

    virtual bool initConnections(PlayedScene *_scene){ return true; }

    //---
    virtual void execute(){}
    virtual void start(UpdateMode _updateMode) {}
    virtual void update(UpdateMode _updateMode){}
    virtual void reset(){}

    bool isConditionTrue();


    //---
    ActionStatus status(){ return mStatus; }
    Action * parentGameAction(){ return mParentGameAction; }
    ActionCondition* condition(){ return mCondition; }
    void _setCondition(ActionCondition* _condition){ mCondition = _condition; }


    const std::string & value(){ return mValue; }


protected:
    BehaviorAction *mParentBehaviorAction = nullptr;      // LINK
    ActionCondition* mCondition = nullptr;    // OWNED


    ActionStatus mStatus = ActionStatus::IDLE;
    Action *mParentGameAction = nullptr;             // LINK

    std::string mValue;

};











}

#endif // PLAYER__ACTION_COMMON_H
