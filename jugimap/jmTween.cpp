#include <assert.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <string>

#include "jmSystem.h"
#include "jmTween.h"




namespace jugimap {



float Easing::GetValue(float p)
{

    static const float pi = 3.1415926;

    switch (kind)
    {
    case EasingKind::LINEAR:
        return p;

    case EasingKind::EASE_START_QUAD:
        return p*p;

    case EasingKind::EASE_END_QUAD:
        return  1.0-(1.0-p)*(1.0-p);

    case EasingKind::EASE_START_END_QUAD:
        return p < 0.5 ? 2*p*p : 1.0 - (2.0-2.0*p)*(2.0-2.0*p)/2.0;

    case EasingKind::EASE_START_CUBIC:
        return p*p*p;

    case EasingKind::EASE_END_CUBIC:
        return  1.0-(1.0-p)*(1.0-p)*(1.0-p);

    case EasingKind::EASE_START_END_CUBIC:
        return p < 0.5 ? 4*p*p*p : 1.0 - (2.0-2.0*p)*(2.0-2.0*p)*(2.0-2.0*p)/2.0;

    case EasingKind::EASE_START_SINE:
        return 1.0 - std::cos(p*pi/2.0);

    case EasingKind::EASE_END_SINE:
        return  std::sin(p*pi/2.0);;

    case EasingKind::EASE_START_END_SINE:
        return -(std::cos(p*pi)-1.0)/2.0;

    case EasingKind::CONSTANT:
        return 0;
    }

    return p;
}


//=============================================================================================



void Tween::Init(float _valueStart, float _valueEnd, float _durationS, EasingKind _easingKind)
{

    valueStart = _valueStart;
    valueEnd = _valueEnd;
    durationMS = _durationS * 1000;
    timeStartMS = time.GetPassedNetTimeMS();
    timeEndMS = timeStartMS + durationMS;
    easing.kind = _easingKind;

    //state = stateIDLE;
    mState = TweenState::IDLE;
}


void Tween::Play(float _valueStart, float _valueEnd)
{

    if(mState == TweenState::NEVER_INITIALIZED){
        return;
    }

    valueStart = _valueStart;
    valueEnd = _valueEnd;
    timeStartMS = time.GetPassedNetTimeMS();
    timeEndMS = timeStartMS + durationMS;
    mState = TweenState::PLAYING;
    reverse = false;

}



void Tween::Play()
{
    //if(state==stateNEVER_INITIALIZED) return;
    if(mState == TweenState::NEVER_INITIALIZED){
        return;
    }

    timeStartMS = time.GetPassedNetTimeMS();
    timeEndMS = timeStartMS + durationMS;
    //state = statePLAYING;
    mState = TweenState::PLAYING;
    reverse = false;
}


void Tween::Stop()
{
    //state = stateIDLE;
    mState = TweenState::IDLE;
}


void Tween::Pause()
{
    //if(state!=statePLAYING) return;
    if(mState != TweenState::PLAYING){
        return;
    }

    //stateStored = state;
    mStateStored = mState;
    msTimeStored = time.GetPassedNetTimeMS();
    //state = statePAUSED;
    mState = TweenState::PAUSED;
}


void Tween::Resume()
{
    //if(state!=statePAUSED) return;
    if(mState != TweenState::PAUSED){
        return;
    }

    //state = stateStored;
    mState = mStateStored;
    timeStartMS += time.GetPassedNetTimeMS() - msTimeStored;
    timeEndMS += time.GetPassedNetTimeMS() - msTimeStored;
}


float Tween::Update()
{

    //if(state==statePLAYING){
    if(mState==TweenState::PLAYING){

        float p = (time.GetPassedNetTimeMS()-timeStartMS)/float(timeEndMS-timeStartMS);

        if(p>1.0f){

            p = 0.0f;

            if(mode==Mode::LOOP){
                timeStartMS = time.GetPassedNetTimeMS();
                timeEndMS = timeStartMS + durationMS;

            }else if(mode==Mode::LOOP_REVERSE){
                timeStartMS = time.GetPassedNetTimeMS();
                timeEndMS = timeStartMS + durationMS;
                reverse = !reverse;

            }else{
                value = valueEnd;
                if(mode==Mode::REVERSE){
                    value = valueStart;
                }
                //state = stateIDLE;
                mState = TweenState::IDLE;
                return value;
            }
        }

        if(reverse){
            p = 1.0f - p;
        }

        if(mode==Mode::REVERSE){
            p = 1.0f - p;
        }

        p = easing.GetValue(p);
        value = valueStart + p*(valueEnd - valueStart);

    }

    return value;
}




}
