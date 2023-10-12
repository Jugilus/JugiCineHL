#ifndef PLAYER__SCRIPTING__BEH_COMMANDS__H
#define PLAYER__SCRIPTING__BEH_COMMANDS__H

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

class Entity;
class BehaviorState;
//class GameInputCommand;

//struct GroundMovementCfg;
//struct JumpMovementCfg;
//struct PathMovementCfg;

//struct GroundMovementData;
//struct JumpMovementData;
//struct PathMovementData;
//struct AnimatedMovementData;
//struct TETransporterData;




class DoPlayAnimation : public ActionCommand
{
public:
    static std::string type;



    DoPlayAnimation(BehaviorAction*_parentEntityAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    void update(UpdateMode _updateMode) override;


private:

};



//==============================================================================

/*

class DoGroundMovement : public ActionCommand
{
public:
    static std::string type;



    DoGroundMovement(BehaviorAction*_parentBehaviorAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    void update(UpdateMode _updateMode) override;


private:
    GroundMovementData *mGroundMovementData = nullptr;         // LINK

};


//==============================================================================


class DoJumpMovement : public ActionCommand
{
public:
    static std::string type;


    DoJumpMovement(BehaviorAction*_parentBehaviorAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    void update(UpdateMode _updateMode) override;


private:
    JumpMovementData *mJumpMovementData = nullptr;         // LINK


};


//==============================================================================


class DoPointToPointPathMovement : public ActionCommand
{
public:
    static std::string type;


    DoPointToPointPathMovement(BehaviorAction*_parentBehaviorAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    void update(UpdateMode _updateMode) override;


private:
    PathMovementData *mPathMovementData = nullptr;         // LINK


};


//==============================================================================


class DoPathMovement : public ActionCommand
{
public:
    static std::string type;


    DoPathMovement(BehaviorAction*_parentBehaviorAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    void update(UpdateMode _updateMode) override;


private:
    PathMovementData *mPathMovementData = nullptr;         // LINK


};


//==============================================================================


class DoAnimatedBehavior : public ActionCommand
{
public:
    static std::string type;


    DoAnimatedBehavior(BehaviorAction*_parentBehaviorAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    void update(UpdateMode _updateMode) override;


private:
    AnimatedBehaviorData *mAnimatedBehaviorData = nullptr;         // LINK


};

*/

//==============================================================================


/*
class DoPointToPointMovementTask : public ActionCommand
{
public:
    static std::string type;


    DoPointToPointMovementTask(BehaviorAction*_parentBehaviorAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    void update(UpdateMode _updateMode) override;


private:
    PointToPointMovementTaskData *mData = nullptr;         // LINK


};
*/

//==============================================================================


class DoSetState : public ActionCommand
{
public:
    static std::string type;



    DoSetState(BehaviorAction*_parentEntityAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    void update(UpdateMode _updateMode) override;


private:
    //GameInputCommand *mGameInputCommand = nullptr;      // LINK
    BehaviorState* mBehaviorState = nullptr;                   // LINK

};


//==============================================================================


class DoSetSignal : public ActionCommand
{
public:
    static std::string type;



    DoSetSignal(BehaviorAction*_parentEntityAction, const std::string &_value);

    bool initConnections(PlayedScene *_scene) override;

    //---
    void update(UpdateMode _updateMode) override;


private:

    Signal* mSignal = nullptr;                   // LINK
    SignalSetter mSignalSetter;

};


//==============================================================================

/*

class DoTransition : public ActionCommand
{
public:
    static std::string type;



    DoTransition(BehaviorAction*_parentEntityAction, const std::string &_value);

    bool initConnections(PlayedScene *scene) override;

    //---
    void update(UpdateMode _updateMode) override;


private:
    //GameInputCommand *mGameInputCommand = nullptr;      // LINK


};


*/








}






#endif
