#ifndef PLAYER__SCRIPTING__BEH_CONDITIONS__H
#define PLAYER__SCRIPTING__BEH_CONDITIONS__H

#include <memory>
#include <string>
#include <vector>
#include "box2d.h"
#include "jmCommon.h"
#include "jpGlobal.h"
#include "jpQueries.h"
#include "jpActionsCommon.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


struct PathMovementData;
struct GroundMovementData;
class GameInputCommand;
class SourceEntity;
class BehaviorState;







//==============================================================



/*

class ACOnContact : public ActionCondition
{
public:
    static std::string type_active;
    static std::string type_activeStart;
    static std::string type_activeEnd;


    enum class Style
    {
        ACTIVE_START,
        ACTIVE,
        ACTIVE_END
    };


    //Style GetPressingStyleFromString(const std::string &_style);


    ACOnContact(BehaviorAction*_parentEntityAction, const std::string &_value, const std::string &_type);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isConditionTrue() override;


private:
    //ButtonState *mTrigger = nullptr;      // LINK
    //bool mContactWithAnyActor = true;
    FixtureRole mContactFixture = FixtureRole::ACTOR_BODY;
    std::string mCustomContactFixtureCategory;
    //std::vector<SourceEntity*>mContactedActors;
    int mContactCategoriesFlags = 0;
    BehaviorEngineType mRequiredEngineType = BehaviorEngineType::UNKNOWN;

    Style mStyle = Style::ACTIVE;


};

*/

//==============================================================

/*

class OnGroundMovementState : public ActionCondition
{
public:
    static std::string type;


    OnGroundMovementState(BehaviorAction*_parentEntityAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isConditionTrue() override;


private:
    std::vector<GroundMovementState>mStates;

};

*/

//==============================================================

/*

class OnJumpMovementState : public ActionCondition
{
public:
    static std::string type;


    OnJumpMovementState(BehaviorAction*_parentEntityAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isConditionTrue() override;


private:
    std::vector<JumpMovementState>mStates;


};

*/




//==============================================================

/*
class OnGroundDetected : public ActionCondition
{
public:
    static std::string type;


    OnGroundDetected(BehaviorAction*_parentEntityAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isConditionTrue() override;


private:

    //bool mGroundDetected = true;
    bool value = false;


};

*/

//==============================================================

/*
class OnPatwayDetected : public ActionCondition
{
public:
    static std::string type;

    enum class AvailableDirection
    {
        FORWARD,
        BACKWARD
    };


    OnPatwayDetected(BehaviorAction*_parentEntityAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isConditionTrue() override;


private:

    std::string mPatwayName;
    AvailableDirection mAvailableDirection = AvailableDirection::FORWARD;


};

*/
//==============================================================

/*
class OnPatwayAccessed : public ActionCondition
{
public:
    static std::string type;



    OnPatwayAccessed(BehaviorAction*_parentEntityAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isConditionTrue() override;


private:
    std::string mPatwayName;
    PathMovementData *mPathMovementData = nullptr;

};

*/
//==============================================================

/*

class OnPatwayEnd : public ActionCondition
{
public:
    static std::string type;



    OnPatwayEnd(BehaviorAction*_parentEntityAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isConditionTrue() override;


private:

    bool value = false;

};

*/

//==============================================================

/*

class OnPathwayToGroundMovement : public ActionCondition
{
public:
    static std::string type;



    OnPathwayToGroundMovement(BehaviorAction*_parentEntityAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isConditionTrue() override;


private:

    GroundMovementData *mGroundMovementData = nullptr;

};

*/



//==============================================================


class ACOnSignal : public ActionCondition
{
public:
    static std::string type;
    //static std::string type_activeStart;
    //static std::string type_activeEnd;

    //enum class Style
    //{
    //    ACTIVE_START,
    //    ACTIVE,
    //    ACTIVE_END
    //};

    //Style GetPressingStyleFromString(const std::string &_style);


    ACOnSignal(BehaviorAction*_parentEntityAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    bool isConditionTrue() override;


private:
    //BoolValueState *mTrigger = nullptr;                    // LINK
    //TriggerResult mTriggerStyle = TriggerResult::ACTIVE;
    SignalQuery mSignalQuery;


};

//==============================================================




class ACOnSignals : public ActionCondition
{
public:
    static std::string type;



    ACOnSignals(BehaviorAction*_parentEntityAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    bool isConditionTrue() override;


private:


    CompositeQuery mCompositeQuery;

};



}






#endif
