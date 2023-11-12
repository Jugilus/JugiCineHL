#ifndef PLAYER__LOGIC__LOGIC_STATE_H
#define PLAYER__LOGIC__LOGIC_STATE_H

#include <memory>
#include <string>
#include <vector>

#include "jmSignal.h"

#include "jpGlobal.h"
#include "jpSettings.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

//class BehaviorAction;
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
struct LogicStateCfg;





class LogicState : public BaseObject
{
public:
    friend class AppConfigurationLoader_xml;

    LogicState(LogicStateCfg *_cfg, BaseObject* _parent);
    virtual ~LogicState();

    bool build(LogicStateCfg* _logicStateCfg);
    virtual bool initConnections(PlayedScene *_scene);

    //---
    void preUpdate_startState();
    void preUpdate_finishState();

    virtual bool startingPhaseUpdate();
    virtual void preUpdate(UpdateMode &_updateMode);
    virtual void update(UpdateMode &_updateMode);
    virtual void postUpdate(UpdateMode &_updateMode);

    virtual void drawShapes(jugimap::ShapeDrawer &_drawer, int _flags=0);

    //---
    const std::string &name(){ return mName;}
    BaseObject* parentObject(){ return mParentObject; }
    BaseObject* rootParentObject();
    LogicState* rootLogicState();

    LogicState* activeChildState(){ return mActiveChildState; }
    LogicState* nextChildState(){ return mNextChildState; }
    LogicState* previousChildState(){ return mPreviousChildState; }

    LogicState * findChildState(const std::string &_stateName, bool _setErrorMessage=true);
    LogicState* findChildStateViaPath(const std::string &_path, bool _setErrorMessage=true);
    LogicState* findChildStateViaPath(std::vector<std::string> &statePathParts, bool _setErrorMessage=true);

    void setNextChildState(LogicState* _gameState);

    PlayedScene* parentPlayerScene(){ return mParentPlayerScene; }
    CustomLogicState *customState(){ return mCustomState; }

    //---
    std::vector<LogicState*>& childStates(){ return mChildStates; }
    std::vector<Action*>& actions(){ return mActions; }
    std::vector<Signal*> & customUpdatedSignals(){ return mCustomUpdatedSignals; }

    //---
    BoolSignal & stateSignal() { return mSigState; }


private:

    LogicStateCfg* mCfg = nullptr;                      // LINK

    std::string mName;
    BaseObject *mParentObject = nullptr;                // LINK
    PlayedScene *mParentPlayerScene = nullptr;          // LINK

    std::vector<LogicState*>mChildStates;               // OWNED
    LogicState* mActiveChildState = nullptr;            // LINK
    LogicState *mNextChildState = nullptr;              // LINK
    LogicState *mPreviousChildState = nullptr;

    std::vector<Action*>mActions;                       // OWNED
    std::vector<Signal*> mCustomUpdatedSignals;          // OWNED

    CustomLogicState *mCustomState = nullptr;                // OWNED

    BoolSignal mSigState;


};




class CustomLogicState
{
public:

    CustomLogicState(){}
    CustomLogicState(LogicState *_parentState) : mParentState(_parentState){}
    virtual ~CustomLogicState(){}

    virtual bool build(){ return true; }
    virtual bool initConnections(PlayedScene *_scene){ return true; }
    virtual bool startingPhaseUpdate(){ return false; }
    virtual void start(){}
    virtual void preUpdate(UpdateMode &_updateMode){}
    virtual void update(UpdateMode &_updateMode){}
    virtual void postUpdate(UpdateMode &_updateMode){}
    virtual void finish(){}
    virtual void initTransitionFrom(CustomLogicState*){}
    virtual void drawShapes(jugimap::ShapeDrawer &_drawer, int _flags=0){}


protected:
    LogicState *mParentState = nullptr;


};



//--------------------------------------------------------------------------------------


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


protected:
    BaseObject* mParentObject = nullptr;                    // LINK
    PlayedScene* mParentPlayerScene = nullptr;              // LINK
    bool mDisabled = false;
    std::string mDbgId;

    std::vector<ActionCondition*>mConditions;                // OWNED
    std::vector<ActionCommand*>mCommands;                   // OWNED

};


//--------------------------------------------------------------------------------------------------------------


class ActionCondition : public BaseObject
{
public:
    friend class AppConfigurationLoader_xml;

    ActionCondition(Action *_parentAction) : mParentAction(_parentAction) { mBaseType = BaseObjectType::ACTION_CONDITION; }
    virtual ~ActionCondition(){}

    virtual bool initConnections(PlayedScene *scene){ return true; }

    //---
    virtual bool isTrue() = 0;

protected:
    Action *mParentAction = nullptr;             // LINK

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


//-------------------------------------------------------------------------------------------------------------


class OverlayState : public BaseObject
{
public:

    OverlayState(Compound *_compound);


    Compound * compound(){ return mCompound; }
    CompoundTransition *compoundTransition(){ return mCompoundTransition.get(); }


private:
    Compound *mCompound = nullptr;                      // LINK
    std::unique_ptr<CompoundTransition>mCompoundTransition;


};








}

#endif // PLAYER__ACTION_COMMON_H
