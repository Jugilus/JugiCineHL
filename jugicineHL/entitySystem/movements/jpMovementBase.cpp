#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmAnimationCommon.h"
#include "jmCommonFunctions.h"
#include "jmSystem.h"

#include "jpB2World.h"
#include "jpEntityUtilities.h"
#include "jpMovementBase.h"



namespace jugimap{




//---------------------------------------------------------------

MovementEngineType MovementEngine::type()
{
    return mFactory->type();

}


void MovementEngine::obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp,  bool _setErrorMessage)
{

    if(_setErrorMessage){
        dbgSystem.addMessage("There is no signal with name '" + _psp.signalFullName() + "' inside the movement engine '" + mFactory->typeSignature()+"' !");
    }

}

void MovementEngine::obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage)
{
    if(_setErrorMessage){
        dbgSystem.addMessage("There is no signal with name '" + _psp.signalFullName() + "' inside the movement engine '" + mFactory->typeSignature()+"' !");
    }
}





void MovementEngine::resetAnimationPlayer()
{

    if(mAnimationPlayer==nullptr) return;

    mAnimationPlayer->Stop();
    mActiveAnimationInstance = nullptr;

}


void MovementEngine::updateAnimationPlayer()
{

    if(mAnimationPlayer==nullptr) return;
    if(mActiveAnimationInstance==nullptr) return;


    int aniPlayerFlags = 0;
    if(mAnimationPlayer->GetAnimationInstance() != mActiveAnimationInstance){
        aniPlayerFlags = mAnimationPlayer->Play(mActiveAnimationInstance);
     }else{
        aniPlayerFlags = mAnimationPlayer->Update();
    }

    if(aniPlayerFlags & AnimationPlayerFlags::ANIMATED_PROPERTIES_CHANGED){
        mActiveAnimationInstance->UpdateAnimatedSprites(true);
    }
    if(aniPlayerFlags & AnimationPlayerFlags::META_KEY_CHANGED){
        DummyFunction();
    }

}


bool MovementEngine::isCurrentAnimationStalled()
{
    if(mAnimationPlayer==nullptr) return false;
    if(mActiveAnimationInstance==nullptr) return false;

    return  mAnimationPlayer->GetState() == AnimationPlayerState::STALLED;

}


//=========================================================================================



GroundMovementEngineBase::GroundMovementEngineBase(jugimap::MovementEngineFactory *_factory) : MovementEngine(_factory)
{
    mDirection = Direction::LEFT;
}


//=========================================================================================


MovementEnginesManager::MovementEnginesManager()
{

    mDummyEngineFactory = new DummyMovementEngineFactory("dummyMovement", MovementEngineType::NO_ENGINE_SET);
    mDummyMovementEngine = new DummyMovementEngine(mDummyEngineFactory);
}


MovementEnginesManager::~MovementEnginesManager()
{

    for(MovementEngineCfg * cfg : mMovementEngineCfgs){
        delete cfg;
    }

    for(MovementEngineFactory * c : mEngineCreators){
        delete c;
    }

    delete mDummyMovementEngine;
    delete mDummyEngineFactory;

}


void MovementEnginesManager::addMovementEngineCreator(MovementEngineFactory *_engineCreator)
{
    mEngineCreators.push_back(_engineCreator);
}


MovementEngineCfg* MovementEnginesManager::addMovementEngineCfg(const std::string &_typeSignature, const std::string &_name, bool _setErrorMessage)
{

    for(MovementEngineCfg * cfg : mMovementEngineCfgs){
        if(cfg->name == _name){
            if(_setErrorMessage){
                dbgSystem.addMessage("The name '" + _name + "' of a movement engine configuration is already used! Every configuration must have unique name!");
            }
            return nullptr;
        }
    }


    //---
    MovementEngineCfg *cfg = nullptr;

    for(MovementEngineFactory* mec : mEngineCreators){
        if(mec->typeSignature() == _typeSignature){
            cfg = mec->createMovementEngineCfg(_name);
            break;
        }
    }

    if(cfg){
        mMovementEngineCfgs.push_back(cfg);
        return cfg;
    }


    if(_setErrorMessage){
        dbgSystem.addMessage("Type signature '" + _typeSignature + "' for a movement engine configuration not recognized!");
    }
    return nullptr;

}


MovementEngineCfg* MovementEnginesManager::getMovementEngineCfg(const std::string &_name, bool _setErrorMessage)
{

    for(MovementEngineCfg * cfg : mMovementEngineCfgs){
        if(cfg->name == _name){
            return cfg;
        }
    }


    if(_setErrorMessage){
        dbgSystem.addMessage("A movement engine configuration with name '" + _name + "' not found!");
    }
    return nullptr;

}


MovementEngine* MovementEnginesManager::createMovementEngine(const std::string &_typeSignature, bool _setErrorMessage)
{

    for(MovementEngineFactory* mec : mEngineCreators){
        if(mec->typeSignature() == _typeSignature){
            return  mec->createMovementEngine();
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("Type signature '" + _typeSignature + "' for a movement engine not recognized!");
    }
    return nullptr;

}



}
