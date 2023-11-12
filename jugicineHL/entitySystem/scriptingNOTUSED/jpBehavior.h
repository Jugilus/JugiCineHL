#ifndef PLAYER__SCRIPTING__BEHAVIOR__H
#define PLAYER__SCRIPTING__BEHAVIOR__H

#include <memory>
#include <string>
#include <vector>
//#include "box2d.h"
#include "jmCommon.h"
#include "jmSignal.h"
#include "jpGlobal.h"

#include "jpLogicStateCfg.h"
#include "jpLogicState.h"


namespace pugi{
class xml_node;
}

namespace jugimap{



class PlayedScene;
class AppConfigurationLoader_xml;

struct EntityLogicStateCfg;
struct ActionCfg;
//struct EnvironmentObjectBehaviorCfg;
class Entity;
//class BehaviorState;
//class BehaviorAction;
class ActionCommand;
class ActionCondition;
class AnimationInstance;
struct MovementEngineData;
struct TaskEngineData;



/*

class EntityController : public BaseObject
{
public:
    friend class AppConfigurationLoader_xml;

    EntityController(EntityControllerCfg* _enginesControllerCfg, Entity *_parentActor);
    //EntityController(EntityControllerCfg* _taskHandlerCfg, Entity *_parentActor);
    //BehEnginesController(EnvironmentObjectBehaviorCfg* _envObjectBehControllerCfg, Actor *_parentActor);
    bool build();

    bool initConnections(PlayedScene *_scene);
    ~EntityController();

    void start();
    void update(UpdateMode _updateMode);

    void preUpdate(UpdateMode _updateMode);
    void postUpdate(UpdateMode _updateMode);


    BehaviorState* findState(const std::string &_stateName, bool _setErrorMessage = true);
    std::vector<BehaviorState*>& states(){ return mStates; }

    BehaviorState* currentState(){ return mCurrentState; }
    void setCurrentState(BehaviorState* _state);

    BehaviorState* nextState(){ return mNextState; }

    Entity* parentActor(){ return mParentActor; }


private:
    EntityControllerCfg *mEnginesControllerCfg = nullptr;                      // LINK
    //EntityControllerCfg *mTaskHandlerCfg = nullptr;
    //EnvironmentObjectBehaviorCfg *mEnvironmentObjectBehaviorCfg = nullptr;      // LINK
    Entity *mParentActor = nullptr;                                              // LINK
    std::vector<BehaviorState*>mStates;
    BehaviorState* mCurrentState = nullptr;

    BehaviorState* mNextState = nullptr;
    BehaviorState* mPreviousState = nullptr;

    //bool mStateStarted = false;

};

*/


//-----------------------------------------------------------------------------------

//enum class BehaviorStateStatus
//{
//    UNKNOWN = 0,
//    GROUND_MOVEMENET = 1 << 0

//};


/*

class BehaviorState : public BaseObject
{
public:
    friend class AppConfigurationLoader_xml;


    BehaviorState(EntityBehaviorStateCfg *_behaviorStateCfg, EntityController* _parent);
    bool build();
    bool initConnections(PlayedScene *_scene);
    ~BehaviorState();

    //---
    void preUpdate();
    void update(UpdateMode _updateMode);
    void postUpdate();

    void start(UpdateMode _updateMode);
    void finish(UpdateMode _updateMode);


    void _setUpdateInModalOverlayMode(bool _update){ mUpdateInModalOverlayMode = _update; }
    bool updateInModalOverlayMode(){ return mUpdateInModalOverlayMode; }

    //---
    EntityBehaviorStateCfg * behaviorStateCfg() { return mBehaviorStateCfg; }
    EntityController* parentStateManager() { return mParentObject; }

    //---
    //BehaviorAction* startAction(){ return mStartAction; }
    std::vector<BehaviorAction*>& actions(){ return mUpdateActions; }


    StateData *stateData(){ return mStateData; }

    //AnimationInstance* animationInstance(){ return mAnimationInstance; }
    //MovementEngineData* movementEngineData(){ return mMovementEngineData; }
    //TaskEngineData * taskEngineData() { return mTaskEngineData; }

    BoolSignal & stateTrigger() { return mStateTrigger; }
    //BoolSignal & animationTrigger() { return mAnimationTrigger; }

    std::vector<Signal*> & customEntitySignals(){ return mCustomEntitySignals; }


private:
    EntityBehaviorStateCfg *mBehaviorStateCfg = nullptr;              //LINK
    EntityController *mParentObject = nullptr;              // LINK


    std::vector<BehaviorAction*>mUpdateActions;                       // OWNED
    //BehaviorAction* mStartAction = nullptr;


    //---
    //AnimationInstance * mAnimationInstance = nullptr;           // LINK
    //MovementEngineData* mMovementEngineData = nullptr;          // LINK
    //TaskEngineData* mTaskEngineData = nullptr;
    //bool mContactsEnabled = true;
    //BoolSignal mAnimationTrigger;


    std::vector<Signal*> mCustomEntitySignals;      // OWNED

    BoolSignal mStateTrigger;


    //----
    bool mUpdateInModalOverlayMode = false;

    StateData *mStateData = nullptr;


};

*/




struct EntityLogicStateCfg : public LogicStateCfg
{

    EntityLogicStateCfg(const std::string &_name) : LogicStateCfg(_name){}

    bool initCfg(const pugi::xml_node &_node);

    LogicStateCfg* createLogicStateCfg(const std::string &_name) override;
    CustomLogicState* createCustomState(LogicState *_parentState) override;


    //std::string name;
    bool contactsEnabled = true;
    std::string animation;
    std::string movementEngine;
    std::string taskEngine;
    //std::vector<ActionCfg>mUpdateActionsCfgs;
    //BehaviorActionCfg mStartActionCfg;
};



//---------------------------------------------------------------------------------------


class EntitySystemStateData : public CustomLogicState
{
public:

    ~EntitySystemStateData() override;

    EntitySystemStateData(EntityLogicStateCfg *_behaviorStateCfg, LogicState *_parent);

    bool initConnections(PlayedScene *_scene) override;

    void start() override;
    void finish() override;


    AnimationInstance* animationInstance(){ return mAnimationInstance; }
    MovementEngineData* movementEngineData(){ return mMovementEngineData; }
    TaskEngineData * taskEngineData() { return mTaskEngineData; }
    bool contactsEnabled(){ return mContactsEnabled; }
    BoolSignal & animationTrigger() { return mAnimationTrigger; }

private:
    LogicState *mParentState = nullptr;                      // LINK
    EntityLogicStateCfg *mBehaviorStateCfg = nullptr;              // LINK

    //---
    AnimationInstance * mAnimationInstance = nullptr;           // LINK
    MovementEngineData* mMovementEngineData = nullptr;          // LINK
    TaskEngineData* mTaskEngineData = nullptr;                   // LINK

    BoolSignal mAnimationTrigger;

    //---
    bool mContactsEnabled = true;



};



//---------------------------------------------------------------------------------------

/*

class BehaviorAction
{
public:
    friend class AppConfigurationLoader_xml;


    BehaviorAction(ActionCfg* _behaviorActionCfg, BehaviorState* _parentObject);
    ~BehaviorAction();

    bool build();
    bool initConnections(PlayedScene *_scene);

    //----
    bool update(UpdateMode _updateMode);


    //----
    ActionStatus status() const { return mStatus; }
    //const std::string& name() const { return mName; }
    BehaviorState* parentBehaviorState(){ return mParentBehaviorState; }

    std::vector<ActionCommand*> & commands(){ return mCommands; }
    std::vector<ActionCondition*> & conditions(){ return mConditions; }

protected:
    //std::string mName;
    BehaviorState* mParentBehaviorState = nullptr;                    // LINK
    ActionCfg* mBehaviorActionCfg = nullptr;
    ActionStatus mStatus = ActionStatus::IDLE;

    std::vector<ActionCondition*>mConditions;                   // OWNED
    std::vector<ActionCommand*>mCommands;                   // OWNED

};

*/












}






#endif
