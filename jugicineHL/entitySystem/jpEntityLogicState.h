#ifndef PLAYER__ENTITY_SYSTEM__ENTITY_LOGIC_STATE__H
#define PLAYER__ENTITY_SYSTEM__ENTITY_LOGIC_STATE__H

#include <memory>
#include <string>
#include <vector>

#include "jmCommon.h"
#include "jmSignal.h"
#include "jpGlobal.h"

#include "jpLogicStateCfg.h"
#include "jpLogicState.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


class AnimationInstance;
struct MovementEngineData;
struct TaskEngineData;




struct EntityLogicStateCfg : public LogicStateCfg
{

    EntityLogicStateCfg(const std::string &_name) : LogicStateCfg(_name){}

    bool initCfg(const pugi::xml_node &_node);

    LogicStateCfg* createLogicStateCfg(const std::string &_name) override;
    CustomLogicState* createCustomState(LogicState *_parentState) override;


    bool contactsEnabled = true;
    std::string animation;
    std::string movementEngine;
    std::string taskEngine;
};



//---------------------------------------------------------------------------------------


class EntityCustomLogicState : public CustomLogicState
{
public:

    ~EntityCustomLogicState() override;

    EntityCustomLogicState(EntityLogicStateCfg *_behaviorStateCfg, LogicState *_parent);

    bool initConnections(PlayedScene *_scene) override;

    void collectSignals(SignalStorage &_signalStorage, std::string _identifier) override;

    void start() override;
    void finish() override;


    AnimationInstance* animationInstance(){ return mAnimationInstance; }
    MovementEngineData* movementEngineData(){ return mMovementEngineData; }
    TaskEngineData * taskEngineData() { return mTaskEngineData; }
    bool contactsEnabled(){ return mContactsEnabled; }
    BoolSignal & animationSignal() { return mAnimationSignal; }

private:
    LogicState *mParentState = nullptr;                      // LINK
    EntityLogicStateCfg *mBehaviorStateCfg = nullptr;              // LINK

    //---
    AnimationInstance * mAnimationInstance = nullptr;           // LINK
    MovementEngineData* mMovementEngineData = nullptr;          // LINK
    TaskEngineData* mTaskEngineData = nullptr;                   // LINK

    BoolSignal mAnimationSignal;

    //---
    bool mContactsEnabled = true;



};










}






#endif
