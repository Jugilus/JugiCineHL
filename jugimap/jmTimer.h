#ifndef JM_TIMER_H
#define JM_TIMER_H

#include<string>
#include "jmGlobal.h"




namespace jugimap {





//================================================================================================================================



/// \brief The Tween class defines transition between two values over time.
class Timer
{
public:

    /// The tween mode.
    enum class Mode
    {
        ONE_SHOT,             ///< The tween runs from the starting value to the end value and then stops.
        AUTO_RESTART,               ///< The tween runs in loops from the start value to the end value.
    };

    enum class State
    {
        IDLE,
        RUNNING,
        INTERVAL_PASSED,
        PAUSED
    };


    void start(float _durationS, Mode _mode);

    State update();

    int elapsed();

    State state() { return mState; }

    void Stop();

    void Pause();

    void Resume();


private:
    int mTimeStartMS = 0;
    int mTimeEndMS = 0;
    int mDurationMS = 0;

    Mode mMode = Mode::ONE_SHOT;
    State mState = State::IDLE;
    State mStateStored = State::IDLE;

    int mTimeStoredMS = 0;


};




}


#endif // JMEASING_H
