#ifndef PLAYER__SCRIPTING__BEHAVIOR__H
#define PLAYER__SCRIPTING__BEHAVIOR__H

#include <memory>
#include <string>
#include <vector>
//#include "box2d.h"
#include "jmCommon.h"
#include "jmSignal.h"
#include "jpGlobal.h"


namespace pugi{
class xml_node;
}

namespace jugimap{



class PlayedScene;
class AppConfigurationLoader_xml;

struct EntityControllerCfg;
//struct TaskControllerCfg;
struct BehaviorStateCfg;
struct BehaviorActionCfg;
//struct EnvironmentObjectBehaviorCfg;
class Entity;
class BehaviorState;
class BehaviorAction;
class ActionCommand;
class ActionCondition;
class AnimationInstance;
struct MovementEngineData;
struct TaskEngineData;




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


    BehaviorState* state(const std::string &_stateName, bool _setErrorMessage = true);
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



//-----------------------------------------------------------------------------------

//enum class BehaviorStateStatus
//{
//    UNKNOWN = 0,
//    GROUND_MOVEMENET = 1 << 0

//};



class BehaviorState : public BaseObject
{
public:
    friend class AppConfigurationLoader_xml;


    BehaviorState(BehaviorStateCfg *_behaviorStateCfg, EntityController* _parent);
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
    //const std::string &name() const { return mName; }
    BehaviorStateCfg * behaviorStateCfg() { return mBehaviorStateCfg; }
    EntityController* parentStateManager() { return mParentObject; }
    //int status(){ return mStatus; }
    //void appendStatusFlag(BehaviorStateStatus _status){ mStatus |= static_cast<int>(_status); }

    //bool startedBeingActive() const { return mStartedBeingActive; }

    //---
    BehaviorAction* startAction(){ return mStartAction; }
    std::vector<BehaviorAction*>& actions(){ return mUpdateActions; }

    AnimationInstance* animationInstance(){ return mAnimationInstance; }
    MovementEngineData* movementEngineData(){ return mMovementEngineData; }
    TaskEngineData * taskEngineData() { return mTaskEngineData; }

    BoolSignal & stateTrigger() { return mStateTrigger; }
    //void setSwitchState(bool _active){ mSwitchState._Set(_active);}
    BoolSignal & animationTrigger() { return mAnimationTrigger; }

    std::vector<Signal*> & customEntitySignals(){ return mCustomEntitySignals; }


private:
    //std::string mName;
    BehaviorStateCfg *mBehaviorStateCfg = nullptr;              //LINK
    EntityController *mParentObject = nullptr;              // LINK

    //int mStatus = static_cast<int>(BehaviorStateStatus::UNKNOWN);

    std::vector<BehaviorAction*>mUpdateActions;                       // OWNED
    BehaviorAction* mStartAction = nullptr;



    //bool mStartedBeingActive = false;

    //---
    AnimationInstance * mAnimationInstance = nullptr;           // LINK
    MovementEngineData* mMovementEngineData = nullptr;          // LINK
    TaskEngineData* mTaskEngineData = nullptr;

    std::vector<Signal*> mCustomEntitySignals;      // OWNED

    //---
    bool mContactsEnabled = true;

    BoolSignal mStateTrigger;
    BoolSignal mAnimationTrigger;

    //----
    bool mUpdateInModalOverlayMode = false;





    //bool mStateStarted = false;
    //bool mStateEnded = false;

};


//---------------------------------------------------------------------------------------


class BehaviorAction
{
public:
    friend class AppConfigurationLoader_xml;


    BehaviorAction(BehaviorActionCfg* _behaviorActionCfg, BehaviorState* _parentObject);
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
    BehaviorActionCfg* mBehaviorActionCfg = nullptr;
    ActionStatus mStatus = ActionStatus::IDLE;

    std::vector<ActionCondition*>mConditions;                   // OWNED
    std::vector<ActionCommand*>mCommands;                   // OWNED

};














}






#endif
