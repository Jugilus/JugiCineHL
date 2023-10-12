#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmCommonFunctions.h"
#include "jmSystem.h"

#include "jpPlayedApp.h"
#include "jpUtilities.h"

#include "jpB2World.h"
#include "jpAngularVelocity.h"



namespace jugimap{



void AngularVelocityPtP::init(float _startAngle, float _endAngle, bool _continousCircularRotation, RotationDirection _startDirection)
{

    mStartAngle = _startAngle;
    while(mStartAngle > 360.0f){
        mStartAngle -= 360.0f;
    }
    //while(mStartAngle < 0.0f){
    //    mStartAngle += 360.0f;
    //}

    mEndAngle = _endAngle;
    while(mEndAngle > 360.0f){
        mEndAngle -= 360.0f;
    }
    //while(mEndAngle < 0.0f){
    //    mEndAngle += 360.0f;
    //}


    mContinousCircularMovement = _continousCircularRotation;
    mStartRotationDirection = _startDirection;

    if(mContinousCircularMovement){
        mDistance = 360.0f;
        mStartAngle = 0.0f;     // doesnt matter
        mEndAngle = 360.0f;

    }else{

        mDistance = std::abs(mEndAngle - mStartAngle);

        if(mEndAngle > mStartAngle)        // CCW
        {
            if(mStartRotationDirection==RotationDirection::CW){
                mDistance = 360.0f - mDistance;
            }

        }else if(mEndAngle < mStartAngle)       //CW
        {
            if(mStartRotationDirection==RotationDirection::CCW){
                mDistance = 360.0f - mDistance;
            }
        }
    }

    mCurrentAngle = mStartAngle;

    mActive = (mDistance > 0.0f);

}


void AngularVelocityPtP::initMovement(float _maxSpeed, float _accDistance)
{

    //float accDistance = _data.accDistance;
    //if(mDistance == 360.0f){
    //    _accDistance = 0.0f;
    //}

    mSpeedGenerator.initMovement(_maxSpeed, _accDistance, mDistance);

    if(mContinousCircularMovement){

        mRotationDirection = mStartRotationDirection;
        mCurrentStartAngle = mStartAngle;
        mCurrentEndAngle = mEndAngle;

    }else{
        if(mCurrentAngle == mStartAngle){
            mRotationDirection = mStartRotationDirection;
            mCurrentStartAngle = mStartAngle;
            mCurrentEndAngle = mEndAngle;

        }else if(mCurrentAngle == mEndAngle){
            mRotationDirection = (mStartRotationDirection==RotationDirection::CW)? RotationDirection::CCW :  RotationDirection::CW;
            mCurrentStartAngle = mEndAngle;
            mCurrentEndAngle = mStartAngle;
        }
    }


}



float AngularVelocityPtP::updateAngularVelocity(float _timeStep)
{

    mSpeed = 0.0f;

    if(mContinousCircularMovement){
        mSpeed = mSpeedGenerator.updateSpeed(_timeStep, true);

        if(mSpeedGenerator.state()==PointToPointSpeedGenerator::State::FINISHED_FULL_SPEED){

            float passedDistance = mSpeedGenerator.passedDistance();
            float mOverDistance = passedDistance - mDistance;
            mSpeedGenerator.continueFullSpeed(mDistance, mOverDistance);
        }

    }else{

        mSpeed = mSpeedGenerator.updateSpeed(_timeStep, false);

    }


    int sgn = (mRotationDirection==RotationDirection::CW)? -1 : 1;
    mCurrentAngle = mCurrentStartAngle + mSpeedGenerator.passedDistance() * sgn;
    mSpeed *= sgn;

    //---
    if(mSpeedGenerator.state()==PointToPointSpeedGenerator::State::STANDING_STILL){
        mCurrentAngle = mCurrentEndAngle;
        mSpeed = 0.0f;
    }

    //print("mCurrentAngle = " + std::to_string(mCurrentAngle) + "  mCurrentStartAngle = " + std::to_string(mCurrentStartAngle) + "  mCurrentEndAngle = " + std::to_string(mCurrentEndAngle));

    // deg -> radians

    mSpeed *= b2_pi/180.0f;


    return mSpeed;

}



}
