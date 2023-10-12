#include <assert.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <string>

#include "jmSystem.h"
#include "jmTimer.h"




namespace jugimap {



void Timer::start(float _durationS, Mode _mode)
{

    mDurationMS = _durationS*1000;
    mTimeStartMS = time.GetPassedNetTimeMS();
    mTimeEndMS = mTimeStartMS + mDurationMS;
    mMode = _mode;
    mState = State::RUNNING;
}


Timer::State Timer::update()
{

    if(mState==State::RUNNING){

        int timePassedMS = time.GetPassedNetTimeMS();

        if(timePassedMS>=mTimeEndMS){

            if(mMode==Mode::ONE_SHOT){
                mState = State::IDLE;

            }else if(mMode==Mode::AUTO_RESTART){
                mTimeStartMS = timePassedMS;
                mTimeEndMS = mTimeStartMS + mDurationMS;
            }

            return State::INTERVAL_PASSED;
        }
    }

    return mState;

}


int Timer::elapsed()
{

    if(mState==State::RUNNING){
        return time.GetPassedNetTimeMS()-mTimeStartMS;
    }

    return 0;
}



void Timer::Stop()
{
    mState = State::IDLE;
}


void Timer::Pause()
{
    if(mState!=State::RUNNING) return;

    mStateStored = mState;
    mTimeStoredMS = time.GetPassedNetTimeMS();
    mState = State::PAUSED;
}


void Timer::Resume()
{
    if(mState!=State::PAUSED) return;

    mState = mStateStored;
    mTimeStartMS += time.GetPassedNetTimeMS() - mTimeStoredMS;
    mTimeEndMS += time.GetPassedNetTimeMS() - mTimeStoredMS;
}







}
