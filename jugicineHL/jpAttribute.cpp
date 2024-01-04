#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmSprite.h"
#include "jmCompositeSprite.h"
#include "jmStreams.h"

#include "logic/jpLogicState.h"
#include "jpPlayedScene.h"
#include "jpPlayedApp.h"

#include "jpAttribute.h"


namespace jugimap{



bool AttributeSetCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading attribute set '" + mName +"' ...");


    for(pugi::xml_node nChild = _node.first_child(); nChild; nChild = nChild.next_sibling()){
        std::string childNodeName = std::string(nChild.name());

        if(childNodeName=="boolean" ){

            attributeCfgs.emplace_back();
            AttributeCfg &aCfg = attributeCfgs.back();
            aCfg.type = SignalType::BOOL;

            for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="name"){
                    aCfg.name = a.as_string("name");

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                    return false;
                }
            }


        }else if(childNodeName=="integer" || childNodeName=="decimal"){

            attributeCfgs.emplace_back();
            AttributeCfg &aCfg = attributeCfgs.back();

            if(childNodeName=="integer"){
                aCfg.type = SignalType::INT;

            }else if(childNodeName=="decimal"){
                aCfg.type = SignalType::FLOAT;
            }

            for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="name"){
                    aCfg.name = a.as_string("name");

                }else if(attributeName=="minValue"){
                    aCfg.minValue = a.as_string("0");

                }else if(attributeName=="maxValue"){
                    aCfg.maxValue = a.as_string("100");

                }else if(attributeName=="startValue"){
                    aCfg.startValue = a.as_string("0");

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + childNodeName +"' !");
                    return false;
                }
            }

        }else{
            dbgSystem.addMessage("Unknown node '" + childNodeName + "' !");
            return false;
        }
    }

    dbgSystem.removeLastMessage();
    return true;

}


//====================================================================================


bool AttributeSet::build()
{

    for(AttributeSetCfg::AttributeCfg & aCfg : mCfg->attributeCfgs){

        if(aCfg.type==SignalType::BOOL){

            BoolSignal * bs = new BoolSignal(aCfg.name, false);

            mAttributeSignals.push_back(bs);


        }else if(aCfg.type==SignalType::INT){

            IntSignal *sig = new IntSignal(aCfg.name, 0);

            IntSignalExtraData *data = sig->createExtraData();

            data->minValue = StdString::stringToInt(aCfg.minValue, 0);
            data->maxValue = StdString::stringToInt(aCfg.maxValue, 0);
            int startValue = StdString::stringToInt(aCfg.startValue, 0);
            ClampValue(startValue, data->minValue, data->maxValue);

            sig->setDefaultValue(startValue);
            sig->reset();

            mAttributeSignals.push_back(sig);


        }else if(aCfg.type==SignalType::FLOAT){

            FloatSignal *sig = new FloatSignal(aCfg.name, 0);
            FloatSignalExtraData *data = sig->createExtraData();

            data->minValue = StdString::stringToFloat(aCfg.minValue, 0);
            data->maxValue = StdString::stringToFloat(aCfg.maxValue, 0);
            float startValue = StdString::stringToFloat(aCfg.startValue, 0);
            ClampValue(startValue, data->minValue, data->maxValue);

            sig->setDefaultValue(startValue);
            sig->reset();

            mAttributeSignals.push_back(sig);

        }else{

            assert(false);

        }


    }

    return true;

}



//====================================================================================


bool AttributeLogicStateCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading state '" + mName + "' ...");

    //---- attributes

    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
           mName = a.as_string("");

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



LogicStateCfg* AttributeLogicStateCfg::createLogicStateCfg(const std::string &_name)
{
    return new AttributeLogicStateCfg(_name);
}


CustomLogicState* AttributeLogicStateCfg::createCustomState(LogicState *_parentState)
{

    return new AttributeCustomState(this, _parentState);

}

//==============================================================================================


AttributeCustomState::AttributeCustomState(AttributeLogicStateCfg *_behaviorStateCfg, LogicState* _parent) :
    mParentState(_parent),
    mLogicStateCfg(_behaviorStateCfg)
{

}


bool AttributeCustomState::initConnections(PlayedScene *_scene)
{

    return true;

}



void AttributeCustomState::start()
{


}


void AttributeCustomState::finish()
{
}





}
