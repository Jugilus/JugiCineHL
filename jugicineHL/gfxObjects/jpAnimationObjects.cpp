#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmFrameAnimation.h"
#include "jmTimelineAnimation.h"
#include "jmTimelineAnimationInstance.h"
#include "jmSystem.h"
#include "jmSprite.h"
#include "jmStandardSprite.h"
#include "jmTextSprite.h"
#include "jmCommonFunctions.h"
#include "jmSceneLayout.h"
#include "jmMap.h"
#include "jmUtilities.h"

#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpQueries.h"
#include "jpUtilities.h"
#include "jpObjectParser.h"
#include "jpAnimationObjects.h"



namespace jugimap{






AnimationObject::AnimationObject(const std::string &_name, const std::vector<Sprite*>_sprites) : mName(_name)
{

    if(_sprites.size()==1){
        mSprite = _sprites.front();
    }else{
        mSprites = _sprites;
    }

}



bool AnimationObject::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Init '" +mName + "' ...");


    Sprite *s = mSprite;

    if(s==nullptr){
        s = mSprites.front();
    }
    assert(s);



    Animation *animation = nullptr;

    /*
    std::string aniName = s->parameters().value("aniObjectAnimation");
    if(aniName.empty()==false){
        DummyFunction();
        assert(false);

        // OBSOLETE ! USE aniOnStart
    }
    */

    std::string aniName;
    //bool paused = false;

     std::string aniOnStart = s->parameters().value("aniOnStart");

     if(aniOnStart.empty()==false){
         std::vector<std::string>parts = StdString::splitString(aniOnStart, ",");
         if(parts.size()==1){

             if(parts[0] == "PAUSED"){
                 mPaused = true;
             }else{
                 aniName = parts[0];
             }

         }else if(parts.size()==2){
             aniName = parts[0];

             if(parts[1] == "PAUSED"){
                 mPaused = true;
             }else{
                 dbgSystem.addMessage("Error parsing paramter value '" + aniOnStart +"' !");
                 return false;
             }

         }else{
             dbgSystem.addMessage("Error parsing paramter value '" + aniOnStart +"' !");
             return false;
         }
     }


    mSigEnabled.reset(!mPaused);


    if(aniName.empty()==false){
        if(s->type()==SpriteType::STANDARD){
            animation = FindFrameAnimationWithName(static_cast<StandardSprite*>(s), aniName);
        }
        if(animation==nullptr){
            animation = FindTimelineAnimationWithName(static_cast<StandardSprite*>(s), aniName);
        }

        if(animation==nullptr){
            dbgSystem.addMessage("Animation '" + aniName + "' not found!");
            return false;
        }

    }else{

        if(s->type()==SpriteType::STANDARD){
            if(s->sourceSprite()->frameAnimations().empty()==false){
                animation = s->sourceSprite()->frameAnimations().front();
            }
        }
        if(animation==nullptr){
            if(s->sourceSprite()->timelineAnimations().empty()==false){
                animation = s->sourceSprite()->timelineAnimations().front();
            }
        }

        if(animation==nullptr){
            dbgSystem.addMessage("Source sprite '" + s->sourceSprite()->name() + "' has no animations!");
            return false;
        }
    }


    if(animation->GetKind()==AnimationKind::FRAME_ANIMATION){
        if(mSprite){
            mAnimationInstance = new FrameAnimationInstance(static_cast<FrameAnimation*>(animation), mSprite);
        }else{
            mAnimationInstance = new FrameAnimationInstance(static_cast<FrameAnimation*>(animation), mSprites);
        }


    }else if(animation->GetKind()==AnimationKind::TIMELINE_ANIMATION){
        if(mSprite){
            mAnimationInstance = new TimelineAnimationInstance(static_cast<TimelineAnimation*>(animation), mSprite);
        }else{
            mAnimationInstance = new TimelineAnimationInstance(static_cast<TimelineAnimation*>(animation), mSprites);
        }

    }


    //---
    dbgSystem.removeLastMessage();
    return true;
}



void AnimationObject::preUpdate()
{
    //mSigDisabled.preUpdate();
}


void AnimationObject::update()
{

    int aniPlayerFlags = 0;
    if(mAnimationPlayer.GetAnimationInstance() != mAnimationInstance){
        aniPlayerFlags = mAnimationPlayer.Play(mAnimationInstance);
     }else{

        if(mSigEnabled.active(true)){
            if(mAnimationPlayer.GetState()==AnimationPlayerState::PAUSED){
                mAnimationPlayer.Resume();
            }

        }else{
            if(mAnimationPlayer.GetState()!=AnimationPlayerState::PAUSED){
                mAnimationPlayer.Pause();
            }
        }

        aniPlayerFlags = mAnimationPlayer.Update();
    }

    if(aniPlayerFlags & AnimationPlayerFlags::ANIMATED_PROPERTIES_CHANGED){
        mAnimationInstance->UpdateAnimatedSprites(true);
    }
}


void AnimationObject::postUpdate()
{
    //mSigDisabled.postUpdate();
}



void AnimationObject::obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    if(_psp.signalFullName()=="ENABLED"){
        _psp.obtainValue(_signalQuery, &mSigEnabled);
    }

    if(_signalQuery.signal()==nullptr &&_setErrorMessage){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }

}


void AnimationObject::obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    if(_psp.signalFullName()=="ENABLED"){
        _psp.obtainValue(_signalSetter, &mSigEnabled);
    }

    if(_signalSetter.signal()==nullptr &&_setErrorMessage){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown or not available for setting it!");
    }
}


bool AnimationObject::obtainSignal(SignalParsingInfo &_spi, bool _setErrorMessage)
{

    if(_spi.signalAccessorType==SignalAccessorType::QUERY){

        if(_spi.signalIdentifierPath=="ENABLED"){
            _spi.signal = &mSigEnabled;

        }else{
            if(_spi.signal==nullptr &&_setErrorMessage){
                dbgSystem.addMessage("There is no signal with name'" + _spi.signalIdentifierPath + "' in the animation object!");
                return false;
            }
        }


    }else if(_spi.signalAccessorType==SignalAccessorType::SETTER){

        if(_spi.signalIdentifierPath=="ENABLED"){
            _spi.signal = &mSigEnabled;

        }else{
            if(_spi.signal==nullptr &&_setErrorMessage){
                dbgSystem.addMessage("There is no signal with name'" + _spi.signalIdentifierPath + "' in the animation object!");
                return false;
            }
        }

    }

    return true;

}


//==============================================================================================


AnimationManager::~AnimationManager()
{

    for(AnimationObject *ao : mAnimationObjects){
        delete ao;
    }
}



bool AnimationManager::create(PlayedScene *_scene)
{

    // standalone animation objects
    std::vector<Sprite*>sprites;
    for(SceneMap *sm : _scene->sceneMaps()){
        CollectSpritesWithParameter(sm->map(), sprites, "aniObject");
    }


    typedef std::pair<std::string, std::vector<Sprite*>> SpritesGroup;
    std::vector<SpritesGroup>spritesGroups;

    for(Sprite *s : sprites){
        const std::string &name = s->parameters().value("aniObject");

        SpritesGroup *spritesGroup = nullptr;
        for(SpritesGroup &sg : spritesGroups){
            if(name == sg.first){
                spritesGroup = &sg;
                break;
            }
        }

        if(spritesGroup==nullptr){
            spritesGroups.emplace_back(SpritesGroup(name, std::vector<Sprite*>({s})));

        }else{
            spritesGroup->second.push_back(s);
        }
    }


    for(SpritesGroup &group : spritesGroups){
        mAnimationObjects.push_back(new AnimationObject(group.first, group.second));
    }


    return true;
}



bool AnimationManager::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing animation manager...");


    for(AnimationObject *ao : mAnimationObjects){
        if(ao->initConnections(_scene)==false){
            return false;
        }
    }


    //---
    dbgSystem.removeLastMessage();
    return true;

}


void AnimationManager::preUpdate()
{

    for(AnimationObject *ao : mAnimationObjects){
        ao->preUpdate();
    }

}


void AnimationManager::update()
{

    for(AnimationObject *ao : mAnimationObjects){
        ao->update();
    }

}


void AnimationManager::postUpdate()
{

    for(AnimationObject *ao : mAnimationObjects){
        ao->postUpdate();
    }

}



AnimationObject * AnimationManager::getAnimationObject(const std::string &_name, bool _setErrorMessage)
{

    for(AnimationObject * ao : mAnimationObjects){
        if(ao->name()==_name){
            return ao;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("Animation object '" + _name + "' not found!");
    }

    return nullptr;
}


//=========================================================================================================

/*

bool AnimationObjectSignalStrings::parse(const std::string &_path)
{

    std::vector<std::string> parts = StdString::splitString(_path, ":");

    if(parts.size()>0){
        animationObject = parts[0];
    }

    if(parts.size()>1){
        signalName = parts[1];
    }

    if(signalName.empty()){
        dbgSystem.addMessage("Error parsing signal path '" + _path + "'!");
        return false;
    }

    // parsing value state for bool signals
    std::vector<std::string> parts2 = StdString::splitString(signalName, "=");
    if(parts2.size()==2){
        signalName = parts2[0];
        signalValueState = parts2[1];
    }

    // parsing value state for int signals and bitflag signals
    if(signalValue.empty()==false){
        parts2 = StdString::splitString(signalValue, "=");
        if(parts2.size()==2){
            signalValue = parts2[0];
            signalValueState = parts2[1];
        }
    }

    return true;

}


bool AnimationObjectSignalStrings::getBoolValue(bool &_state, bool _setErrorMessage) const
{

    if(signalValueState=="TRUE" || signalValueState=="ON" || signalValueState=="1"){
        _state = true;
        return true;

    }else if(signalValueState=="FALSE" || signalValueState=="OFF" || signalValueState=="0"){
        _state = false;
        return true;
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("Wrong signal value '" + signalValueState +" ' !");
    }

    return false;

}

*/



}
