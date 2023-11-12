#ifndef PLAYER__ACTION_TRANSITION_OPERATIONS_H
#define PLAYER__ACTION_TRANSITION_OPERATIONS_H


#include <string>
#include <vector>

#include <jmTween.h>
#include <jmTimer.h>
//#include "jpTransition.h"

#include "jpGlobal.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

class PlayedScene;
class Transition;
class TransitionOp;
class TransitionMember;




struct TransitionOpCfg
{

    virtual ~TransitionOpCfg(){}

    virtual bool initCfg(const pugi::xml_node &_node);


    std::string transitionOp;

    //TransitionType transitionType = TransitionType::NOT_DEFINED;
    float durationS = 1.0f;
    float delayS = 0.0f;
    Timer delayTimer;

    //---
    std::string direction;

    //---
    float alphaMin = 0.0f;
    float alphaMax = 1.0f;

    //---
    float xScaleMin = 0.0f;
    float yScaleMin = 0.0f;

};




class TransitionOp
{
public:


    TransitionOp(TransitionMember*_parentObject, const pugi::xml_node &_node, TransitionType _transitionType);

    TransitionOp(TransitionMember*_parentObject, TransitionOpCfg *_cfg, TransitionType _transitionType);

    virtual ~TransitionOp(){}

    virtual bool initConnections(PlayedScene *_scene) = 0;

    virtual void onTransitionStarted() = 0;
    virtual void execute() = 0;
    virtual void update() = 0;

    TransitionType transitionType(){ return  mTransitionType; }
    ActionStatus status(){ return mStatus; }


protected:

    ActionStatus mStatus = ActionStatus::IDLE;

    TransitionType mTransitionType = TransitionType::NOT_DEFINED;
    float mDurationS = 1.0f;
    float mDelayS = 0.0f;
    Timer mDelayTimer;

    //--- affected objects
    TransitionMember* mParentTransitionMember = nullptr;                    // LINK

    float mDurationMin = 0.1f;
    float mDurationMax = 5.0f;

};





class TranslateInOutOfScreen : public TransitionOp
{
public:
    static std::string typeIn;
    static std::string typeOut;


    enum class Direction
    {
        TO_LEFT,
        TO_RIGHT,
        TO_TOP,
        TO_BOTTOM,

        FROM_LEFT,
        FROM_RIGHT,
        FROM_TOP,
        FROM_BOTTOM,

        NOT_DEFINED
    };

    enum class Axis
    {
        NOT_DEFINED,
        X,
        Y
    };


    TranslateInOutOfScreen(TransitionMember *_parentObject, const pugi::xml_node &_node, TransitionType _translateType);

    TranslateInOutOfScreen(TransitionMember *_parentObject, TransitionOpCfg *_cfg, TransitionType _transitionType);



    bool initConnections(PlayedScene *_scene) override;
    void onTransitionStarted() override;
    void execute() override;
    void update() override;


protected:

    //--- properties
    std::string mDirectionName;
    Direction mDirection = Direction::NOT_DEFINED;


    //--- logic
    Axis mAxis = Axis::NOT_DEFINED;
    float mTranslation = 0.0f;
    float mStartTranslation = 0.0f;
    Tween mTween;


    static Direction directionFromString(const std::string &_direction);


};




class AlphaFadeInOut : public TransitionOp
{
public:
    static std::string typeIn;
    static std::string typeOut;



    AlphaFadeInOut(TransitionMember *_parentObject, const pugi::xml_node &_node, TransitionType _transitionType);

    AlphaFadeInOut(TransitionMember *_parentObject, TransitionOpCfg *_cfg, TransitionType _transitionType);


    bool initConnections(PlayedScene *_scene) override;
    void onTransitionStarted() override;
    void execute() override;
    void update() override;


private:

    //--- logic
    float mAlphaMin = 0.0f;
    float mAlphaMax = 1.0f;
    float mStartAlpha = 0.0f;
    Tween mTween;



};


//------------------------------------------------------------------------------------------------


class ScaleInOut : public TransitionOp
{
public:
    static std::string typeIn;
    static std::string typeOut;



    ScaleInOut(TransitionMember *_parentObject, const pugi::xml_node &_node, TransitionType _transitionType);

    ScaleInOut(TransitionMember *_parentObject, TransitionOpCfg *_cfg, TransitionType _transitionType);


    bool initConnections(PlayedScene *_scene) override;
    void onTransitionStarted() override;
    void execute() override;
    void update() override;


private:
    //--- logic
    Vec2f mScaleMin;
    Tween mTweenX;
    Tween mTweenY;

};



}


#endif // PLAYER__ACTION_TRANSITION_OPERATIONS_H
