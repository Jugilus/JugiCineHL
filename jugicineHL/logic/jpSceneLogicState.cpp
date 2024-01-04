#include "pugixml/pugixml.hpp"
#include "jmSystem.h"

//#include "jmGuiBar.h"
#include "jmAnimationCommon.h"
#include "map/jmSpriteLayer.h"
#include "map/jmMap.h"
#include "jmGlobal.h"
#include "jmScene.h"
#include "jmSceneLayout.h"
#include "jmUtilities.h"

#include "gui/jpGuiCommon.h"
#include "gui/jpGuiSystem.h"

#include "jpVariables.h"
#include "jpUtilities.h"
#include "jpCompound.h"
#include "jpComponentsCommon.h"
#include "jpPlayedScene.h"
#include "jpObjectFactory.h"
#include "jpSceneLogicState.h"



namespace jugimap{



LogicStateCfg* SceneLogicStateCfg::createLogicStateCfg(const std::string &_name)
{
    return new SceneLogicStateCfg(_name);
}


CustomLogicState* SceneLogicStateCfg::createCustomState(LogicState *_parentState)
{
    return new SceneCustomLogicState(_parentState, this);
}


bool SceneLogicStateCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading state '" + mName + "' ...");


    //---- attributes

    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
            mName = a.as_string("");

        }else if(attributeName=="compound"){
            compound = a.as_string("");

        }else if(attributeName=="component"){
            component = a.as_string("");

        }else if(attributeName=="components"){
            std::string sValue = a.as_string("");
            components = StdString::splitString(sValue, ",");

        }else{

            dbgSystem.addMessage("Wrong attribute '" + attributeName + "' !");
            return false;
        }
    }


    //---- subnodes  (actions and child states)
    if(LogicStateCfg::initCfg(_node)==false){
        return false;
    }


    dbgSystem.removeLastMessage();
    return true;
}





//===========================================================================


SceneCustomLogicState::SceneCustomLogicState(LogicState *_parentState, SceneLogicStateCfg *_cfg) : CustomLogicState(_parentState), mCfg(_cfg)
{

    mCompoundTransition.reset(new CompoundTransition());

}


SceneCustomLogicState::~SceneCustomLogicState()
{
    //
}



bool SceneCustomLogicState::initConnections(PlayedScene *_scene)
{

    //---
    if(mCfg->component.empty()==false){

        Component *component = _scene->componentsGroup()->getComponent(mCfg->component);
        if(component==nullptr){
            return false;
        }
        mUsedComponents.push_back(component);
    }


    for(const std::string &s : mCfg->components){

        std::string cName;
        std::string cParameters;
        std::vector<std::string>parts = StdString::splitString(s, ":");

        if(parts.size()>0){
            cName = parts[0];
        }
        if(parts.size()>1){
            cParameters = parts[1];
        }

        Component *component = _scene->componentsGroup()->getComponent(cName);
        if(component==nullptr){
            return false;
        }
        if(component->initConnections_setParameters(cParameters, _scene)==false){
            return false;
        }
        mUsedComponents.push_back(component);
    }


    //---
    if(mCfg->compound.empty()==false){
        mCompound = _scene->compoundStorage()->getCompund(mCfg->compound);
        if(mCompound==nullptr){
            return false;
        }
    }

    return true;

}





bool SceneCustomLogicState::startingPhaseUpdate()
{

    //initCompoundTransition();


    //----
    bool startingPhase = false;

    for(Component* c : mUsedComponents){
        if(c->startingPhaseUpdate()==true){
            startingPhase = true;
        }
    }

    if(mCompound){
        if(mCompound->startingPhaseUpdate(mCompoundTransition.get())==true){
            startingPhase = true;
        }
    }

    return startingPhase;

}


void SceneCustomLogicState::start()
{

    //initCompoundTransition();

    //----
    for(Component *c : usedComponents()){
        c->start();
    }

    if(mCompound){
        mCompound->start(mCompoundTransition.get());
    }

}


void SceneCustomLogicState::initTransitionFrom(CustomLogicState *_prevStateData)
{

    Compound * previousCompound = nullptr;
    if(_prevStateData){
        previousCompound = static_cast<SceneCustomLogicState*>(_prevStateData)->compound();
    }

    mCompoundTransition->init(previousCompound, mCompound);


    /*
    if(mParentState->parentObject() && mParentState->parentObject()->baseType()==BaseObjectType::LOGIC_STATE){

        LogicState *grandParent = static_cast<LogicState*>(mParentState->parentObject());

        if(grandParent->nextChildState()){  // flag

            assert(grandParent->activeChildState()==mParentState);

            Compound * previousCompound = nullptr;
            if(grandParent->previousChildState()){
                previousCompound = static_cast<SceneLogicStateData*>(grandParent->previousChildState()->stateData())->compound();
            }

            mCompoundTransition->init(previousCompound, mCompound);
        }
    }
    */
}


void SceneCustomLogicState::preUpdate(UpdateMode &_updateMode)
{


    for(Component* c : mUsedComponents){
        c->preUpdate(_updateMode);
    }

    if(mCompound){
        mCompound->preUpdate(_updateMode);
    }
}


void SceneCustomLogicState::update(UpdateMode &_updateMode)
{

    for(Component* c : mUsedComponents){
        c->update(_updateMode);
    }

    if(mCompound){
        mCompound->update(_updateMode);
    }

}


void SceneCustomLogicState::postUpdate(UpdateMode &_updateMode)
{

    for(Component* c : mUsedComponents){
        c->postUpdate(_updateMode);
    }

    if(mCompound){
        mCompound->postUpdate(_updateMode);
    }

}


void SceneCustomLogicState::finish()
{

    for(Component *c : usedComponents()){
        c->onStateEnded();
    }

    //---
    if(mCompound){
        mCompound->onStateEnded();
    }

    compoundTransition()->reset();          // not really needed, just to clean up un-needed parameters

}


void SceneCustomLogicState::drawShapes(jugimap::ShapeDrawer &_drawer, int _flags)
{

    for(Component* c : mUsedComponents){
        c->drawShapes(_drawer, _flags);
    }

    if(mCompound){
        mCompound->drawShapes(_drawer, _flags);
    }

}

//==========================================================================


//===============================================================================================================
/*

SceneLogicStateCfgsStorage::~SceneLogicStateCfgsStorage()
{

    for(SceneLogicStateCfg * c : mStorage){
        delete c;
    }

}



SceneLogicStateCfg *SceneLogicStateCfgsStorage::addEnginesControllerCfg(const std::string &_name)
{

    mStorage.push_back(new SceneLogicStateCfg(_name));
    return mStorage.back();
}


SceneLogicStateCfg *SceneLogicStateCfgsStorage::getEnginesControllerCfg(const std::string &_name, bool _setErrorMessage)
{

    for(SceneLogicStateCfg *c : mStorage){
        if(c->mName==_name){
            return c;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A 'EnginesControllerCfg' object with name '" + _name + "' not found!");
    }

    return nullptr;
}

*/

}
