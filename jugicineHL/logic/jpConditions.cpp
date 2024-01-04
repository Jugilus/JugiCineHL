#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmAnimationCommon.h"
#include "map/jmSpriteLayer.h"
#include "jmGlobal.h"
#include "jmSystem.h"

#include "data/jpData.h"
#include "jpPlayedScene.h"
#include "jpPlayedApp.h"
#include "jpUtilities.h"
#include "jpLogicState.h"
#include "jpObjectParser.h"
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

    /*
    BaseObject * parentObject = mParentAction->parentObject();
    LogicState * parentState = nullptr;
    if(parentObject->baseType()==BaseObjectType::LOGIC_STATE){
        parentState = static_cast<LogicState*>(parentObject);
    }

    BaseObject * rootParentObject = nullptr;
    if(parentState){
        rootParentObject = parentState->rootParentObject();
    }
    */

    mParentState = mParentAction->parentLogicState();
    mRootParentObject = mParentState->rootParentObject();


    //app->signalParserManager()->parseSignalAccessor(scene, mValue, mSignalQuery, actor, mParentBehaviorAction->parentBehaviorState());
    //app->signalParserManager()->parseSignalAccessor(scene, mValue, mSignalQuery, mThisSignalOrigin, mParentState);
    app->signalParserManager()->parseSignalAccessor(scene, mValue, mSignalQuery, mRootParentObject, mParentState);

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

    if(mSignalQuery.notUsed()){
        return true;
    }

    if(mSignalQuery.active()){

        //BaseObject *signalDataObject = mSignalQuery.signal()->dataObject();
        //if(signalDataObject && mParentState){
        //    mParentState->storedObjectsPerFrame().push_back(signalDataObject);
        //}

        return true;
    }

   return false;

}



//=================================================================================================


std::string ACOnSignalNEW::type = "onSignalNEW";


ACOnSignalNEW::ACOnSignalNEW(Action *_parentAction, const std::string &_value) : ActionCondition(_parentAction)
{
    mValue = _value;
}

ACOnSignalNEW::ACOnSignalNEW(ActionTrigger *_parentAction, const std::string &_value) : ActionCondition(_parentAction)
{
    mValue = _value;
}


bool ACOnSignalNEW::initConnections(PlayedScene *scene)
{
    dbgSystem.addMessage("Initializing '" + type +"' ...");

    if(mValue=="ENTITY:THIS/CUSTOM_ACT:throwingItem"){
        DummyFunction();
    }

    if(mParentAction){
        mParentState = mParentAction->parentLogicState();
        mRootParentObject = mParentState->rootParentObject();

        if(mSignalQuery.parse(scene, mValue, mRootParentObject, mParentState)==false){
            return false;
        }

    }else if(mParentActionTrigger){
        mRootParentObject = mParentActionTrigger->parentObject();

        if(mSignalQuery.parse(scene, mValue, mRootParentObject, nullptr)==false){
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;
}


bool ACOnSignalNEW::isTrue()
{

    //Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();
    if(mValue=="ENTITY:THIS/CONTACT:ANY/START"){
        DummyFunction();
    }

    if(mSignalQuery.active()){

        if(mValue=="ENTITY:CONTACTED:iGamepad/CONTACT:ANY/START"){
            DummyFunction();
        }

        //BaseObject *signalDataObject = mSignalQuery.signal()->dataObject();
       // if(signalDataObject && mParentState){
       //     mParentState->storedObjectsPerFrame().push_back(signalDataObject);
       // }

        return true;
    }

   return false;

}




//=================================================================================================



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


    /*
    BaseObject * parentObject = mParentAction->parentObject();
    LogicState * parentState = nullptr;
    if(parentObject->baseType()==BaseObjectType::LOGIC_STATE){
        parentState = static_cast<LogicState*>(parentObject);
    }

    BaseObject * rootParentObject = nullptr;
    if(parentState){
        rootParentObject = parentState->rootParentObject();
    }
    */

    LogicState * parentState = mParentAction->parentLogicState();
    BaseObject * rootParentObject = parentState->rootParentObject();


    //if(mCompositeQuery.initConnections(_scene, actor, mParentBehaviorAction->parentBehaviorState())==false){
    //if(mCompositeQuery.initConnections(_scene, mThisSignalOrigin, mParentState)==false){
    if(mCompositeQuery.initConnections(_scene, rootParentObject, parentState)==false){
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


//------------------------------------------------------------------------------------



std::string ACOnSignalsNEW::type = "onSignalsNEW";


ACOnSignalsNEW::ACOnSignalsNEW(Action *_parentAction, const std::string &_value) : ActionCondition(_parentAction)
{
    mValue = _value;
}


ACOnSignalsNEW::ACOnSignalsNEW(ActionTrigger *_parentAction, const std::string &_value) : ActionCondition(_parentAction)
{
    mValue = _value;
}


bool ACOnSignalsNEW::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing '" + type + "' ...");

    if(mValue == "ENTITY:CONTACTED:iGamepad/CONTACT:THIS/START=TRUE , ENTITY:CONTACTED:iGamepad/ITEM_PICKABLE=TRUE"){
        DummyFunction();
    }

    if(mCompositeQuery.parseNEW(mValue)==false){
        return false;
    }


    if(mParentAction){
        mParentState = mParentAction->parentLogicState();
        mRootParentObject = mParentState->rootParentObject();

        if(mCompositeQuery.initConnectionsNEW(_scene, mRootParentObject, mParentState)==false){
            return false;
        }

    }else if(mParentActionTrigger){

        mRootParentObject = mParentActionTrigger->parentObject();

        if(mCompositeQuery.initConnectionsNEW(_scene, mRootParentObject, nullptr)==false){
            return false;
        }

    }



    dbgSystem.removeLastMessage();
    return true;

}


bool ACOnSignalsNEW::isTrue()
{

    //Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    //if(mParentBehaviorAction->parentBehaviorState()->behaviorStateCfg()->name=="standBy"){
    //    DummyFunction();
    //}

    return mCompositeQuery.active();

}


//=================================================================================================

/*

std::string ACOnData::type = "onData";


ACOnData::ACOnData(Action *_parentAction, const std::string &_value) : ActionCondition(_parentAction)
{
    mValue = _value;
}



bool ACOnData::initConnections(PlayedScene *scene)
{

    dbgSystem.addMessage("Initializing '" + type +"' ...");


    LogicState * parentState = mParentAction->parentLogicState();
    BaseObject * rootParentObject = parentState->rootParentObject();


    std::vector<std::string>parts = StdString::splitString(mValue, "=");
    if(parts.size()!=2){
        dbgSystem.addMessage("Error parsing string '" + mValue + "'!");
        return false;
    }

    const std::string & leftSide = parts[0];
    const std::string & rightSide = parts[1];

    if(leftSide.substr(0,6)=="LOCAL/"){
        std::string leftSidePath = leftSide.substr(6);
        mDataQuery.mLeftSideItemDataPair.data = parentState->dataStorage().getObject(leftSidePath);

    }else if(leftSide.substr(0,5)=="ITEM/"){
        std::string leftSidePath = leftSide.substr(5);
        app->signalParserManager()->parseItemDataPair(scene, leftSidePath, mDataQuery.mLeftSideItemDataPair, ParseItemMode::DATA, rootParentObject, parentState);
    }

    if(mDataQuery.mLeftSideItemDataPair.data==nullptr){
        return false;
    }


    //----
    if(rightSide.substr(0,6)=="LOCAL/"){
        std::string rightSidePath = rightSide.substr(6);
        mDataQuery.mRightSideItemDataPair.data = parentState->dataStorage().getObject(rightSidePath);

        if(mDataQuery.mRightSideItemDataPair.data==nullptr){
            return false;
        }

    }else if(rightSide.substr(0,5)=="ITEM:"){
        std::string rightSidePath = rightSide.substr(5);
        app->signalParserManager()->parseItemDataPair(scene, rightSidePath, mDataQuery.mRightSideItemDataPair, ParseItemMode::DATA, rootParentObject, parentState);

        if(mDataQuery.mRightSideItemDataPair.data==nullptr){
            return false;
        }

    }else if(rightSide=="MIN_VALUE"){
        if(mDataQuery.mLeftSideItemDataPair.data->type()==ItemDataType::INT){
            mDataQuery.mDirectValues.mIntValue = static_cast<IntItemData*>(mDataQuery.mLeftSideItemDataPair.data)->minValue();

        }else if(mDataQuery.mLeftSideItemDataPair.data->type()==ItemDataType::FLOAT){
            mDataQuery.mDirectValues.mFloatValue = static_cast<FloatItemData*>(mDataQuery.mLeftSideItemDataPair.data)->minValue();
        }

    }else if(rightSide=="MAX_VALUE"){
        if(mDataQuery.mLeftSideItemDataPair.data->type()==ItemDataType::INT){
            mDataQuery.mDirectValues.mIntValue = static_cast<IntItemData*>(mDataQuery.mLeftSideItemDataPair.data)->maxValue();

        }else if(mDataQuery.mLeftSideItemDataPair.data->type()==ItemDataType::FLOAT){
            mDataQuery.mDirectValues.mFloatValue = static_cast<FloatItemData*>(mDataQuery.mLeftSideItemDataPair.data)->maxValue();
        }

    }else{      // numeric

        if(mDataQuery.mLeftSideItemDataPair.data->type()==ItemDataType::INT){
            if(StdString::integerNumber(rightSide, mDataQuery.mDirectValues.mIntValue)==false){
                return false;
            }

        }else if(mDataQuery.mLeftSideItemDataPair.data->type()==ItemDataType::FLOAT){
            if(StdString::floatNumber(rightSide, mDataQuery.mDirectValues.mFloatValue)==false){
                return false;
            }

        }else if(mDataQuery.mLeftSideItemDataPair.data->type()==ItemDataType::BOOL){
            if(StdString::boolValue(rightSide, mDataQuery.mDirectValues.mBoolValue)==false){
                return false;
            }
        }
    }


    if(mDataQuery.mRightSideItemDataPair.data){
        if(mDataQuery.mLeftSideItemDataPair.data->type() != mDataQuery.mRightSideItemDataPair.data->type()){
            dbgSystem.addMessage("Data on the right side is of different type than the data on the left side!");
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}



bool ACOnData::isTrue()
{

    //Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    if(mDataQuery.isTrue()){
        return true;
    }

   return false;

}

*/

//=================================================================================================



std::string ACOnDataNEW::type = "onDataNEW";


ACOnDataNEW::ACOnDataNEW(Action *_parentAction, const std::string &_value) : ActionCondition(_parentAction)
{
    mValue = _value;
}


ACOnDataNEW::ACOnDataNEW(ActionTrigger *_parentAction, const std::string &_value) : ActionCondition(_parentAction)
{
    mValue = _value;
}


bool ACOnDataNEW::initConnections(PlayedScene *scene)
{

    dbgSystem.addMessage("Initializing '" + type +"' ...");


    if(mParentAction){

        LogicState * parentState = mParentAction->parentLogicState();
        BaseObject * rootParentObject = parentState->rootParentObject();

        if(mDataQuery.parse(scene, mValue, rootParentObject, parentState, ParseItemMode::DATA)==false){
            return false;
        }

    }else if(mParentActionTrigger){

        BaseObject * rootParentObject = mParentActionTrigger->parentObject();

        if(mDataQuery.parse(scene, mValue, rootParentObject, nullptr, ParseItemMode::DATA)==false){
            return false;
        }

    }



    dbgSystem.removeLastMessage();
    return true;

}



bool ACOnDataNEW::isTrue()
{

    if(mDataQuery.accessorNotUsed()){
        return true;
    }

    if(mDataQuery.isTrue()){
        return true;
    }

   return false;

}


//=================================================================================================



std::string ACOnItem::type = "onItem";


ACOnItem::ACOnItem(Action *_parentAction, const std::string &_value) : ActionCondition(_parentAction)
{
    mValue = _value;
}

ACOnItem::ACOnItem(ActionTrigger *_parentAction, const std::string &_value) : ActionCondition(_parentAction)
{
    mValue = _value;
}



bool ACOnItem::initConnections(PlayedScene *scene)
{

    dbgSystem.addMessage("Initializing '" + type +"' ...");


    if(mParentAction){
        LogicState * parentState = mParentAction->parentLogicState();
        BaseObject * rootParentObject = parentState->rootParentObject();

        if(mItemQuery.parse(scene, mValue, rootParentObject, parentState, ParseItemMode::ITEM)==false){
            return false;
        }

    }else if(mParentActionTrigger){
        BaseObject * rootParentObject = mParentActionTrigger->parentObject();

        if(mItemQuery.parse(scene, mValue, rootParentObject, nullptr, ParseItemMode::ITEM)==false){
            return false;
        }

    }


    dbgSystem.removeLastMessage();
    return true;

}



bool ACOnItem::isTrue()
{

    if(mItemQuery.accessorNotUsed()){
        return true;
    }

    if(mItemQuery.isTrue()){
        return true;
    }

   return false;

}


}
