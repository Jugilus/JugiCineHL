#ifndef PLAYER__MOVEMENT__SPEED_GENERATORS__H
#define PLAYER__MOVEMENT__SPEED_GENERATORS__H

#include <memory>
#include <string>
#include <vector>
#include "box2d.h"
#include "jpGlobal.h"
#include "jpVariables.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

class PlayedScene;
class Animation;
class BoolSignal;
struct GroundMovementCfg;



class SpeedGenerator
{
public:

    enum class State
    {
        STANDING_STILL,
        ACCELERATING,
        FULL_SPEED,
        DECELERATING,
    };


    void setMovement(float _maxSpeed, float _accDistance, float _startSpeed);
    float updateSpeed(float _timeStep, bool generateSpeed);
    void stop();
    void setFullSpeed();

    float speed() const { return  mSpeed; }
    State state() const { return mState; }

    std::string dbgName;

private:



    State mState = State::STANDING_STILL;

    float mMaxSpeed = 0.0f;
    float mSpeed = 0.0f;
    float mAcc = 0.0;

    void dbgPrint();

};


//------------------------------------------------------------------------------------------


class ConstSpeedGenerator
{
public:

    void setMovement(float _maxSpeed, float _accDistance, float _startSpeed);
    float updateSpeed(float _timeStep, bool generateSpeed);
    void stop();

    float speed(){ return  mSpeed; }


private:

    float mMaxSpeed = 0.0f;
    float mSpeed = 0.0f;

};


//------------------------------------------------------------------------------------------


class JumpSpeedGenerator
{
public:

    static int finalFreefallSpeed;


    enum class State
    {
        ASCENDING_HIGH_SPEED,
        ASCENDING_LOW_SPEED,
        AT_MAX_HEIGHT,
        DESCENDING_LOW_SPEED,
        DESCENDING_HIGH_SPEED,
        DESCENDING_FINAL_SPEED,
        ON_GROUND
    };



    void start(float _heightMax, float _interHeight, float _pushOffSpeed, float _interSpeed, float _extraPushOffSpeed, bool _startWithDescending=false);
    float updateVelocity(float _timeStep);
    void stop();

    //void switchToHighSpeedDescending();

    State state(){ return mState; }
    void setState(State _state);
    void setOnBlockingCeiling();

    void setDistToCeiling(float _distToCeiling);
    float distToCeiling(){ return mDistToCeiling; }
    float speed(){ return mSpeed; }

    float interSpeed(){ return mInterSpeed; }
    float distance(){ return mDist; }


private:
    State mState = State::ON_GROUND;

    float mHeightMax = 2.0f;
    float mInterHeight = 0.0f;
    float mPushOffSpeed = 15.0f;
    float mInterSpeed = 10.0f;

    float mAccHighSpeed = 0.0f;
    float mAccLowSpeed = 0.0f;
    float mSpeed = 0.0f;
    float mDist = 0.0f;


    float mDistToCeiling = 0.0f;
    //---

    void dbgPrint();

};



//------------------------------------------------------------------------------------------



class PointToPointSpeedGenerator
{
public:

    enum class State
    {
        ACCELERATING,
        FULL_SPEED,
        DECELERATING,
        STANDING_STILL,
        FINISHED_FULL_SPEED,
    };


    void initMovement(float _maxSpeed, float _accDistance, float _distance);

    //void continueFullSpeed(float _distance, bool _resetDistance=false);
    void continueFullSpeed(float _nextDistance, float _passedDistance);

    float updateSpeed(float _timeStep, bool _finishFullSpeed);
    void stop();

    float speed(){ return mSpeed; }
    float distance(){ return mDistance; }
    float passedDistance(){ return mPassedDistance; }
    State state(){ return mState; }


private:


    State mState = State::STANDING_STILL;

    float mSpeedMaxTarget = 0.0f;
    float mSpeedMax = 0.0f;
    //float mSpeedMaxReached = 0.0f;
    float mAccDistanceTarget = 0.0f;
    float mAccDistance = 0.0f;
    float mAcc = 0.0f;

    //---
    float mDistance = 0.0;
    float mPassedDistance = 0.0;

    float mSpeed = 0.0f;

    void dbgPrint();


    float mPassedDistance2 = 0.0;
    float mSpeed2= 0.0f;
};







}






#endif
