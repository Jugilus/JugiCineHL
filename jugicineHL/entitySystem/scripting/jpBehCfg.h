#ifndef PLAYER__SCRIPTING__BEH_CFG__H
#define PLAYER__SCRIPTING__BEH_CFG__H

#include <memory>
#include <string>
#include <vector>
//#include "box2d.h"
//#include "jmCommon.h"
#include "jpGlobal.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

//struct GroundMovementCfg;
//struct JumpMovementCfg;
//struct PathMovementCfg;
struct TETransporterCfg;
//struct AnimatedMovementCfg;



struct ActionConditionCfg
{

    ActionConditionCfg(const std::string &_name, const std::string &_value) : name(_name), value(_value){}

    std::string name;
    std::string value;
};


struct ActionCommandCfg
{

    ActionCommandCfg(const std::string &_name, const std::string &_value) : name(_name), value(_value){}

    std::string name;
    std::string value;
};


struct BehaviorActionCfg
{
    //std::string mName;
    std::vector<ActionConditionCfg>mConditions;
    std::vector<ActionCommandCfg>mCommands;
};



struct BehaviorStateCfg
{

    BehaviorStateCfg(const std::string &_name) : name(_name){}

    bool initCfg(const pugi::xml_node &_node);


    std::string name;
    bool contactsEnabled = true;
    std::string animation;
    std::string movementEngine;
    std::string taskEngine;
    std::vector<BehaviorActionCfg>mUpdateActionsCfgs;
    BehaviorActionCfg mStartActionCfg;
};




struct EntityControllerCfg
{

    EntityControllerCfg(const std::string &_name) : name(_name){}

    bool initCfg(const pugi::xml_node &_node);


    std::string name;

    std::vector<BehaviorStateCfg>statesCfgs;

    //---
    //std::string decider;

};




class EnginesControllersCfgsGroup
{
public:

    ~EnginesControllersCfgsGroup();

    EntityControllerCfg * addEnginesControllerCfg(const std::string &_name);
    EntityControllerCfg * getEnginesControllerCfg(const std::string &_name, bool _setErrorMessage = true);

    std::vector<EntityControllerCfg*> & enginesControllersCfgs(){ return mEnginesControllersCfgs; }


private:
    std::vector<EntityControllerCfg*>mEnginesControllersCfgs;      // OWNED  (stored as pointers to avoid unvalidation problems when linking objects as references)


};


//------------------------------------------------------------------------------


//struct EnvironmentObjectBehaviorCfg
//{

//    std::vector<BehaviorStateCfg>mStatesCfgs;

//};



//------------------------------------------------------------------------------

/*
struct TaskControllerCfg
{

    TaskControllerCfg(const std::string &_name) : name(_name){}

    std::string name;

    std::vector<BehaviorStateCfg>mStatesCfgs;

    //---
    //std::vector<PointToPointMovementTaskCfg*>mPointToPointMovementTaskCfgs;
    // std::vector<JumpMovementCfg*>mJumpMovementCfgs;
    //std::vector<PathMovementCfg*>mPathMovementCfgs;

};




class TasksHandlersCfgsGroup
{
public:

    TaskControllerCfg & addTaskControllerCfg(const std::string &_name);
    TaskControllerCfg * getTaskControllerCfg(const std::string &_name, bool _setErrorMessage = true);

    std::vector<TaskControllerCfg> & taskControllersCfgs(){ return mTaskControllersCfgs; }


private:
    std::vector<TaskControllerCfg>mTaskControllersCfgs;


};
*/














}






#endif
