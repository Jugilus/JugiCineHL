#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmCommonFunctions.h"
#include "jmSystem.h"
#include "jmTimelineAnimation.h"
#include "jmTimelineAnimationInstance.h"
#include "jmVectorShape.h"
#include "jmStandardSprite.h"
#include "jmCompositeSprite.h"
#include "jmSpriteLayer.h"
#include "jmSceneLayout.h"
#include "jmUtilities.h"

#include "jpPlayedApp.h"
#include "gfxObjects/jpAnimationObjects.h"
#include "jpUtilities.h"
#include "jpPlayedScene.h"

#include "jpEntitySystem.h"
#include "jpSourceEntity.h"
#include "jpEntity.h"
#include "jpEntityUtilities.h"
#include "jpB2World.h"
#include "jpAnimatedBehavior.h"



namespace jugimap{





bool AnimatedMovementCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading movement engine of type '" + factory->typeSignature() + "' and name '" + name + "'...");

    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());


    }

    dbgSystem.removeLastMessage();
    return true;

}



bool AnimatedMovementData::initConnections(PlayedScene *_scene, Entity *_actor)
{

    dbgSystem.addMessage("Init connections for the data object of '" + cfg->name + "' ...");


    //---
    dbgSystem.removeLastMessage();
    return true;
}




//============================================================================================



AnimatedMovementEngine::AnimatedMovementEngine(MovementEngineFactory *_factory) : MovementEngine(_factory)
{
    mDirection = Direction::FORWARD;
}



void AnimatedMovementEngine::createDataObjects(std::vector<MovementEngineCfg *> &_cfgs)
{

    mAniMovementDatas.resize(_cfgs.size());
    for(unsigned int i=0; i<_cfgs.size(); i++){
        assert(_cfgs[i]->factory == mFactory);
        mAniMovementDatas.at(i).factory = mFactory;
        mAniMovementDatas.at(i).cfg = static_cast<AnimatedMovementCfg*>(_cfgs[i]);
    }

}


bool AnimatedMovementEngine::initDataObjectsConnections(PlayedScene *_scene, Entity *_actor)
{

    for(AnimatedMovementData& d : mAniMovementDatas){
        if(d.initConnections(_scene, _actor)==false){
            return false;
        }
    }

    return true;
}


void AnimatedMovementEngine::collectSignalsForLUT(SignalStorage &_storage)
{

    for(AnimatedMovementData& d : mAniMovementDatas){


    }

}



/*
void AnimatedBehaviorEngine::start(AnimatedBehaviorData *_animatedBehaviorData, Actor *_actor)
{

    mCurrentData = _animatedBehaviorData;
    mAnimationPlayer = &_actor->animationPlayer();

    mActorSprite = _actor->stdSprite();

    //---
    mAnimatedSprite = nullptr;
    mTracedSprite = nullptr;
    Animation *behAnimation = nullptr;

    if(mCurrentData->animations.empty()){

        std::string sParameters = mActorSprite->parameters().value("eBehAnimation");

        if(sParameters.empty()==false){

            std::vector<Sprite*>sprites;
            std::vector<std::string>parameters = StdString::splitString(sParameters, ",");

            if(parameters.size()==2){

                const std::string & animatedSpriteName = parameters[0];
                const std::string & animationName = parameters[1];


                CollectSpritesWithName(mActorSprite->parentLayer()->map(), sprites, animatedSpriteName);
                if(sprites.empty()==false){

                    mAnimatedSprite = sprites.front();
                    behAnimation = FindTimelineAnimationWithName(mAnimatedSprite, animationName);
                }
            }


            if(mAnimatedSprite && behAnimation){

                std::string ghostSpriteName = mActorSprite->parameters().value("eBehAnimationGhost");

                if(ghostSpriteName.empty()==false){
                    if(ghostSpriteName == mAnimatedSprite->name()){
                        mTracedSprite = mAnimatedSprite;

                    }else{
                        if(mAnimatedSprite->sourceSprite()->kind()==SpriteType::COMPOSITE){
                            sprites.clear();
                            CollectSpritesWithName(static_cast<CompositeSprite*>(mAnimatedSprite)->map(), sprites, ghostSpriteName);
                            if(sprites.empty()==false){
                                mTracedSprite = sprites.front();
                            }
                        }
                    }
                }
            }
        }
    }



    if(mTracedSprite && mAnimatedSprite && behAnimation){

        if(behAnimation->GetKind()==AnimationKind::TIMELINE_ANIMATION){
            mBehAniInstance = new TimelineAnimationInstance(static_cast<TimelineAnimation*>(behAnimation), mAnimatedSprite);
        }

    }

}

*/


bool AnimatedMovementEngine::init(Entity *_entity)
{
    mParentEntity = _entity;

    for(AnimatedMovementData& d : mAniMovementDatas){
        mSignals.push_back(&d.sigDisabled);
    }

    return true;
}


bool AnimatedMovementEngine::start(MovementEngineData *_data)
{

    assert(mFactory==_data->factory);




    if(mParentEntity->statusFlagsRef() & EntityStatusFlags::FORCED_ENTITY_TRANSFORMATION){
        return false;
    }

    mCurrentData = static_cast<AnimatedMovementData*>(_data);

    //AnimatedBehaviorEngine * engine = animatedBehaviorEngine();

    //mDirection = engine->direction();

    //engine->start(_animatedBehaviorData, this);


    mAnimationPlayer = &mParentEntity->animationPlayer();

    mActorSprite = mParentEntity->sprite();

    //---
    mAnimatedSprite = nullptr;
    mTracedSprite = nullptr;

    if(mCurrentData->animations.empty()){

        std::string sParameters = mActorSprite->parameters().value("eBehAniObject");

        if(sParameters.empty()==false){

            std::vector<Sprite*>sprites;
            std::vector<std::string>parameters = StdString::splitString(sParameters, ",");

            if(parameters.size()==1){
                const std::string & aniObjectName = parameters[0];

                //mAniObject = mParentEntity->parentEntitySystem()->animationManager()->getAnimationObject(aniObjectName, false);
                mAniObject = mParentEntity->parentEntitySystem()->parentPlayerScene()->animationManager()->getAnimationObject(aniObjectName, false);
                if(mAniObject){
                    mAnimatedSprite = mAniObject->sprite();
                    mTracedSprite = mAnimatedSprite;
                }

            }else if(parameters.size()==2){

                const std::string & aniObjectName = parameters[0];
                const std::string & ghostSpriteName = parameters[1];

               //mAniObject = mParentEntity->parentEntitySystem()->animationManager()->getAnimationObject(aniObjectName, false);
                mAniObject = mParentEntity->parentEntitySystem()->parentPlayerScene()->animationManager()->getAnimationObject(aniObjectName, false);

               if(mAniObject){
                   mAnimatedSprite = mAniObject->sprite();
                   if(mAnimatedSprite){
                       if(mAnimatedSprite->sourceSprite()->kind()==SpriteType::COMPOSITE){
                           sprites.clear();
                           CollectSpritesWithName(static_cast<CompositeSprite*>(mAnimatedSprite)->map(), sprites, ghostSpriteName);
                           if(sprites.empty()==false){
                               mTracedSprite = sprites.front();
                           }
                       }
                   }
               }
            }
        }
    }

    //---
    mPrevPos = mActorSprite->absPosition();

    SourceImage *si = mActorSprite->sourceSprite()->sourceImages().front();
    Vec2f handle = Vec2iToVec2f(si->handle());
    Vec2f anchorPointP(handle.x - si->width() * 0.5f, handle.y - si->height()*0.5f);
    mPrevPos += anchorPointP;


    return true;

}



b2Vec2 AnimatedMovementEngine::update(EngineUpdateParameters &eup)
{

    mStateEllapsedTimeS += eup.timeStep;
    mVelocity.SetZero();


    //---
    if(mTracedSprite==nullptr) return mVelocity;


    if(mAniObject->animationPlayer().GetState()==AnimationPlayerState::PLAYING){

        Vec2f currentPos = mTracedSprite->absPosition();

        b2Vec2 posStartM = gWorldInfo.pixelsToMeters(mPrevPos);
        b2Vec2 posEndM = gWorldInfo.pixelsToMeters(currentPos);

        mVelocity.x = (posEndM.x - posStartM.x)/eup.timeStep;
        mVelocity.y = (posEndM.y - posStartM.y)/eup.timeStep;

        //---
        mPrevPos = currentPos;
    }


    //---
    updateAnimationPlayer();


    return mVelocity;
}


void AnimatedMovementEngine::stop()
{
    //mVelocityGenerator.stop();
}


MovementEngineData* AnimatedMovementEngine::getMovementEngineData(const std::string &_name, bool _setErrorMessage)
{

    for(AnimatedMovementData &d : mAniMovementDatas){
        if(d.cfg->name == _name){
            return &d;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("An 'AnimatedBehaviorData' object with name '" + _name + "' not found!");
    }

    return nullptr;

}

/*
AnimatedBehaviorData* AnimatedBehaviorEngine::getAnimatedBehaviorData(const std::string &_name, bool _setErrorMessage)
{

    for(AnimatedBehaviorData &d : mAniMovementDatas){
        if(d.cfg->name == _name){
            return &d;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("An 'AnimatedBehaviorData' object with name '" + _name + "' not found!");
    }

    return nullptr;
}
*/

//===========================================================================================================


AnimatedMovementCfg* AnimatedMovementFactory::createMovementEngineCfg(const std::string &_name)
{
    return new AnimatedMovementCfg(this, _name);
}


AnimatedMovementEngine* AnimatedMovementFactory::createMovementEngine()
{
    return new AnimatedMovementEngine(this);
}


}
