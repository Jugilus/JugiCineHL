#include "pugixml/pugixml.hpp"
#include "jmSprite.h"
#include "jmSystem.h"
#include "jmAnimationCommon.h"
#include "map/jmSpriteLayer.h"
#include "map/jmMap.h"
#include "jmSystem.h"
#include "jmGlobal.h"
#include "jmScene.h"
#include "jmSceneLayout.h"
#include "jmUtilities.h"
#include "jmCommonFunctions.h"

#include "jpUtilities.h"
#include "jpObjectFactory.h"
#include "jpTransition.h"
#include "jpTransitionOperations.h"



namespace jugimap{




bool TransitionOpCfg::initCfg(const pugi::xml_node &_node)
{

    transitionOp = std::string(_node.name());


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName = std::string(a.name());

        if(attributeName=="duration"){
            durationS = a.as_float(durationS);

        }else if(attributeName=="delay"){
            delayS = a.as_float(delayS);

        }else if(attributeName=="direction"){
            direction = a.as_string();

        }else if(attributeName=="alphaMin"){
            alphaMin = a.as_float(alphaMin);

        }else if(attributeName=="alphaMax"){
            alphaMax = a.as_float(alphaMax);

        }else if(attributeName=="xScaleMin"){
            xScaleMin = a.as_float(alphaMax);

        }else if(attributeName=="yScaleMin"){
            yScaleMin = a.as_float(alphaMax);

        }else{
            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + transitionOp +"' !");
            return false;
        }


    }

    return true;
}


//--------------------------------------------------------------------------------------------------


TransitionOp::TransitionOp(TransitionMember *_parentObject, const pugi::xml_node &_node, TransitionType _transitionType)
{

    mParentTransitionMember = _parentObject;
    mTransitionType = _transitionType;
    mDurationS = _node.attribute("duration").as_float(1.0f);
    mDurationS = ClampValue(mDurationS, mDurationMin, mDurationMax);
    mDelayS = _node.attribute("delay").as_float(0.0f);
    mDelayS = ClampValue(mDelayS, 0.0f, mDurationMax);

}


TransitionOp::TransitionOp(TransitionMember*_parentObject, TransitionOpCfg *_cfg, TransitionType _transitionType)
{

    mParentTransitionMember = _parentObject;
    mTransitionType = _transitionType;
    mDurationS = _cfg->durationS;
    mDurationS = ClampValue(mDurationS, mDurationMin, mDurationMax);
    mDelayS = _cfg->delayS;
    mDelayS = ClampValue(mDelayS, 0.0f, mDurationMax);

}


//--------------------------------------------------------------------------------------------------



std::string TranslateInOutOfScreen::typeIn = "translateIntoScreen";
std::string TranslateInOutOfScreen::typeOut = "translateOutOfScreen";


TranslateInOutOfScreen::TranslateInOutOfScreen(TransitionMember *_parentObject, const pugi::xml_node &_node, TransitionType _translateType)
    : TransitionOp(_parentObject, _node, _translateType)
{

    mDirectionName = _node.attribute("direction").as_string("");
}


TranslateInOutOfScreen::TranslateInOutOfScreen(TransitionMember *_parentObject, TransitionOpCfg *_cfg, TransitionType _transitionType)
    : TransitionOp(_parentObject, _cfg, _transitionType)
{

    mDirectionName = _cfg->direction;
}


bool TranslateInOutOfScreen::initConnections(PlayedScene *_scene)
{

    std::vector<void*> &objects = mParentTransitionMember->objects();
    GfxObjectType objectTypeId = mParentTransitionMember->objectType();


    if(mTransitionType==TransitionType::OUT){
        dbgSystem.addMessage("Initializing '" + typeOut + "' ...");

    }else if(mTransitionType==TransitionType::IN){
        dbgSystem.addMessage("Initializing '" + typeIn + "' ...");
    }

    mDirection = directionFromString(mDirectionName);

    if((mTransitionType==TransitionType::OUT && !(mDirection==Direction::TO_LEFT || mDirection==Direction::TO_RIGHT || mDirection==Direction::TO_TOP || mDirection==Direction::TO_BOTTOM )) ||
       (mTransitionType==TransitionType::IN && !(mDirection==Direction::FROM_LEFT || mDirection==Direction::FROM_RIGHT || mDirection==Direction::FROM_TOP || mDirection==Direction::FROM_BOTTOM ))
    ){

        dbgSystem.addMessage("Wrong direction '" + mDirectionName + "'");
        return false;
    }


    //--- obtain required translation from bounding box and screen size

    BoundingBoxFloat bb;
    bb.setInitialSize();

    if(objectTypeId==GfxObjectType::SPRITE){

        for(void* obj : objects){
            Sprite * s = static_cast<Sprite*>(obj);
            bb.Unite(s->boundingBox());
        }

    }else if(objectTypeId==GfxObjectType::SPRITE_LAYER){

        for(void* obj : objects){
            SpriteLayer * spriteLayer = static_cast<SpriteLayer*>(obj);
            //spriteLayer->updateBoundingBox();
            bb.Unite(spriteLayer->worldBoundingBox());
        }


    }else if(objectTypeId==GfxObjectType::MAP){


    }

    //---

    mStartTranslation = 0.0f;
    mTranslation = 0.0f;

    if(mDirection==Direction::TO_LEFT){
        mAxis = Axis::X;
        mTranslation = - bb.max.x;

    }else if(mDirection==Direction::TO_RIGHT){
        mAxis = Axis::X;
        mTranslation = jugimap::settings.GetScreenSize().x - bb.min.x;

    }else if(mDirection==Direction::TO_TOP){
        mAxis = Axis::Y;
        mTranslation = jugimap::settings.GetScreenSize().y - bb.min.y;

    }else if(mDirection==Direction::TO_BOTTOM){
        mAxis = Axis::Y;
        mTranslation = - bb.max.y;

    }else if(mDirection==Direction::FROM_LEFT){
        mAxis = Axis::X;
        mStartTranslation = - bb.max.x;
        mTranslation = -mStartTranslation;

    }else if(mDirection==Direction::FROM_RIGHT){
        mAxis = Axis::X;
        mStartTranslation = jugimap::settings.GetScreenSize().x - bb.min.x;
        mTranslation = -mStartTranslation;

    }else if(mDirection==Direction::FROM_BOTTOM){
        mAxis = Axis::Y;
        mStartTranslation = - bb.max.y;
        mTranslation = -mStartTranslation;

    }else if(mDirection==Direction::FROM_TOP){
        mAxis = Axis::Y;
        mStartTranslation = jugimap::settings.GetScreenSize().y - bb.min.y;
        mTranslation = -mStartTranslation;

    }

    mTween.Init(0.0f, mTranslation, mDurationS, jugimap::EasingKind::EASE_END_QUAD);

    //---
    //reset();

    //---
    dbgSystem.removeLastMessage();
    return true;

}


void TranslateInOutOfScreen::onTransitionStarted()
{

    std::vector<void*> &mObjects = mParentTransitionMember->objects();
    GfxObjectType mObjectType = mParentTransitionMember->objectType();


    Vec2f translation(0.0f, 0.0f);
    if(mAxis==Axis::X){
        translation.x = mStartTranslation;
    }else if(mAxis==Axis::Y){
        translation.y = mStartTranslation;
    }


    if(mObjectType==GfxObjectType::SPRITE){

        for(void* obj : mObjects){
            Sprite * s = static_cast<Sprite*>(obj);
            s->createAnimatedPropertiesIfNone();
            s->animatedProperties()->translation = translation;
            s->updatePosition();
            s->setVisible(true);
        }

    }else if(mObjectType==GfxObjectType::SPRITE_LAYER){

        for(void* obj : mObjects){
            SpriteLayer * sl = static_cast<SpriteLayer*>(obj);

            //---
            for(jugimap::Sprite* s : sl->sprites()){
                s->createAnimatedPropertiesIfNone();
                s->animatedProperties()->translation = translation;
                s->updatePosition();
            }

            sl->setVisible(true);
        }

    }else if(mObjectType==GfxObjectType::MAP){


    }

    mTween.Stop();

    mStatus = ActionStatus::IDLE;
}


void TranslateInOutOfScreen::execute()
{

    onTransitionStarted();

    if(mDelayS>0.01){
        mDelayTimer.start(mDelayS, Timer::Mode::ONE_SHOT);
    }else{
        mTween.Play();
    }
    mStatus = ActionStatus::STARTED;

}


void TranslateInOutOfScreen::update()
{

    std::vector<void*> &mObjects = mParentTransitionMember->objects();
    GfxObjectType mObjectType = mParentTransitionMember->objectType();


    if(mStatus==ActionStatus::IDLE) return;

    if(mStatus==ActionStatus::STARTED){
        mStatus = ActionStatus::ACTIVE;
    }


    if(mStatus==ActionStatus::ACTIVE){

        //--- manage delay
        if(mDelayTimer.state()==Timer::State::RUNNING){
            if(mDelayTimer.update()==Timer::State::INTERVAL_PASSED){
                mTween.Play();
            }else{
                return;
            }
        }


        //---
        float delta = mTween.Update();

        Vec2f translation(0.0f, 0.0f);
        if(mAxis==Axis::X){
            translation.x = mStartTranslation + delta;
        }else if(mAxis==Axis::Y){
            translation.y = mStartTranslation + delta;
        }


        if(mObjectType==GfxObjectType::SPRITE){

            for(void* obj : mObjects){
                Sprite * s = static_cast<Sprite*>(obj);
                s->animatedProperties()->translation = translation;
                s->updatePosition();
            }

        }else if(mObjectType==GfxObjectType::SPRITE_LAYER){

            for(void* obj : mObjects){
                SpriteLayer * sl = static_cast<SpriteLayer*>(obj);

                //---
                for(jugimap::Sprite* s : sl->sprites()){
                    s->animatedProperties()->translation = translation;
                    s->updatePosition();
                }
            }

        }else if(mObjectType==GfxObjectType::MAP){


        }

        if(mTween.IsIdle()){
            //onTransitionFinished();
            mStatus = ActionStatus::FINISHED;
        }

    }else if(mStatus==ActionStatus::FINISHED){

        mStatus = ActionStatus::IDLE;
    }

}


TranslateInOutOfScreen::Direction TranslateInOutOfScreen::directionFromString(const std::string &_direction)
{

    if(_direction=="toLeft"){
        return Direction::TO_LEFT;

    }else if (_direction=="toRight"){
        return Direction::TO_RIGHT;

    }else if (_direction=="toTop"){
        return Direction::TO_TOP;

    }else if (_direction=="toBottom"){
        return Direction::TO_BOTTOM;


    }else if (_direction=="fromLeft"){
        return Direction::FROM_LEFT;

    }else if (_direction=="fromRight"){
        return Direction::FROM_RIGHT;

    }else if (_direction=="fromTop"){
        return Direction::FROM_TOP;

    }else if (_direction=="fromBottom"){
        return Direction::FROM_BOTTOM;
    }

    return Direction::NOT_DEFINED;

}


//---------------------------------------------------------------------------


std::string AlphaFadeInOut::typeIn = "alphaFadeIn";
std::string AlphaFadeInOut::typeOut = "alphaFadeOut";


AlphaFadeInOut::AlphaFadeInOut(TransitionMember *_parentObject, const pugi::xml_node &_node, TransitionType _transitionType)
    : TransitionOp(_parentObject, _node, _transitionType)
{

    mAlphaMin = _node.attribute("alphaMin").as_float(0.0);
    mAlphaMax = _node.attribute("alphaMax").as_float(1.0);
}


AlphaFadeInOut::AlphaFadeInOut(TransitionMember *_parentObject, TransitionOpCfg *_cfg, TransitionType _transitionType)
    : TransitionOp(_parentObject, _cfg, _transitionType)
{

    mAlphaMin = _cfg->alphaMin;
    mAlphaMax = _cfg->alphaMax;
}


bool AlphaFadeInOut::initConnections(PlayedScene *_scene)
{

    if(mTransitionType==TransitionType::OUT){
        dbgSystem.addMessage("Initializing '" + typeOut + "' ...");

    }else if(mTransitionType==TransitionType::IN){
        dbgSystem.addMessage("Initializing '" + typeIn + "' ...");
    }


    //---
    if(mTransitionType==TransitionType::IN){
        //mStartAlpha = 0.0f;
        //mTween.Init(mStartAlpha, 1.0f, mDurationS, jugimap::EasingKind::EASE_END_QUAD);
        mStartAlpha = mAlphaMin;
        mTween.Init(mStartAlpha, mAlphaMax, mDurationS, jugimap::EasingKind::EASE_END_QUAD);


    }else if(mTransitionType==TransitionType::OUT){
        //mStartAlpha = 1.0f;
       // mTween.Init(mStartAlpha, 0.0f, mDurationS, jugimap::EasingKind::EASE_END_QUAD);
        mStartAlpha = mAlphaMax;
        mTween.Init(mStartAlpha, mAlphaMin, mDurationS, jugimap::EasingKind::EASE_END_QUAD);

        if(mAlphaMin>0.01){
            mParentTransitionMember->_setSkipOnTransitionFinished(true);
        }
    }


    //---
    //reset();

    //---
    dbgSystem.removeLastMessage();
    return true;

}


void AlphaFadeInOut::onTransitionStarted()
{

    std::vector<void*> &objects = mParentTransitionMember->objects();
    GfxObjectType objectTypeId = mParentTransitionMember->objectType();


    if(objectTypeId==GfxObjectType::SPRITE){

        for(void* obj : objects){
            Sprite * s = static_cast<Sprite*>(obj);
            s->createAnimatedPropertiesIfNone();
            s->animatedProperties()->alpha = mStartAlpha;
            //s->setAlpha(mStartAlpha);
            s->setAlpha(s->alpha(false));
            s->setVisible(true);
        }

    }else if(objectTypeId==GfxObjectType::SPRITE_LAYER){

        for(void* obj : objects){
            SpriteLayer * sl = static_cast<SpriteLayer*>(obj);

            //---

            for(jugimap::Sprite* s : sl->sprites()){
                s->createAnimatedPropertiesIfNone();
                s->animatedProperties()->alpha = mStartAlpha;
                s->setAlpha(s->alpha(false));
            }

            //sl->setAlpha(mStartAlpha);

            sl->setVisible(true);
        }

    }else if(objectTypeId==GfxObjectType::MAP){


    }

    mTween.Stop();

    mStatus = ActionStatus::IDLE;
}


void AlphaFadeInOut::execute()
{

    onTransitionStarted();


    if(mDelayS>0.01){
        mDelayTimer.start(mDelayS, Timer::Mode::ONE_SHOT);
    }else{
        mTween.Play();
    }


    mStatus = ActionStatus::STARTED;

}


void AlphaFadeInOut::update()
{

    if(mStatus==ActionStatus::IDLE) return;


    std::vector<void*> &objects = mParentTransitionMember->objects();
    GfxObjectType objectTypeId = mParentTransitionMember->objectType();


    if(mStatus==ActionStatus::STARTED){
        mStatus = ActionStatus::ACTIVE;
    }


    if(mStatus==ActionStatus::ACTIVE){

        //--- manage delay
        if(mDelayTimer.state()==Timer::State::RUNNING){
            if(mDelayTimer.update()==Timer::State::INTERVAL_PASSED){
                mTween.Play();
            }else{
                return;
            }
        }


        //---
        float alpha = mTween.Update();

        if(objectTypeId==GfxObjectType::SPRITE){

            for(void* obj : objects){
                Sprite * s = static_cast<Sprite*>(obj);
                s->animatedProperties()->alpha = alpha;
                s->setAlpha(s->alpha(false));
            }

        }else if(objectTypeId==GfxObjectType::SPRITE_LAYER){

            for(void* obj : objects){
                SpriteLayer * sl = static_cast<SpriteLayer*>(obj);

                //---
                for(jugimap::Sprite* s : sl->sprites()){
                    s->animatedProperties()->alpha = alpha;
                    s->setAlpha(s->alpha(false));
                }
            }

        }else if(objectTypeId==GfxObjectType::MAP){


        }

        if(mTween.IsIdle()){
            if(!(mTransitionType==TransitionType::OUT && mAlphaMin>0.001f)){
                //onTransitionFinished();
            }

            mStatus = ActionStatus::FINISHED;
        }

    }else if(mStatus==ActionStatus::FINISHED){

        mStatus = ActionStatus::IDLE;
    }

}


//---------------------------------------------------------------------------


std::string ScaleInOut::typeIn = "scaleIn";
std::string ScaleInOut::typeOut = "scaleOut";


ScaleInOut::ScaleInOut(TransitionMember* _parentObject, const pugi::xml_node &_node, TransitionType _transitionType)
        : TransitionOp(_parentObject, _node, _transitionType)
{

    mScaleMin.x = _node.attribute("xScaleMin").as_float(0.0);
    mScaleMin.y = _node.attribute("yScaleMin").as_float(0.0);

}



ScaleInOut::ScaleInOut(TransitionMember* _parentObject, TransitionOpCfg *_cfg, TransitionType _transitionType)
        : TransitionOp(_parentObject, _cfg, _transitionType)
{

    mScaleMin.x = _cfg->xScaleMin;
    mScaleMin.y = _cfg->yScaleMin;

}


bool ScaleInOut::initConnections(PlayedScene *_scene)
{


    if(mTransitionType==TransitionType::OUT){
        dbgSystem.addMessage("Initializing '" + typeOut + "' ...");

    }else if(mTransitionType==TransitionType::IN){
        dbgSystem.addMessage("Initializing '" + typeIn + "' ...");
    }


    //---
    if(mTransitionType==TransitionType::IN){
        //mStartAlpha = 0.0f;
        mTweenX.Init(mScaleMin.x, 1.0f, mDurationS, jugimap::EasingKind::EASE_END_QUAD);
        mTweenY.Init(mScaleMin.y, 1.0f, mDurationS, jugimap::EasingKind::EASE_END_QUAD);

    }else if(mTransitionType==TransitionType::OUT){
       // mStartAlpha = 1.0f;
        mTweenX.Init(1.0f, mScaleMin.x, mDurationS, jugimap::EasingKind::EASE_END_QUAD);
        mTweenY.Init(1.0f, mScaleMin.y, mDurationS, jugimap::EasingKind::EASE_END_QUAD);
    }


    //---
    //reset();  / NO !

    //---
    dbgSystem.removeLastMessage();
    return true;

}


void ScaleInOut::onTransitionStarted()
{

    std::vector<void*> &objects = mParentTransitionMember->objects();
    GfxObjectType objectTypeId = mParentTransitionMember->objectType();


    Vec2f initialScale;

    if(mTransitionType==TransitionType::IN){
        initialScale = mScaleMin;

    }else if(mTransitionType==TransitionType::OUT){
        initialScale.set(1.0f, 1.0f);
    }


    if(objectTypeId==GfxObjectType::SPRITE){

        for(void* obj : objects){
            Sprite * s = static_cast<Sprite*>(obj);
            s->createAnimatedPropertiesIfNone();
            s->animatedProperties()->scale = initialScale;
            s->setScale(s->scale(false));
            s->setVisible(true);
        }

    }else if(objectTypeId==GfxObjectType::SPRITE_LAYER){

        for(void* obj : objects){
            SpriteLayer * sl = static_cast<SpriteLayer*>(obj);

            for(jugimap::Sprite* s : sl->sprites()){
                s->createAnimatedPropertiesIfNone();
                s->animatedProperties()->scale = initialScale;
                s->setScale(s->scale(false));
            }

            sl->setVisible(true);
        }

    }else if(objectTypeId==GfxObjectType::MAP){


    }

    mTweenX.Stop();
    mTweenY.Stop();

    mStatus = ActionStatus::IDLE;
}


void ScaleInOut::execute()
{

    onTransitionStarted();

    if(mDelayS>0.01){
        mDelayTimer.start(mDelayS, Timer::Mode::ONE_SHOT);
    }else{
        mTweenX.Play();
        mTweenY.Play();
    }

    mStatus = ActionStatus::STARTED;

}


void ScaleInOut::update()
{

    if(mStatus==ActionStatus::IDLE) return;

    std::vector<void*> &objects = mParentTransitionMember->objects();
    GfxObjectType objectTypeId = mParentTransitionMember->objectType();


    if(mStatus==ActionStatus::STARTED){
        mStatus = ActionStatus::ACTIVE;
    }


    if(mStatus==ActionStatus::ACTIVE){

        //--- manage delay
        if(mDelayTimer.state()==Timer::State::RUNNING){
            if(mDelayTimer.update()==Timer::State::INTERVAL_PASSED){
                mTweenX.Play();
                mTweenY.Play();
            }else{
                return;
            }
        }

        //---
        Vec2f scale;
        scale.x = mTweenX.Update();
        scale.y = mTweenY.Update();

        if(objectTypeId==GfxObjectType::SPRITE){

            for(void* obj : objects){
                Sprite * s = static_cast<Sprite*>(obj);
                s->animatedProperties()->scale = scale;
                s->setScale(s->scale(false));
            }

        }else if(objectTypeId==GfxObjectType::SPRITE_LAYER){

            for(void* obj : objects){
                SpriteLayer * sl = static_cast<SpriteLayer*>(obj);

                //---
                for(jugimap::Sprite* s : sl->sprites()){
                    s->animatedProperties()->scale = scale;
                    s->setScale(s->scale(false));
                }
            }

        }else if(objectTypeId==GfxObjectType::MAP){


        }

        if(mTweenX.IsIdle() && mTweenY.IsIdle()){
            //onTransitionFinished();
            mStatus = ActionStatus::FINISHED;
        }

    }else if(mStatus==ActionStatus::FINISHED){

        mStatus = ActionStatus::IDLE;
    }

}






}
