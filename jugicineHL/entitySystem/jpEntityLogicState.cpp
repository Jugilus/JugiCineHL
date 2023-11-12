#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmMap.h"
#include "jmCamera.h"
#include "jmSceneLayout.h"
#include "jmCommonFunctions.h"
#include "jmSpriteLayer.h"
#include "jmUtilities.h"

//#include "jpActionsCommon.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"

#include "jpB2Body.h"
#include "jpB2Utilities.h"
#include "jpEntity.h"
#include "jpEntityUtilities.h"
#include "jpSourceEntity.h"
#include "movements/jpMovementBase.h"
#include "jpObjectFactory.h"
#include "jpEntityLogicState.h"



namespace jugimap{



using namespace jugimap;



bool EntityLogicStateCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading state '" + mName + "' ...");

    //---- attributes

    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
           mName = a.as_string("");

        }else if(attributeName=="contactsEnabled"){
            contactsEnabled = a.as_bool(true);

        }else if(attributeName=="animation"){
            animation = a.as_string("");

        }else if(attributeName=="movementEngine"){
            movementEngine = a.as_string("");

        }else if(attributeName=="taskEngine"){
            taskEngine = a.as_string("");

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


    /*

    //---- sub-nodes

    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        ActionCfg * behaviorActionCfg = nullptr;

        //if(nodeName=="aStart"){
        //    behaviorActionCfg = & mStartActionCfg;
        //
        //}else
        if(nodeName=="a"){
            mUpdateActionsCfgs.push_back(ActionCfg());
            behaviorActionCfg = & mUpdateActionsCfgs.back();

        }else{
            dbgSystem.addMessage("Unknown node '"+ nodeName + "'!");
            return false;
        }

        for(pugi::xml_attribute a = n.first_attribute(); a; a=a.next_attribute()){
            std::string aName = std::string(a.name());
            std::string aValue = std::string(a.as_string());

            const std::string prefix = aName.substr(0,2);

            if(prefix=="on"){
                behaviorActionCfg->mConditions.push_back(ActionConditionCfg(aName, aValue));

            }else if(prefix=="do"){
                behaviorActionCfg->mCommands.push_back(ActionCommandCfg(aName, aValue));

            }
        }
    }

    dbgSystem.removeLastMessage();

    return true;
    */
}



LogicStateCfg* EntityLogicStateCfg::createLogicStateCfg(const std::string &_name)
{
    return new EntityLogicStateCfg(_name);
}


CustomLogicState* EntityLogicStateCfg::createCustomState(LogicState *_parentState)
{

    return new EntityCustomLogicState(this, _parentState);

}

//==============================================================================================


EntityCustomLogicState::EntityCustomLogicState(EntityLogicStateCfg *_behaviorStateCfg, LogicState* _parent) :
    mParentState(_parent),
    mBehaviorStateCfg(_behaviorStateCfg)
{

}


EntityCustomLogicState::~EntityCustomLogicState()
{
    if(mAnimationInstance){
        delete mAnimationInstance;
    }
}



bool EntityCustomLogicState::initConnections(PlayedScene *_scene)
{

    //Entity *parentEntity = mParentState->parentStateManager()->parentActor();

    Entity *parentEntity = static_cast<Entity*>(mParentState->rootParentObject());

    if(mBehaviorStateCfg->animation.empty()==false){
        mAnimationInstance = ObtainAnimationInstance(parentEntity->sprite(), mBehaviorStateCfg->animation);
        if(mAnimationInstance==nullptr){
            return false;
        }

    }else{
        // check out for possible animation with the same name
        if(mBehaviorStateCfg->movementEngine.empty()){
            mAnimationInstance = ObtainAnimationInstance(parentEntity->sprite(), mParentState->name(), false);
        }
    }

    //----
    if(mBehaviorStateCfg->movementEngine.empty()==false){
        mMovementEngineData = ObtainMovementEngineData(parentEntity, mBehaviorStateCfg->movementEngine);
        if(mMovementEngineData==nullptr){
            return false;
        }
    }

    //----
    if(mBehaviorStateCfg->taskEngine.empty()==false){
        mTaskEngineData = ObtainTaskEngineData(parentEntity, mBehaviorStateCfg->taskEngine);
        if(mTaskEngineData==nullptr){
            return false;
        }
    }


    mContactsEnabled = mBehaviorStateCfg->contactsEnabled;


    return true;

}


void EntityCustomLogicState::start()
{

    //Entity *parentEntity = mParentState->parentStateManager()->parentActor();
    Entity *parentEntity = static_cast<Entity*>(mParentState->rootParentObject());

    if(mAnimationInstance){
        mAnimationSignal.setValue(true);
    }

    if(mMovementEngineData){
        if(parentEntity->sourceEntity()->name()=="hero"){
            DummyFunction();
        }

        //mParentObject->parentActor()->startMovementEngine(mMovementEngineData);
    }

    if(parentEntity->body()){
        parentEntity->body()->B2Body()->SetEnabled(mContactsEnabled);
    }

}


void EntityCustomLogicState::finish()
{
    mAnimationSignal.setValue(false);
}



}
