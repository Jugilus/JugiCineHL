#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmGuiBar.h"
#include "jmGuiTextInput.h"
#include "jmCommonFunctions.h"
#include "actions/jpTransition.h"
#include "actions/jpCommands_Logic.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpComponentsCommon.h"



namespace jugimap{



Component::~Component()
{
    //if(mData){
    //    delete mData;
    //}
}



bool OverlayComponent::startOpenTransition()
{

    if(mAOpenTransition==nullptr){
        mState = State::NORMAL;
        return false;
    }

    mAOpenTransition->execute();
    mParentPlayerScene->addLongRunningAction(mAOpenTransition);     // make scene aware that a long action is running
    if(mModal){
        mParentPlayerScene->widgetManager()->addModalBlockLevelToUsedWidgets();   // mark current used widgets as modal blocked!
    }
    mParentPlayerScene->widgetManager()->appendToUsedWidgets(mUsedWidgets);
    mState = State::TRANSITION_IN;

    return true;
}


bool OverlayComponent::startCloseTransition()
{

    if(mACloseTransition==nullptr){
        mState = State::CLOSED;
        return false;
    }

    mACloseTransition->execute();
    mParentPlayerScene->addLongRunningAction(mACloseTransition);     // make scene aware that a long action is running
    mState = State::TRANSITION_OUT;

    return true;
}


void OverlayComponent::update(UpdateMode _updateMode)
{

    if(mState==State::TRANSITION_IN){

        mAOpenTransition->update(_updateMode);

        if(mAOpenTransition->status()==ActionStatus::IDLE){
            mState = State::NORMAL;
            mParentPlayerScene->removeLongRunningAction(mAOpenTransition);
        }


    }else if(mState==State::NORMAL){

        // managed in extended classes


    }else if(mState==State::TRANSITION_OUT){

        mACloseTransition->update(_updateMode);

        if(mACloseTransition->status()==ActionStatus::IDLE){
            mState = State::CLOSED;
            mParentPlayerScene->removeLongRunningAction(mACloseTransition);
            mParentPlayerScene->widgetManager()->removeFromUsedWidgets(mUsedWidgets);
            if(mModal){
                mParentPlayerScene->widgetManager()->removeModalBlockLevelToUsedWidgets();   // mark current used widgets as modal blocked!
            }

            //---
            mTrigger_Closed->doSetTriggered();
            mParentPlayerScene->activatedTriggersUpdater().addTrigger(mTrigger_Closed);

            //---
            mParentPlayerScene->endOverlayComponent(this);
        }
    }

}



void OverlayComponent::setTransitions(ADoTransition *_openTransition, ADoTransition *_closeTransition)
{
    mAOpenTransition = _openTransition;
    mACloseTransition = _closeTransition;
}



//---------------------------------------------------------------------------


bool ComponentsGroup::initConnections(PlayedScene* _scene)
{

    dbgSystem.addMessage("init components");


    for(Component* c : mComponents){
        if(c->initConnections(_scene)==false){
            return false;
        }
    }


    //---
    dbgSystem.removeLastMessage();

    return true;
}


Component* ComponentsGroup::getComponent(const std::string &_name, bool _setErrorMessage)
{


    for(Component* c : mComponents){
        if(c->name()==_name){
            return c;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("Component '" + _name +"' not found!");
    }

    return nullptr;

}

//================================================================================================




CustomComponentFactoryManager::~CustomComponentFactoryManager()
{

    for(CustomComponentFactory * f : mFactories){
        delete f;
    }

}


void CustomComponentFactoryManager::addFactory(CustomComponentFactory *_factory)
{
    mFactories.push_back(_factory);
}



Component* CustomComponentFactoryManager::createCustomComponent(const std::string &_typeSignature, bool _setErrorMessage)
{

    for(CustomComponentFactory* f : mFactories){
        if(f->typeSignature() == _typeSignature){
            return  f->createComponent();
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("Type signature '" + _typeSignature + "' for a component not recognized!");
    }
    return nullptr;

}



}
