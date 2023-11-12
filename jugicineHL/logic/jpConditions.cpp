#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmAnimationCommon.h"
#include "map/jmSpriteLayer.h"
#include "jmGlobal.h"
#include "jmSystem.h"

#include "jpPlayedScene.h"
#include "jpPlayedApp.h"
#include "jpUtilities.h"
#include "jpConditions.h"


namespace jugimap{



/*

std::string IfCondition::type = "if";


IfCondition::IfCondition(const pugi::xml_node &_node)
{

    mCfg.reset(new Cfg());

    //mParentGameAction = _parentGameAction;
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

*/


//=================================================================================================


std::string ACOnSignal::type = "onSignal";


ACOnSignal::ACOnSignal(Action *_parentAction, const std::string &_value) : ActionCondition(_parentAction)
{
    mValue = _value;
}


bool ACOnSignal::initConnections(PlayedScene *scene)
{

    dbgSystem.addMessage("Initializing '" + type +"' ...");


    //Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    //if(actor->sourceEntity()->name()=="wallSwitchB"){
    //    DummyFunction();
    //}

    BaseObject * parentObject = mParentAction->parentObject();
    LogicState * parentState = nullptr;
    if(parentObject->baseType()==BaseObjectType::LOGIC_STATE){
        parentState = static_cast<LogicState*>(parentObject);
    }

    BaseObject * rootParentObject = nullptr;
    if(parentState){
        rootParentObject = parentState->rootParentObject();
    }


    //app->signalParserManager()->parseSignalAccessor(scene, mValue, mSignalQuery, actor, mParentBehaviorAction->parentBehaviorState());
    //app->signalParserManager()->parseSignalAccessor(scene, mValue, mSignalQuery, mThisSignalOrigin, mParentState);
    app->signalParserManager()->parseSignalAccessor(scene, mValue, mSignalQuery, rootParentObject, parentObject);

    if(mSignalQuery.isValid()==false){
        return false;
    }

    /*
    if(mSignalQuery.mSignal->id() == static_cast<unsigned char>(SignalID::CUSTOM_ENTITY_SIGNAL)){
        mParentBehaviorAction->parentBehaviorState()->customEntitySignals().push_back(mSignalQuery.mSignal);
        mSignalQuery.mOwnedSignal = true;   // custom signals are owned by signal accessors
    }
    */

    dbgSystem.removeLastMessage();
    return true;

}


bool ACOnSignal::isTrue()
{

    //Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    if(mSignalQuery.active()){
        return true;
    }

   return false;

}





//------------------------------------------------------------------------------------



std::string ACOnSignals::type = "onSignals";


ACOnSignals::ACOnSignals(Action *_parentAction, const std::string &_value) : ActionCondition(_parentAction)
{
    mValue = _value;
}



bool ACOnSignals::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing '" + type + "' ...");


    //Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    //if(actor->sourceEntity()->sourceEntityCfg()->name=="fancyPlatformA_light"){
    //    DummyFunction();
    //}

    if(mCompositeQuery.parse(mValue)==false){
        return false;
    }

    //if(mParentBehaviorAction->parentBehaviorState()->behaviorStateCfg()->name=="standBy"){
    //    DummyFunction();
    //}

    BaseObject * parentObject = mParentAction->parentObject();
    LogicState * parentState = nullptr;
    if(parentObject->baseType()==BaseObjectType::LOGIC_STATE){
        parentState = static_cast<LogicState*>(parentObject);
    }

    BaseObject * rootParentObject = nullptr;
    if(parentState){
        rootParentObject = parentState->rootParentObject();
    }


    //if(mCompositeQuery.initConnections(_scene, actor, mParentBehaviorAction->parentBehaviorState())==false){
    //if(mCompositeQuery.initConnections(_scene, mThisSignalOrigin, mParentState)==false){
    if(mCompositeQuery.initConnections(_scene, rootParentObject, parentObject)==false){
        return false;
    }


    dbgSystem.removeLastMessage();
    return true;

}



bool ACOnSignals::isTrue()
{

    //Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    //if(mParentBehaviorAction->parentBehaviorState()->behaviorStateCfg()->name=="standBy"){
    //    DummyFunction();
    //}

    return mCompositeQuery.active();

}



}
