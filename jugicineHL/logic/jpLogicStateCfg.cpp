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

#include "jpLogicStateCfg.h"



namespace jugimap{




LogicStateCfg::LogicStateCfg(const std::string &_name) : mName(_name)
{

}


LogicStateCfg::~LogicStateCfg()
{



    for(LogicStateCfg *cfg : mChildStatesCfgs){
        delete cfg;
    }

}


bool LogicStateCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading (base) state '" + mName + "' ...");


    //---- sub-nodes
    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        ActionCfg * behaviorActionCfg = nullptr;


        if(nodeName=="state"){

            std::string childStateName = n.attribute("name").as_string();
            if(childStateName.empty()){
                dbgSystem.addMessage("Missing attribute 'name' in node 'state' !" );
                return false;
            }

            LogicStateCfg * childState = createLogicStateCfg(childStateName);
            if(childState->initCfg(n)==false){
                return false;
            }

            mChildStatesCfgs.push_back(childState);

        }else if(nodeName=="a"){
            mActionsCfgs.push_back(ActionCfg());
            behaviorActionCfg = & mActionsCfgs.back();


        }else{

            dbgSystem.addMessage("Unknown node '"+ nodeName + "'!");
            return false;
        }


        if(behaviorActionCfg){

            for(pugi::xml_attribute a = n.first_attribute(); a; a=a.next_attribute()){
                std::string aName = std::string(a.name());
                std::string aValue = std::string(a.as_string());


                if(aName=="disabled"){
                    behaviorActionCfg->mDisabled = a.as_bool(false);
                    continue;

                }else if(aName=="dbgId"){
                    behaviorActionCfg->mDbgId = aValue;
                    continue;
                }

                const std::string prefix = aName.substr(0,2);

                if(prefix=="on"){
                    behaviorActionCfg->mConditions.push_back(ActionConditionCfg(aName, aValue));

                }else if(prefix=="do"){
                    behaviorActionCfg->mCommands.push_back(ActionCommandCfg(aName, aValue));

                }
            }

        }
    }


    dbgSystem.removeLastMessage();
    return true;
}



}
