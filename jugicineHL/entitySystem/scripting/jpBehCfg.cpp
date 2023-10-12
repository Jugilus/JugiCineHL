#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmMap.h"
#include "jmCamera.h"
#include "jmSceneLayout.h"
#include "jmCommonFunctions.h"
#include "jmGuiText.h"
#include "jmSpriteLayer.h"
#include "jmUtilities.h"

#include "jpPlayedApp.h"
#include "jpPlayedScene.h"

#include "jpB2Body.h"
#include "jpB2Utilities.h"
#include "jpBehCfg.h"



namespace jugimap{



bool BehaviorStateCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading state '" + name + "' ...");

    //---- attributes

    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
            name = a.as_string("");

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


    //---- sub-nodes

    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        BehaviorActionCfg * behaviorActionCfg = nullptr;

        if(nodeName=="aStart"){
            behaviorActionCfg = & mStartActionCfg;

        }else if(nodeName=="a"){
            mUpdateActionsCfgs.push_back(BehaviorActionCfg());
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
}


//===============================================================================================================


bool EntityControllerCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading controller '" + name +"' states ...");

    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="state"){

            std::string name = n.attribute("name").as_string("");
            if(name.empty()){
                dbgSystem.addMessage("Missing attribute 'name' in the node 'state'!");
                return false;
            }

            statesCfgs.push_back(BehaviorStateCfg(name));
            BehaviorStateCfg & stateCfg =  statesCfgs.back();

            if(stateCfg.initCfg(n)==false){
                return false;
            }
        }
    }


    dbgSystem.removeLastMessage();

    return true;
}


//===============================================================================================================


EnginesControllersCfgsGroup::~EnginesControllersCfgsGroup()
{

    for(EntityControllerCfg * c : mEnginesControllersCfgs){
        delete c;
    }

}



EntityControllerCfg *EnginesControllersCfgsGroup::addEnginesControllerCfg(const std::string &_name)
{
    mEnginesControllersCfgs.push_back(new EntityControllerCfg(_name));
    return mEnginesControllersCfgs.back();
}


EntityControllerCfg *EnginesControllersCfgsGroup::getEnginesControllerCfg(const std::string &_name, bool _setErrorMessage)
{

    for(EntityControllerCfg *c : mEnginesControllersCfgs){
        if(c->name==_name){
            return c;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A 'EnginesControllerCfg' object with name '" + _name + "' not found!");
    }

    return nullptr;
}


//-------------------------------------------------------------------------------------------------------------

/*

TaskControllerCfg & TasksHandlersCfgsGroup::addTaskControllerCfg(const std::string &_name)
{
    mTaskControllersCfgs.push_back(TaskControllerCfg(_name));
    return mTaskControllersCfgs.back();
}


TaskControllerCfg *TasksHandlersCfgsGroup::getTaskControllerCfg(const std::string &_name, bool _setErrorMessage)
{

    for(TaskControllerCfg & c : mTaskControllersCfgs){
        if(c.name==_name){
            return &c;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A 'TaskControllerCfg' object with name '" + _name + "' not found!");
    }

    return nullptr;
}

*/


}
