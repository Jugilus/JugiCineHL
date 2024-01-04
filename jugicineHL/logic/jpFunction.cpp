#include "pugixml/pugixml.hpp"
#include "jmSystem.h"

#include "jpLogicState.h"
#include "jpPlayedScene.h"
#include "jpObjectFactory.h"
#include "jpLogicStateCfg.h"
#include "jpLogicAction.h"
#include "jpFunction.h"



namespace jugimap{



/*

bool LogicFunctionCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading function '" + mName +"' ...");


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="arguments"){
           std::string sValue = a.as_string();
           std::vector<std::string>parts = StdString::splitString(sValue, ",");

           for(const std::string & s : parts){
               std::vector<std::string>sArg = StdString::splitString(s, ",");

               if(sArg.size()!=2){
                   dbgSystem.addMessage("Error parsing arguments '" + sValue + "'!");
                   return false;
               }

               const std::string & sType = sArg[0];
               const std::string & name = sArg[1];
               BaseObjectType type = BaseObjectType::NOT_DEFINED;

               if(sType=="ITEM"){
                   type = BaseObjectType::ITEM;

               }else{
                   dbgSystem.addMessage("Unknown function argument type '" + sType +"'!");
                   return false;
               }

               FunctionArgument *fa = mArgumentCfg.addObject(FunctionArgument(name, type));
               if(fa==nullptr){
                   return false;
               }
           }


        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="intData"){

            IntSourceItemData *sid = new IntSourceItemData("");
            if(sid->initCfg(n)==false){
                return false;
            }
            if(mSourceData.addObject(sid)==nullptr){
                return false;
            }


        }else if(nodeName=="floatData"){

            FloatSourceItemData *sid = new FloatSourceItemData("");
            if(sid->initCfg(n)==false){
                return false;
            }
            if(mSourceData.addObject(sid)==nullptr){
                return false;
            }


        }else if(nodeName=="boolData"){

            BoolSourceItemData *sid = new BoolSourceItemData("");
            if(sid->initCfg(n)==false){
                return false;
            }
            if(mSourceData.addObject(sid)==nullptr){
                return false;
            }


        }else if(nodeName=="a"){


            mActionsCfgs.push_back(ActionCfg());
            ActionCfg & actionCfg = mActionsCfgs.back();

            for(pugi::xml_attribute a = n.first_attribute(); a; a=a.next_attribute()){
                std::string aName = std::string(a.name());
                std::string aValue = std::string(a.as_string());

                if(aName=="disabled"){
                    actionCfg.mDisabled = a.as_bool(false);
                    continue;

                }else if(aName=="dbgId"){
                    actionCfg.mDbgId = aValue;
                    continue;
                }

                const std::string prefix = aName.substr(0,2);

                if(prefix=="on"){
                    actionCfg.mConditions.push_back(ActionConditionCfg(aName, aValue));

                }else if(prefix=="do"){
                    actionCfg.mCommands.push_back(ActionCommandCfg(aName, aValue));

                }
            }

        }else{

            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}


//===========================================================================================


LogicFunction::LogicFunction(LogicFunctionCfg *_cfg, BaseObject * _parent)
{

    mCfg = _cfg;
    mParentObject = _parent;
    mName = _cfg->name();

    mBaseType = BaseObjectType::LOGIC_FUNCTION;


}



bool LogicFunction::build()
{

    dbgSystem.addMessage("Building logic function '" + mName +"' ...");



    //for(const std::string &s : mCfg->arguments()){
    //    mArguments.addObject(FunctionArgument(s));
    //}


    for(ItemSourceData *isd : mCfg->sourceData().objects()){
        if(isd->mType==ItemDataType::INT){
            mData.addObject(new IntItemData(static_cast<IntSourceItemData*>(isd)));

        }else if(isd->mType==ItemDataType::FLOAT){
            mData.addObject(new FloatItemData(static_cast<FloatSourceItemData*>(isd)));

        }else if(isd->mType==ItemDataType::BOOL){
            mData.addObject(new BoolItemData(static_cast<BoolSourceItemData*>(isd)));

        }
    }

    //---
    for(ActionCfg &cfg : mCfg->actionsCfgs()){
        Action *a = new Action(this);
        if(a->build(cfg)==false){
            return false;
        }
        mActions.push_back(a);
    }


    dbgSystem.removeLastMessage();
    return true;
}


bool LogicFunction::initConnections(PlayedScene *_scene, SimpleNoNameStorage<FunctionArgument> *_arguments, ParentObjects &_parentObjects)
{

    dbgSystem.addMessage("Initializing logic function '" + mName +"' ...");


    mRootParentObject = _parentObjects.rootParentObject;
    mParentState = _parentObjects.parentLogicState;
    mArguments = _arguments;


    dbgSystem.removeLastMessage();
    return true;

}


bool LogicFunction::run(SimpleNoNameStorage<FunctionArgument> *_arguments, ParentObjects &_parentObjects)
{

    mRootParentObject = _parentObjects.rootParentObject;
    mParentState = _parentObjects.parentLogicState;
    mArguments = _arguments;



    //----
    UpdateMode updateMode;

    for(Action* ga : mActions){
        if(ga->update(updateMode)==true){
            //if(updateMode.loopAllStateActions==false){
            //    break;
            //}
        }
    }



    return true;

}


//=================================================================================================




bool FunctionStorage::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading functions ... !");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="function"){

            std::string name = _node.attribute("name").as_string();
            if(name.empty()){
                dbgSystem.addMessage("Missing function name!");
                return false;
            }

            LogicFunctionCfg *functionCfg =  mFunctionCfgs.addObject(new LogicFunctionCfg(name));
            if(functionCfg==nullptr){
                return false;
            }

            if(functionCfg->initCfg(n)==false){
                return false;
            }


        }else{
            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;

        }
    }


    dbgSystem.removeLastMessage();
    return true;
}


bool FunctionStorage::buildObjects(PlayedScene *_scene)
{

    dbgSystem.addMessage("Building functions ... !");

    for(LogicFunctionCfg *cfg : mFunctionCfgs.objects()){

        LogicFunction* function =  mFunctions.addObject(new LogicFunction(cfg, _scene));
        if(function->build()==false){
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;
}

*/



}
