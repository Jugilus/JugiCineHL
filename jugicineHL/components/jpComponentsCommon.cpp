#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmCommonFunctions.h"
#include "logic/jpTransition.h"
//#include "actions/jpCommands.h"

#include "gui/widgets/jpGuiButton.h"
#include "gui/jpGuiSystem.h"

#include "jpCompound.h"
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

bool Component::startCloseTransition()
{

    if(mParentCompound){
        mParentCompound->startOutTransition();
    }

    return true;
}


void Component::obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    if(_setErrorMessage){
        dbgSystem.addMessage("Signal obtaining function for component '" + mName + "' has not been defined!");
    }

}


void Component::obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_path, bool _setErrorMessage)
{

    if(_setErrorMessage){
        dbgSystem.addMessage("Signal obtaining function for component '" + mName + "' has not been defined!");
    }
}


/*


bool OverlayComponent::startOpenTransition()
{

    if(mParentCompound){
        if(mParentCompound->state()==Compound::State::TRANSITION_IN){
            mState = State::TRANSITION_IN;
            return true;
        }else{
            mState = State::NORMAL;
            return false;
        }
    }

    return true;
}


bool OverlayComponent::startCloseTransition()
{

    if(mParentCompound){
        mParentCompound->startOutTransition();

        if(mParentCompound->state()==Compound::State::TRANSITION_OUT){
            mState = State::TRANSITION_OUT;
            return true;
        }else{
            mState = State::CLOSED;
            return false;
        }
    }

    return true;
}


void OverlayComponent::update(UpdateMode _updateMode)
{

    if(mParentCompound){
        mState = State::NORMAL;
        return;         // transitions are handled in compound
    }


}



void OverlayComponent::setTransitions(ADoTransition *_openTransition, ADoTransition *_closeTransition)
{
    mAOpenTransition = _openTransition;
    mACloseTransition = _closeTransition;
}


*/

//---------------------------------------------------------------------------


ComponentsGroup::~ComponentsGroup()
{

    for(Component* c : mComponents){
        delete c;
    }

}


bool ComponentsGroup::buildObjects(PlayedScene *_scene)
{

    dbgSystem.addMessage("Build components...");


    for(Component* c : mComponents){
        if(c->buildObjects(_scene)==false){
            return false;
        }
    }


    //---
    dbgSystem.removeLastMessage();
    return true;
}


bool ComponentsGroup::initConnections(PlayedScene* _scene)
{

    dbgSystem.addMessage("Initialize components...");


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
