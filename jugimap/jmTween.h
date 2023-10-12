#ifndef JM_EASING_H
#define JM_EASING_H

#include<string>
#include "jmGlobal.h"




namespace jugimap {




/*

/// \brief The Easing struct defines interpolation methods used for obtaining values between a start value and an end value. It is used in
/// timeline animations and tweens.
class Easing
{
public:

    /// \brief The kind of Easing.
    enum class Kind : int
    {
        LINEAR = 0,             ///< Linear transition from the start value to the end value.
        EASE_START = 1,         ///< Ease-out from the start value (quadratic interpolation).
        EASE_END = 2,           ///< Ease-in into the end value.
        EASE_START_END = 3,     ///< Ease-out from the start value and ease-in into the end value.
        CONSTANT = 4            ///< Constant value; the same as the start value.
    };


    /// Returns an ouput value according to the Easing::Kind. The given parameter **p** must be in range from 0.0 to 1.0.
    float GetValue(float p);

    EasingKind kind = EasingKind::LINEAR;

};
*/



/// \brief The Easing struct defines interpolation methods used for obtaining values between a start value and an end value. It is used in
/// timeline animations and tweens.
class Easing
{
public:

    /// Returns an ouput value according to the Easing::Kind. The given parameter **p** must be in range from 0.0 to 1.0.
    float GetValue(float p);

    EasingKind kind = EasingKind::LINEAR;
};



//================================================================================================================================



/// \brief The Tween class defines transition between two values over time.
class Tween
{
public:
    friend class Tweens;

    /// The tween mode.
    enum class Mode
    {
        NORMAL,             ///< The tween runs from the starting value to the end value and then stops.
        REVERSE,            ///< The tween runs from the end value to the start value and then stops.
        LOOP,               ///< The tween runs in loops from the start value to the end value.
        LOOP_REVERSE        ///< The tween runs in loops from the start value to the end value and reverse direction on each loop for smooth transitions.
    };

    //~Tween();


    /// \brief Initialize the parameters of this tween.
    ///
    /// \param _valueStart The starting value.
    /// \param _valueEnd The end value.
    /// \param _durationS Duration of one loop in seconds.
    /// \param _easingKind The easing kind.
    void Init(float _valueStart, float _valueEnd, float _durationS, EasingKind _easingKind);


    /// \brief Start running this tween.
    void Play();


    /// \brief Updates the tween value and returns it.
    ///
    /// This function must be called inside the application update loop.
    float Update();


    /// \brief Returns true if this tween is not running; otherwise return false.
    bool IsIdle() { return state==stateIDLE || state == stateNEVER_INITIALIZED; }


    /// \brief Stop running the tween.
    void Stop();


    /// \brief Pause running the tween.
    ///
    /// \see Resume
    void Pause();


    /// \brief Resume running the tween.
    ///
    /// \see Pause
    void Resume();


    /// \brief Set the easing kind of this tween.
    void SetEasingKind(EasingKind _kind) { easing.kind = _kind; }


    /// \brief Set the mode of this tween.
    void SetMode(Mode _mode) { mode = _mode;}


    /// \brief Returns the starting value of this tween;
    float GetStartValue(){ return valueStart;}


    /// \brief Returns the end value of this tween;
    float GetEndValue(){ return valueEnd;}




private:
    float value = 0;
    float valueStart = 0.0;
    float valueEnd = 0.0;
    int timeStartMS = 0;
    int timeEndMS = 0;
    int durationMS = 0;
    Easing easing;

    int state = stateNEVER_INITIALIZED;
    static const int stateNEVER_INITIALIZED = -1;
    static const int stateIDLE = 0;
    static const int statePLAYING = 1;
    static const int statePAUSED = 3;


    Mode mode = Mode::NORMAL;
    bool reverse = false;

    int stateStored = 0;
    int msTimeStored = 0;


};




}


#endif // JMEASING_H
