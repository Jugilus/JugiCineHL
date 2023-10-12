#ifndef PLAYER__MOVEMENT__ANGULAR_VELOCITY__H
#define PLAYER__MOVEMENT__ANGULAR_VELOCITY__H

#include <memory>
#include <string>
#include <vector>

#include "jpEntityGlobal.h"
#include "jpSpeedGenerators.h"


namespace pugi{
class xml_node;
}

namespace jugimap{




struct RotationData
{

    float maxSpeed = 0.0f;                      // degrees per second
    float accDistance = 0.0f;                   // degrees, seconds
    float startAngle = 0.0f;                    // degrees
    float endAngle = 0.0f;                      // degrees
    bool continousCircularRotation = false;

};





class AngularVelocityPtP
{
public:

    void init(float _startAngle, float _endAngle, bool _continousCircularRotation, RotationDirection _startDirection);
    void initMovement(float _maxSpeed, float _accDistance);

    float updateAngularVelocity(float _timeStep);

    bool isActive(){ return mActive; }
    float currentAngle(){ return mCurrentAngle; }
    float startAngle(){ return mStartAngle; }
    float endAngle(){ return mEndAngle; }


private:
    bool mActive = false;
    PointToPointSpeedGenerator mSpeedGenerator;
    float mStartAngle = 0.0f;                       // degrees
    float mEndAngle = 0.0f;                         // degrees

    float mCurrentStartAngle = 0.0f;
    float mCurrentEndAngle = 0.0f;
    float mCurrentAngle = 0.0f;                     // degrees

    float mDistance = 0.0f;                         // degrees
    bool mContinousCircularMovement = false;
    float mSpeed = 0.0f;                            // degrees, seconds
    RotationDirection mStartRotationDirection = RotationDirection::NONE;
    RotationDirection mRotationDirection = RotationDirection::NONE;




};







}






#endif
