#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmCommonFunctions.h"
#include "jmSystem.h"
#include "jpPlayedApp.h"
#include "jpUtilities.h"
#include "jpInput.h"

#include "jpB2World.h"
#include "jpSpeedGenerators.h"



namespace jugimap{







void SpeedGenerator::setMovement(float _maxSpeed, float _accDistance, float _startSpeed)
{

    mMaxSpeed = _maxSpeed;

    if(_accDistance > 0.001f){
        mAcc = (mMaxSpeed*mMaxSpeed)/(2.0f * _accDistance);
    }else{
        mAcc = 1000000.0f;   // some big number to increase mSpeed immediately
    }


    mSpeed = _startSpeed;

    assert(mSpeed>=0.0f);

    if(mSpeed==0.0f){
        mState = State::STANDING_STILL;
    }else if(mSpeed<mMaxSpeed){
        mState = State::ACCELERATING;
    }else{
        mState = State::FULL_SPEED;
    }

}


float SpeedGenerator::updateSpeed(float _timeStep, bool generateSpeed)
{

    float dt = _timeStep;

    if(generateSpeed){

        if(mSpeed < mMaxSpeed){
            mSpeed += mAcc * dt;
            mState = State::ACCELERATING;

            if(mSpeed >= mMaxSpeed){
                mSpeed = mMaxSpeed;
                mState = State::FULL_SPEED;
            }

        }else if(mSpeed > mMaxSpeed){           // also possible
            mSpeed -= mAcc * dt;
            mState = State::FULL_SPEED;

            if(mSpeed <= mMaxSpeed){
                mSpeed = mMaxSpeed;
                mState = State::FULL_SPEED;
            }
        }

    }else{

        if(mSpeed > 0){
            mSpeed -= mAcc * dt;
            mState = State::DECELERATING;

            if(mSpeed<=0.0f){
                mSpeed = 0.0f;
                mState = State::STANDING_STILL;
            }
        }
    }

    //dbgPrint();

    return mSpeed;
}


void SpeedGenerator::stop()
{

    mSpeed = 0.0;
    mState = State::STANDING_STILL;

    //print("SpeedGenerator '" + dbgName + "' stop!");

}


void SpeedGenerator::setFullSpeed()
{

    mSpeed = mMaxSpeed;
    mState = State::FULL_SPEED;

    //print("SpeedGenerator '" + dbgName + "' stop!");
}


void SpeedGenerator::dbgPrint()
{

    if(mState==State::ACCELERATING){
        print("ACCELERATING");

    }else if(mState==State::FULL_SPEED){
       print("FULL SPEED");

    }else if(mState==State::DECELERATING){
        print("DECELERATING");

    }else if(mState==State::STANDING_STILL){
        print("STANDING_STILL");
    }
    print("mSpeed="+ std::to_string(mSpeed));

}



//==========================================================================================================


void ConstSpeedGenerator::setMovement(float _maxSpeed, float _accDistance, float _startSpeed)
{

    mMaxSpeed = _maxSpeed;
    mSpeed = 0.0f;

}


float ConstSpeedGenerator::updateSpeed(float _timeStep, bool generateSpeed)
{

    if(generateSpeed){
        mSpeed = mMaxSpeed;

    }else{
        mSpeed = 0.0f;
    }

    return mSpeed;
}


void ConstSpeedGenerator::stop()
{
    mSpeed = 0.0;

}



//============================================================================================

int JumpSpeedGenerator::finalFreefallSpeed = 20;



void JumpSpeedGenerator::start(float _heightMax, float _interHeight, float _pushOffSpeed, float _interSpeed, float _extraPushOffSpeed, bool _startWithDescending)
{

    mHeightMax = _heightMax;
    mInterHeight = _interHeight;
    mPushOffSpeed = _pushOffSpeed + _extraPushOffSpeed;
    mInterSpeed = _interSpeed;

    mAccHighSpeed = (mPushOffSpeed*mPushOffSpeed - mInterSpeed*mInterSpeed)/((mInterHeight)*2.0f);

    if(_startWithDescending){
        //mDist = mInterHeight;
        //mSpeed = -mInterSpeed;
        //mState = State::DESCENDING_HIGH_SPEED;

        setState(State::DESCENDING_HIGH_SPEED);

    }else{
        //mDist = 0.0f;
        //mSpeed = mPushOffSpeed;
        //mState = State::ASCENDING_HIGH_SPEED;

        setState(State::ASCENDING_HIGH_SPEED);
    }

    mDistToCeiling = 0.0f;
}


/*
void EJumping::startDescendingJump()
{


    mState = JumpingState::DESCENDING_HIGH_SPEED;
    mDist = mInterHeight;

    //float mSpeed = (mSpeedMax + mInterSpeed)/2.0f;
    mSpeed = -mInterSpeed;

    //mAccHighSpeed = (mSpeed*mSpeed)/(mHeightMax*2.0f);

}
*/


void JumpSpeedGenerator::setState(State _state)
{

    mState = _state;


    if(mState==State::ASCENDING_HIGH_SPEED){
        mDist = 0.0f;
        mSpeed = mPushOffSpeed;

    }else if(mState==State::ASCENDING_LOW_SPEED){
        mDist = mInterHeight;
        mSpeed = mInterSpeed;
        mAccLowSpeed = std::abs((mInterSpeed*mInterSpeed)/(2.0f * (mHeightMax - mInterHeight)));

    }else if(mState==State::AT_MAX_HEIGHT){
        mDist = mHeightMax;
        mSpeed = 0.0f;

    }else if(mState==State::DESCENDING_LOW_SPEED){      // the same as at max height
        mDist = mHeightMax;
        mSpeed = 0.0f;

    }else if(mState==State::DESCENDING_HIGH_SPEED){
        mDist = mInterHeight;
        mSpeed = -mInterSpeed;

    }else if(mState==State::DESCENDING_FINAL_SPEED){
        mDist = 0.0f;                   // distance has no role in this state
        mSpeed = -finalFreefallSpeed;

    }

}


void JumpSpeedGenerator::setOnBlockingCeiling()
{

    if(mState==State::ASCENDING_HIGH_SPEED){
        mSpeed = -mSpeed;
        mState = State::DESCENDING_HIGH_SPEED;

    }else if(mState==State::ASCENDING_LOW_SPEED){
        mSpeed = -mSpeed;
        mState = State::DESCENDING_LOW_SPEED;
    }

}


/*
void JumpSpeedGenerator::switchToHighSpeedDescending()
{

    mDist = mInterHeight;
    mSpeed = -mInterSpeed;
    mState = State::DESCENDING_HIGH_SPEED;

}
*/


void JumpSpeedGenerator::setDistToCeiling(float _distToCeiling)
{

    mDistToCeiling = _distToCeiling;

     if(mState==State::ASCENDING_HIGH_SPEED){

         // adjust acceleration

         mAccHighSpeed = (mSpeed*mSpeed)/(2.0f * _distToCeiling);

     }

}


float JumpSpeedGenerator::updateVelocity(float _timeStep)
{

    float dt = _timeStep;


    if(mState==State::ASCENDING_HIGH_SPEED){

        mSpeed -= mAccHighSpeed * dt;
        mDist += mSpeed * dt;

        if(mDist >= mInterHeight){
            float distAdj = mSpeed * dt / 2.0f;
            float distance = mHeightMax - mInterHeight + distAdj;
            mAccLowSpeed = std::abs((mSpeed*mSpeed)/(2.0f * distance));

            mState = State::ASCENDING_LOW_SPEED;

        }else if(mSpeed<=0.0){
            mState = State::AT_MAX_HEIGHT;
        }


    }else if(mState==State::ASCENDING_LOW_SPEED){

        mSpeed -= mAccLowSpeed * dt;
        mDist += mSpeed * dt;

        if(mSpeed<=0.0){
            mState = State::AT_MAX_HEIGHT;
            //mDist = mHeightMax;
            //mSpeed = 0.0f;
        }


    }else if(mState==State::AT_MAX_HEIGHT){

        mState = State::DESCENDING_LOW_SPEED;


    }else if(mState==State::DESCENDING_LOW_SPEED){

        mSpeed -= mAccLowSpeed * dt;
        mDist += mSpeed * dt;

        if(mDist <= mInterHeight){
            mState = State::DESCENDING_HIGH_SPEED;
        }


    }else if(mState==State::DESCENDING_HIGH_SPEED){

        mSpeed -= mAccHighSpeed * dt;
        mDist += mSpeed * dt;

        if(mSpeed<= -finalFreefallSpeed){
            mState = State::DESCENDING_FINAL_SPEED;
        }


    }else if(mState==State::DESCENDING_FINAL_SPEED){

        mDist += mSpeed * dt;


   }else if(mState==State::ON_GROUND){
        mSpeed = 0.0f;

    }

    //dbgPrint();


    return mSpeed;

}


void JumpSpeedGenerator::stop()
{
    mSpeed = 0.0f;
    mDistToCeiling = 0.0f;
    mState = State::ON_GROUND;
}



void JumpSpeedGenerator::dbgPrint()
{

    if(mState==State::ASCENDING_HIGH_SPEED){
        print("ASCENDING_HIGH_SPEED");

    }else if(mState==State::ASCENDING_LOW_SPEED){
        print("ASCENDING_LOW_SPEED");

    }else if(mState==State::AT_MAX_HEIGHT){
        print("EJumping AT_MAX_HEIGHT");

    }else if(mState==State::DESCENDING_LOW_SPEED){
        print("DESCENDING_LOW_SPEED");

    }else if(mState==State::DESCENDING_HIGH_SPEED){
        print("DESCENDING_HIGH_SPEED");

    }else if(mState==State::DESCENDING_FINAL_SPEED){
        print("DESCENDING_TERMINAL_SPEED");

   }else if(mState==State::ON_GROUND){
        print("ON_GROUND");

    }

    print("mSpeed=" + std::to_string(mSpeed) + "  mDist="+ std::to_string(mDist));

}



//============================================================================================



void PointToPointSpeedGenerator::initMovement(float _maxSpeed, float _accDistance, float _distance)
{

    mSpeedMax = mSpeedMaxTarget = _maxSpeed;
    mAccDistance = mAccDistanceTarget = _accDistance;

    if(mAccDistance==0.0f){
        mState = State::FULL_SPEED;

    }else{
        mAcc = (mSpeedMax*mSpeedMax)/(2.0f * _accDistance);
        mState = State::ACCELERATING;
    }

    mSpeed = 0.0f;
    mDistance = _distance;
    mPassedDistance = 0.0f;
}

/*
void PointToPointSpeedGenerator::continueFullSpeed(float _distance, bool _resetDistance)
{

    assert(mState==State::FINISHED_FULL_SPEED);

    if(_resetDistance){
        mPassedDistance = 0.0f;
        mDistance = _distance;

    }else{

        mDistance += _distance;
    }



    mState = State::FULL_SPEED;

}
*/


void PointToPointSpeedGenerator::continueFullSpeed(float _nextDistance, float _passedDistance)
{

    assert(mState==State::FINISHED_FULL_SPEED);


    mDistance = _nextDistance;
    mPassedDistance = _passedDistance;

    mState = State::FULL_SPEED;

}


/*
float DistancePassingSpeedGenerator::updateSpeed(bool generateSpeed)
{

    float dt = 1.0f / 60.0f;


    if(mState==State::ACCELERATING){

        mSpeed += mAcc * dt;
        //float ds = c += h * v;

        mDist += mSpeed * dt;

        if(mDist>=mDistance){
            mDist = mDistance;
            mSpeed = 0.0f;
            mState = State::STANDING_STILL;

        }else if(mDist>=mAccDistance){
         //}else if(mSpeed>=mSpeedMax){
        //}else if(mDist>=mAccDistance){
            //float dSpeedOver = mSpeed - mSpeedMax;
            //float dtOver = dSpeedOver * std::cos(mAngle);



        //}else if(speed>=mSpeedMax){
            //mTimeMovementStartMS = mTimeCurrentMS;
            //speed = mSpeedMax;
            mSpeedMax = mSpeed;
            mAccDistance = mDist;
            //mAcc = (mSpeedMax*mSpeedMax)/(2.0f * mDist);
            mState = State::FULL_SPEED;
        }

        //print("ACCELERATING");

    }else if(mState==State::FULL_SPEED){

        mSpeed = mSpeedMax;
        //mDist = mAccDistance + speed * mdtS;
        mDist += mSpeed * dt;

        if(mDist>=mDistance){
            mDist = mDistance;
            mSpeed = 0.0f;
            mState = State::STANDING_STILL;

        }else if(mDist>=mDistance-mAccDistance){
            //mTimeMovementStartMS = mTimeCurrentMS;
            //mAccDistNew = mDistance-mDist;
            //mAcc = (mSpeedMax*mSpeedMax)/(2.0f * mAccDistNew);

            float distAdj = mSpeed * dt / 2.0f;
            mAcc = (mSpeed*mSpeed)/(2.0f * (mAccDistance + distAdj));

            mState = State::DECELERATING;
        }

        //print("FULL SPEED");

    }else if(mState==State::DECELERATING){

        mSpeed -= mAcc * dt;
        mDist += mSpeed * dt;

        //mDist = (mDistance - mAccDistance) + (0.5 * mAcc * mdtS * mdtS);

        if(mDist>=mDistance || mSpeed<=0.0f){
            //mDist = mDistance;
            mSpeed = 0.0f;
            mState = State::STANDING_STILL;
        }

        //print("DECELERATING");

    }else if(mState==State::STANDING_STILL){
        mSpeed = 0.0f;
    }


    dbgPrint();


    return mSpeed;

}
*/


float PointToPointSpeedGenerator::updateSpeed(float _timeStep, bool _finishFullSpeed)
{

    float dt = _timeStep;


    if(mState==State::ACCELERATING){

        mSpeed += mAcc * dt;
        mPassedDistance += mSpeed * dt;

        if(mPassedDistance>=mDistance){

            if(_finishFullSpeed){
                mState = State::FINISHED_FULL_SPEED;
            }else{
                mPassedDistance = mDistance;
                mSpeed = 0.0f;
                mState = State::STANDING_STILL;
            }

        }else if(mPassedDistance>=mAccDistance){

            mSpeedMax = mSpeed;
            mAccDistance = mPassedDistance;
            mState = State::FULL_SPEED;
        }

    }else if(mState==State::FULL_SPEED){

        mSpeed = mSpeedMax;
        mPassedDistance += mSpeed * dt;

        if(mPassedDistance>=mDistance){

            if(_finishFullSpeed){
                mState = State::FINISHED_FULL_SPEED;
            }else{
                mPassedDistance = mDistance;
                mSpeed = 0.0f;
                mState = State::STANDING_STILL;
            }

        }else if(mPassedDistance>=mDistance-mAccDistance){
            if(_finishFullSpeed==false){

                float dOver = mPassedDistance - (mDistance-mAccDistance);

                float distAdj = mSpeed * dt / 2.0f;
                //distAdj = mSpeed * dt;
                mAcc = (mSpeed*mSpeed)/(2.0f * (mAccDistance + distAdj));
                //mAcc = (mSpeed*mSpeed)/(2.0f * (mDistance - mPassedDistance));
                //mAcc = 0.0f;
                //mAccDistance = mAccDistance - dOver;
                //mAcc = (mSpeed*mSpeed)/(2.0f * (mAccDistance));

                mState = State::DECELERATING;
            }
        }

    }else if(mState==State::DECELERATING){

        mSpeed -= mAcc * dt;
        mPassedDistance += mSpeed * dt;

        if(mPassedDistance>=mDistance || mSpeed<=0.0f){
            //print ("mSpeed = "+ std::to_string(mSpeed) + " overDistance = " + std::to_string(mPassedDistance-mDistance));
            mPassedDistance = mDistance;
            mSpeed = 0.0f;
            mState = State::STANDING_STILL;
        }


    }else if(mState==State::STANDING_STILL){
        mSpeed = 0.0f;


    }else if(mState==State::FINISHED_FULL_SPEED){

    }


    //dbgPrint();


    return mSpeed;

}


void PointToPointSpeedGenerator::stop()
{
    mState = State::STANDING_STILL;
    mSpeed = 0.0;

    //print("SpeedGenerator '" + dbgName + "' stop!");

}


void PointToPointSpeedGenerator::dbgPrint()
{

    if(mState==State::ACCELERATING){
        print("ACCELERATING");

    }else if(mState==State::FULL_SPEED){
       print("FULL SPEED");

    }else if(mState==State::DECELERATING){
        print("DECELERATING");

    }else if(mState==State::STANDING_STILL){
        print("STANDING_STILL");
    }
    print("mSpeed="+ std::to_string(mSpeed));

}






}
