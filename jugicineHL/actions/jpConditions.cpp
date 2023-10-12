#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmGuiCommon.h"
#include "jmGuiBar.h"
#include "jmAnimationCommon.h"
#include "map/jmSpriteLayer.h"
#include "jmGlobal.h"
#include "jmSystem.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"
#include "jpConditions.h"


namespace jugimap{




std::string IfCondition::type = "if";

IfCondition::IfCondition(Action *_parentGameAction, const pugi::xml_node &_node)
{

    mCfg.reset(new Cfg());

    mParentGameAction = _parentGameAction;
    mCfg->mVariables = _node.attribute("variables").as_string("");

}


bool IfCondition::initConnections(PlayedScene *scene)
{

    dbgSystem.addMessage("init '" + type + "'");

    std::vector<std::string>parts = StdString::splitString(mCfg->mVariables, ",");
    for(const std::string &s : parts){

        std::vector<std::string>var_value = StdString::splitString(s, ":");
        if(var_value.size()!=2){
            dbgSystem.addMessage("Wrong variable path '" + s + "'!");
            return false;
        }

        const std::string &varPath = var_value[0];
        const std::string &varValuePath = var_value[1];

        std::string directValue;
        Variable *var = ObtainVariableFromPath(varPath, scene, directValue);
        if(var==nullptr){
            return false;
        }

        //----
        Variable *varValue = ObtainVariableFromPath(varValuePath, scene, directValue);
        if(varValue==nullptr && directValue.empty()){
            return false;
        }
        if(varValue){
            if(varValue->type()!=var->type()){
                dbgSystem.addMessage("Variable '" + varPath +"' and comparing value '" + varValuePath +"' have differemt type!" );
            }
        }
        if(directValue.empty()==false){
            if(directValue=="''"){      // for empty string
                directValue = "";
            }
            varValue = scene->localVariablesStorage().addAnonymousVariable("", directValue, var->type());
        }

        //---
        mVariables.push_back(var);
        mVariableValues.push_back(varValue);
    }

    dbgSystem.removeLastMessage();
    return true;

}


bool IfCondition::isConditionTrue()
{

    for(unsigned int i=0; i<mVariables.size(); i++){
        if(Variable::haveSameValues(mVariables[i], mVariableValues[i])==false){
            return false;
        }
    }

   return true;

}




}
