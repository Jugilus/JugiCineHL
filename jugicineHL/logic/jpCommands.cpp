#include "ncine/Application.h"
#include "pugixml/pugixml.hpp"
#include "jmAnimationCommon.h"
#include "map/jmSpriteLayer.h"
#include "map/jmMap.h"
#include "jmGlobal.h"
#include "jmScene.h"
#include "jmSprite.h"
#include "jmTextSprite.h"
#include "jmSystem.h"
#include "jmSceneLayout.h"
#include "jmUtilities.h"

//#include "gui/jpGuiCommon.h"
//#include "jmGuiBar.h"
//#include "jmGuiTextInput.h"
#include "gui/widgets/jpGuiTable.h"
#include "gui/widgets/jpGuiTableNew.h"
//#include "gui/widgets/jpGuiSlider.h"

//#include "character/jpCharacterAction.h"
#include "data/jpData.h"
#include "data/jpItem.h"


#include "jpSceneLogicState.h"
#include "jpCompound.h"
#include "jpVariables.h"
#include "jpQueries.h"
#include "jpPlayedScene.h"
#include "jpPlayedApp.h"
#include "jpUtilities.h"
#include "jpTransition.h"
#include "jpSettings.h"
#include "jpSystemVarious.h"
#include "jpGfxObjectsCommon.h"
#include "jpComponentsCommon.h"
#include "jpItemsCommon.h"
#include "jpFunction.h"
#include "jpUser.h"
#include "jpObjectParser.h"
#include "jpCommands.h"



namespace jugimap{




//---------------------------------------------------------------------------


std::string SetNextState::doType = "doSetState";



SetNextState::SetNextState(Action*_parentAction, const std::string &_value) : ActionCommand(_parentAction)
{
    mValue = _value;
}


bool SetNextState::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing '" + doType + "' ...");


    assert(mParentAction->parentObject()->baseType()==BaseObjectType::LOGIC_STATE);
    LogicState * currentState = static_cast<LogicState*>(mParentAction->parentObject());

    mGameState = ObtainLogicStateFromPath(_scene, currentState, mValue);


    if(mGameState==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}



void SetNextState::run(UpdateMode &_updateMode)
{

    if(mGameState->parentObject()->baseType()==BaseObjectType::LOGIC_STATE){
        LogicState *parentState = static_cast<LogicState*>(mGameState->parentObject());
        parentState->setNextChildState(mGameState);
    }

}


//---------------------------------------------------------------------------


std::string DoSetSignal::doType = "doSetSignal";


DoSetSignal::DoSetSignal(Action *_parentGameAction, const std::string &_value) : ActionCommand(_parentGameAction)
{
    mValue = _value;
}


bool DoSetSignal::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing '" + doType + "' ...");


    //Entity *actor = mParentBehaviorAction->parentBehaviorState()->parentStateManager()->parentActor();

    //if(actor->sourceEntity()->name()=="wallSwitchB"){
    //    DummyFunction();
    //}

    if(mValue=="ENTITY:THIS/MOVABLE_OBJECT:MOVED=TRUE"){
        DummyFunction();
    }


    BaseObject * parentObject = mParentAction->parentObject();
    LogicState * parentState = nullptr;
    if(parentObject->baseType()==BaseObjectType::LOGIC_STATE){
        parentState = static_cast<LogicState*>(parentObject);
    }

    BaseObject * rootParentObject = nullptr;
    if(parentState){
        rootParentObject = parentState->rootParentObject();
    }


    std::string signalPath = mValue;
    std::string dataAsSignal;

    std::vector<std::string>parts = StdString::splitString(mValue, "=");
    if(parts.size()==2){
        const std::string & value = parts[1];
        if(value.substr(0,7) == "SIGNAL/"){
            dataAsSignal = value.substr(7);
            signalPath = parts[0];
        }
    }

    if(dataAsSignal.empty()==false){
        SignalQuery tmpQuery;
        tmpQuery._setValueNotRequired(true);
        //mValueNotRequired = true;   // !!
        app->signalParserManager()->parseSignalAccessor(_scene, dataAsSignal, tmpQuery, rootParentObject, parentObject);

        if(tmpQuery.isValid()==false){
            return false;
        }

        mSignalSetter.mRightSide.mSigData = tmpQuery.mSignal;
        mSignalSetter.mRightSide.mSigDataFlag = tmpQuery.mRightSide.mIntValue;
    }


    app->signalParserManager()->parseSignalAccessor(_scene, signalPath, mSignalSetter, rootParentObject, parentObject);

    if(mSignalSetter.isValid()==false){
        return false;
    }

    //---
    if(mSignalSetter.mRightSide.mSigData){
        if(mSignalSetter.mSignal->type() != mSignalSetter.mRightSide.mSigData->type()){
            dbgSystem.addMessage("Data signal is not of the same type as the signal which is being set!");
            return false;
        }
    }

    /*
    if(mSignalSetter.mSignal->id() == static_cast<unsigned char>(SignalID::CUSTOM_ENTITY_SIGNAL)){
        if(parentState){
            parentState->customUpdatedSignals().push_back(mSignalSetter.mSignal);
            //mParentBehaviorAction->parentBehaviorState()->customEntitySignals().push_back(mSignalSetter.mSignal);
            mSignalSetter.mOwnedSignal = true;   // custom signals are owned by signal accessors
        }
    }
    */

    dbgSystem.removeLastMessage();
    return true;

}


void DoSetSignal::run(UpdateMode &_updateMode)
{


    mSignalSetter.set();


}

//---------------------------------------------------------------------------


std::string DoSetSignalNEW::doType = "doSetSignalNEW";


DoSetSignalNEW::DoSetSignalNEW(Action *_parentGameAction, const std::string &_value) : ActionCommand(_parentGameAction)
{
    mValue = _value;
}


bool DoSetSignalNEW::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing '" + doType + "' ...");


    if(mValue=="ENTITY:THIS/MOVABLE_OBJECT:MOVED=TRUE"){
        DummyFunction();
    }

    LogicState * parentState = mParentAction->parentLogicState();
    BaseObject * rootParentObject = parentState->rootParentObject();

    if(mSignalSetter.parse(_scene, mValue, rootParentObject, parentState)==false){
        return false;
    }


    dbgSystem.removeLastMessage();
    return true;

}


void DoSetSignalNEW::run(UpdateMode &_updateMode)
{

    mSignalSetter.set();

}


//---------------------------------------------------------------------------
/*

std::string DoSetData::doType = "doSetData";


DoSetData::DoSetData(Action *_parentGameAction, const std::string &_value) : ActionCommand(_parentGameAction)
{
    mValue = _value;
}


bool DoSetData::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing '" + doType + "' ...");


    LogicState * parentState = mParentAction->parentLogicState();
    BaseObject * rootParentObject = parentState->rootParentObject();


    std::vector<std::string>parts = StdString::splitString(mValue, "=");
    if(parts.size()!=2){
        dbgSystem.addMessage("Error parsing string '" + mValue + "'!");
        return false;
    }

    const std::string & leftSide = parts[0];
    const std::string & fullRightSide = parts[1];

    if(leftSide.substr(0,6)=="LOCAL/"){
        std::string leftSidePath = leftSide.substr(6);
        mDataSetter.mLeftSideItemDataPair.data = parentState->dataStorage().getObject(leftSidePath);

    }else if(leftSide.substr(0,5)=="ITEM/"){
        std::string leftSidePath = leftSide.substr(5);
        app->signalParserManager()->parseItemDataPair(_scene, leftSidePath, mDataSetter.mLeftSideItemDataPair, ParseItemMode::DATA, rootParentObject, parentState);
    }

    if(mDataSetter.mLeftSideItemDataPair.data==nullptr){
        return false;
    }

    ItemDataType leftDataType = mDataSetter.mLeftSideItemDataPair.data->type();


    //----

    std::string rightSide;
    if(fullRightSide.substr(0,4)=="ADD/"){
        mDataSetter.mArithemticOperation = ArithemticOperator::ADD;
        rightSide = fullRightSide.substr(4);

    }else if(fullRightSide.substr(0,9)=="SUBTRACT/"){
        mDataSetter.mArithemticOperation = ArithemticOperator::SUBTRACT;
        rightSide = fullRightSide.substr(9);

    }else if(fullRightSide.substr(0,9)=="MULTIPLY/"){
        mDataSetter.mArithemticOperation = ArithemticOperator::MULTIPLY;
        rightSide = fullRightSide.substr(9);

    }else{
        mDataSetter.mArithemticOperation = ArithemticOperator::NONE;
        rightSide = fullRightSide;
    }

    if(mDataSetter.mArithemticOperation != ArithemticOperator::NONE){
        if(leftDataType != ItemDataType::INT && leftDataType != ItemDataType::FLOAT){
            dbgSystem.addMessage("Error! Arithemtic operations specified for a non-arithmetic data!");
            return false;
        }
    }

    StdString::removeStringWhiteSpacesOnStartAndEnd(rightSide);

    if(rightSide.substr(0,6)=="LOCAL/"){
        std::string rightSidePath = rightSide.substr(6);
        mDataSetter.mRightSideItemDataPair.data = parentState->dataStorage().getObject(rightSidePath);

        if(mDataSetter.mRightSideItemDataPair.data==nullptr){
            return false;
        }

    }else if(rightSide.substr(0,5)=="ITEM/"){
        std::string rightSidePath = rightSide.substr(5);
        app->signalParserManager()->parseItemDataPair(_scene, rightSidePath, mDataSetter.mRightSideItemDataPair, ParseItemMode::DATA, rootParentObject, parentState);

        if(mDataSetter.mRightSideItemDataPair.data==nullptr){
            return false;
        }

    }else if(rightSide=="MIN_VALUE"){
        if(mDataSetter.mLeftSideItemDataPair.data->type()==ItemDataType::INT){
            mDataSetter.mDirectValues.mIntValue = static_cast<IntItemData*>(mDataSetter.mLeftSideItemDataPair.data)->minValue();

        }else if(mDataSetter.mLeftSideItemDataPair.data->type()==ItemDataType::FLOAT){
            mDataSetter.mDirectValues.mFloatValue = static_cast<FloatItemData*>(mDataSetter.mLeftSideItemDataPair.data)->minValue();
        }

    }else if(rightSide=="MAX_VALUE"){
        if(mDataSetter.mLeftSideItemDataPair.data->type()==ItemDataType::INT){
            mDataSetter.mDirectValues.mIntValue = static_cast<IntItemData*>(mDataSetter.mLeftSideItemDataPair.data)->maxValue();

        }else if(mDataSetter.mLeftSideItemDataPair.data->type()==ItemDataType::FLOAT){
            mDataSetter.mDirectValues.mFloatValue = static_cast<FloatItemData*>(mDataSetter.mLeftSideItemDataPair.data)->maxValue();
        }

    }else{      // numeric

        if(mDataSetter.mLeftSideItemDataPair.data->type()==ItemDataType::INT){
            if(StdString::integerNumber(rightSide, mDataSetter.mDirectValues.mIntValue)==false){
                return false;
            }

        }else if(mDataSetter.mLeftSideItemDataPair.data->type()==ItemDataType::FLOAT){
            if(StdString::floatNumber(rightSide, mDataSetter.mDirectValues.mFloatValue)==false){
                return false;
            }

        }else if(mDataSetter.mLeftSideItemDataPair.data->type()==ItemDataType::BOOL){
            if(StdString::boolValue(rightSide, mDataSetter.mDirectValues.mBoolValue)==false){
                return false;
            }
        }
    }


    if(mDataSetter.mRightSideItemDataPair.data){
        if(mDataSetter.mLeftSideItemDataPair.data->type() != mDataSetter.mRightSideItemDataPair.data->type()){
            dbgSystem.addMessage("Data on the right side is of different type than the data on the left side!");
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}


void DoSetData::run(UpdateMode &_updateMode)
{

    mDataSetter.set();

}
*/

//---------------------------------------------------------------------------


std::string DoSetDataNEW::doType = "doSetDataNEW";


DoSetDataNEW::DoSetDataNEW(Action *_parentGameAction, const std::string &_value) : ActionCommand(_parentGameAction)
{
    mValue = _value;
}


bool DoSetDataNEW::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing '" + doType + "' ...");


    LogicState * parentState = mParentAction->parentLogicState();
    BaseObject * rootParentObject = parentState->rootParentObject();


    if(mDataSetter.parse(_scene, mValue, rootParentObject, parentState, ParseItemMode::DATA) == false){
        return false;
    }


    dbgSystem.removeLastMessage();
    return true;

}


void DoSetDataNEW::run(UpdateMode &_updateMode)
{

    if(mDataSetter.accessorNotUsed()){
        return;
    }

    mDataSetter.set();

}



//---------------------------------------------------------------------------



std::string DoSetItem::doType = "doSetItem";


DoSetItem::DoSetItem(Action *_parentGameAction, const std::string &_value) : ActionCommand(_parentGameAction)
{
    mValue = _value;
}


bool DoSetItem::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing '" + doType + "' ...");


    LogicState * parentState = mParentAction->parentLogicState();
    BaseObject * rootParentObject = parentState->rootParentObject();


    if(mItemMover.parse(_scene, mValue, rootParentObject, parentState, ParseItemMode::ITEM) == false){
        return false;
    }


    dbgSystem.removeLastMessage();
    return true;
}


void DoSetItem::run(UpdateMode &_updateMode)
{

    if(mItemMover.accessorNotUsed()){
        return;
    }

    mItemMover.set();

}


//---------------------------------------------------------------------------


std::string AShowOverlayCompound::doType = "doShowOverlayCompound";


AShowOverlayCompound::AShowOverlayCompound(Action*_parentGameAction, const std::string &_value) : ActionCommand(_parentGameAction)
{
    mValue = _value;
}


bool AShowOverlayCompound::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + doType + "'");


    if(mValue.empty()==false){


        std::vector<std::string>parts = StdString::splitString(mValue, "/");
        if(parts.size() < 1){
            dbgSystem.addMessage("Parsing string '" +mValue + "' error!");
            return false;
        }

        std::string compound;
        std::string data;

        if(parts.size() > 0){
            compound = parts[0];
        }
        if(parts.size() > 1){
            data = parts[1];
        }

        if(compound.empty()){
            dbgSystem.addMessage("Parsing string '" +mValue + "' error!");
            return false;
        }

        mCompound = _scene->compoundStorage()->getCompund(compound);
        if(mCompound==nullptr){
            return false;
        }

        if(mCompound->overlay()==false){
            dbgSystem.addMessage("Compound '" + mCompound->name() + "' is not an overlay compound!");
            return false;
        }

        //---
        if(data.empty()==false){
            std::vector<std::string>dataElements = StdString::splitString(data, ",");

            for(const std::string &dataElement : dataElements){

                if(dataElement.size()>14 && dataElement.substr(0,13)=="TEXT_SEGMENT:"){

                    std::string dataContent = dataElement.substr(13);

                    TextSegment *ts = ObtainTextSegment2(dataContent);
                    if(ts==nullptr){
                        return false;
                    }

                    if(mComponentData.get()==nullptr){
                        mComponentData.reset(new ComponentData());
                    }

                    mComponentData->mTextSegments.push_back(ts);
                }

            }
        }



    }else{

        mCompound = _scene->compoundStorage()->getCompund(mCfg->mCompound);
        if(mCompound==nullptr){
            return false;
        }

        if(mCompound->overlay()==false){
            dbgSystem.addMessage("Compound '" + mCompound->name() + "' is not an overlay compound!");
            return false;
        }


        //DATA
        if(mCfg->mTextSegment.empty()==false){
            TextSegment *ts = ObtainTextSegment(mCfg->mTextSegment);
            if(ts==nullptr){
                return false;
            }

            if(mComponentData.get()==nullptr){
                mComponentData.reset(new ComponentData());
            }

            mComponentData->mTextSegments.push_back(ts);
        }
    }

    dbgSystem.removeLastMessage();
    return true;

}


void AShowOverlayCompound::run(UpdateMode &_updateMode)
{

    LogicState *currentState =  static_cast<LogicState*>(mParentAction->parentObject());

    mCompound->component()->setData(mComponentData.get());

    SceneCustomLogicState *data = dynamic_cast<SceneCustomLogicState*>(currentState->customState());

    if(data){
        mParentAction->parentPlayerScene()->startOverlayCompound(data->compound(), mCompound);
    }

}



//---------------------------------------------------------------------------


bool PrintParsingHelper::parseMainPaths(const std::string &_fullPath)
{

    std::vector<std::string>pathParts = StdString::splitString(_fullPath, "/");

    if(pathParts.size()>0){
        textKind = pathParts[0];
    }
    if(pathParts.size()>1){
        originObject = pathParts[1];
    }
    if(pathParts.size()>2){
        originData = pathParts[2];
    }

    if(textKind.empty() || originObject.empty()){
        dbgSystem.addMessage("Error parsing print cfg data from string '" + _fullPath +"'!");
        return false;
    }

    return true;

}


std::string ADbgPrint::doType = "doDbgPrint";



ADbgPrint::ADbgPrint(Action*_parentGameAction, const std::string &_value) : ActionCommand(_parentGameAction)
{
    mValue = _value;
}


ADbgPrint::~ADbgPrint()
{
    if(mOriginObtainer){
        delete mOriginObtainer;
    }

}


bool ADbgPrint::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + doType + "'");


    LogicState * parentState = mParentAction->parentLogicState();
    BaseObject * rootParentObject = parentState->rootParentObject();


    PrintParsingHelper pph;
    pph.rootParentObject = rootParentObject;
    pph.logicState = parentState;

    if(pph.parseMainPaths(mValue)==false){
        return false;
    }


    if(pph.textKind=="ITEM"){
        mKind = Kind::ITEM_DATA;

    }else if(pph.textKind=="LOCAL"){
        mKind = Kind::LOCAL_DATA;

    }else if(pph.textKind=="OBJECT_NAME"){
        mKind = Kind::ORIGIN_OBJECT;
    }


    if(mValue.substr(0,5)=="ITEM/"){
        std::string dataPath = mValue.substr(5);
        app->signalParserManager()->parseItemDataPair(_scene, dataPath, mItemDataPair, ParseItemMode::DATA, rootParentObject, parentState);
        if(mItemDataPair.data==nullptr){
            return false;
        }

    }else if(mValue.substr(0,6)=="LOCAL/"){
        std::string dataPath = mValue.substr(6);
        mItemDataPair.data = parentState->dataStorage().getObject(dataPath);
        if(mItemDataPair.data==nullptr){
            return false;
        }

    }else if(pph.textKind=="OBJECT_NAME"){

        SignalParser *parser = app->signalParserManager()->obtainObjectParserNEW(pph.originObject);
        if(parser==nullptr){
            return false;
        }

        if(parser->parseObjectName(_scene, pph)==false){
            return false;
        }

        mText = pph.text;
        mItemDataPair = pph.itemDataPair;
        mOriginObtainer = pph.originObtainer;

    }


    dbgSystem.removeLastMessage();
    return true;

}




void ADbgPrint::run(UpdateMode &_updateMode)
{

    std::string text;

    if(mOriginObtainer){
        BaseObject *originObject = mOriginObtainer->findOriginObject();



    }else{

        if(mItemDataPair.data){
            text = mItemDataPair.data->valueString();

        }else if(mText.empty()==false){
            text = mText;

        }else{
            text = mValue;
        }

    }

    print(text);

}



//----------------------------------------------------------------------------



std::string ADbgPrintNEW::doType = "doDbgPrintNEW";



ADbgPrintNEW::ADbgPrintNEW(Action*_parentGameAction, const std::string &_value) : ActionCommand(_parentGameAction)
{
    mValue = _value;
}


ADbgPrintNEW::~ADbgPrintNEW()
{
    //if(mOriginObtainer){
    //    delete mOriginObtainer;
    //}

}


bool ADbgPrintNEW::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + doType + "'");


    LogicState * parentState = mParentAction->parentLogicState();
    BaseObject * rootParentObject = parentState->rootParentObject();

    if(mValue=="ITEM/ENTITY:CONTACTED:iSmallCoin/worth"){
        DummyFunction();
    }



    if(mValue.substr(0,5)=="ITEM/"){
        mKind = Kind::DATA;

    }else if(mValue.substr(0,6)=="LOCAL/"){
        mKind = Kind::DATA;

    }


    if(mKind==Kind::DIRECT_TEXT){
        // text is 'mValue'

    }else if(mKind==Kind::DATA){

        if(mDataToText.parse(_scene, mValue, rootParentObject, parentState, ParseItemMode::DATA)==false){
            return false;
        }


    }


    dbgSystem.removeLastMessage();
    return true;

}




void ADbgPrintNEW::run(UpdateMode &_updateMode)
{

    if(mKind==Kind::DIRECT_TEXT){
        print(mValue);

    }else if(mKind==Kind::DATA){

        if(mDataToText.accessorNotUsed()){
            return;
        }

        std::string text = mDataToText.text();
        print(text);
    }

}





//---------------------------------------------------------------------------


//std::string AQuitGame::type = "quitGame";
std::string AQuitGame::doType = "doQuitGame";

/*
AQuitGame::AQuitGame(Action*_parentGameAction, const pugi::xml_node &_node)
{

    mParentGameAction = _parentGameAction;

}
*/

AQuitGame::AQuitGame(Action*_parentGameAction, const std::string &_value) : ActionCommand(_parentGameAction)
{
    mValue = _value;
}



bool AQuitGame::initConnections(PlayedScene *_scene)
{
    return true;
}



void AQuitGame::run(UpdateMode &_updateMode)
{
    ncine::theApplication().quit();
}




//---------------------------------------------------------------------------------------



//std::string SetTableItemsGroup::type = "setTableItemsGroup";
std::string SetTableItemsGroup::doType = "doSetTableItemsGroup";

/*
SetTableItemsGroup::SetTableItemsGroup(Action*_parentGameAction, const pugi::xml_node &node)
{

    mParentGameAction = _parentGameAction;
    mTableName = node.attribute("table").as_string("");
    mItemsGroupName = node.attribute("itemsGroup").as_string("");
    //mSystemItemsGroupName = node.attribute("systemItemsGroup").as_string("");

}
*/

SetTableItemsGroup::SetTableItemsGroup(Action*_parentAction, const std::string &_value) : ActionCommand((_parentAction))
{
    mValue = _value;
}



bool SetTableItemsGroup::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + doType + "'");


    if(mValue.empty()==false){

        std::vector<std::string>parts = StdString::splitString(mValue, ",");

        if(parts.size()!=2){
            dbgSystem.addMessage("Wrong path '" + mValue + "' !");
        }

        const std::string &table = parts[0];
        const std::string &itemsGroup = parts[1];

        mTable = dynamic_cast<GuiTable*>(ObtainGuiWidget(_scene, table, WidgetType::TABLE));
        if(mTable==nullptr){
            return false;
        }

        mItemsGroup = ObtainItemsGroup(_scene, itemsGroup);
        if(mItemsGroup==nullptr){
            return false;
        }


    }else{

        mTable = dynamic_cast<GuiTable*>(ObtainGuiWidget(_scene, mTableName, WidgetType::TABLE));
        if(mTable==nullptr){
            return false;
        }

        //---
        //if(mSystemItemsGroupName=="usersNames"){
        //    mItemsGroup = app->usersDatabase()->usersNamesItems();

        //}else{
            mItemsGroup = ObtainItemsGroup(_scene, mItemsGroupName);
        //}

        if(mItemsGroup==nullptr){
            return false;
        }


    }



    dbgSystem.removeLastMessage();
    return true;

}



void SetTableItemsGroup::run(UpdateMode &_updateMode)
{

    mTable->setItemsGroup(mItemsGroup);

}




//---------------------------------------------------------------------------------------



std::string SetTableNewItemsGroup::doType = "doSetTableNewItemsGroup";



SetTableNewItemsGroup::SetTableNewItemsGroup(Action*_parentAction, const std::string &_value) : ActionCommand((_parentAction))
{
    mValue = _value;
}



bool SetTableNewItemsGroup::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + doType + "'");

    LogicState * parentState = mParentAction->parentLogicState();
    BaseObject * rootParentObject = parentState->rootParentObject();


    std::vector<std::string>parts = StdString::splitString(mValue, ",");

    if(parts.size()!=2){
        dbgSystem.addMessage("Wrong path '" + mValue + "' !");
    }

    const std::string &table = parts[0];
    const std::string &itemPath = parts[1];

    mTable = dynamic_cast<GuiTableNew*>(ObtainGuiWidget(_scene, table, WidgetType::TABLE));
    if(mTable==nullptr){
        return false;
    }


    //---
    app->signalParserManager()->parseItemDataPair(_scene, itemPath, mItemDataPair, ParseItemMode::ITEM, rootParentObject, parentState);
    if(mItemDataPair.item==nullptr){
        return false;
    }


    dbgSystem.removeLastMessage();
    return true;

}



void SetTableNewItemsGroup::run(UpdateMode &_updateMode)
{

    mTable->setItem(mItemDataPair.item);

}

//---------------------------------------------------------------------------------------



std::string AssignItemToWidget::doType = "doAssignItemToWidget";


AssignItemToWidget::AssignItemToWidget(Action*_parentAction, const std::string &_value) : ActionCommand((_parentAction))
{
    mValue = _value;
}


bool AssignItemToWidget::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("init '" + doType + "'");

    LogicState * parentState = mParentAction->parentLogicState();
    BaseObject * rootParentObject = parentState->rootParentObject();


    std::vector<std::string>parts = StdString::splitString(mValue, "=");

    if(parts.size()!=2){
        dbgSystem.addMessage("Wrong path '" + mValue + "' !");
    }

    const std::string &widget = parts[0];
    const std::string &itemPath = parts[1];


    mWidget = ObtainGuiWidget(_scene, widget, WidgetType::NOT_DEFINED);
    if(mWidget==nullptr){
        return false;
    }


    //---
    app->signalParserManager()->parseItemDataPair(_scene, itemPath, mItemDataPair, ParseItemMode::ITEM, rootParentObject, parentState);
    if(mItemDataPair.item==nullptr){
        return false;
    }


    dbgSystem.removeLastMessage();
    return true;

}



void AssignItemToWidget::run(UpdateMode &_updateMode)
{

    mWidget->setItem(mItemDataPair.item);

}








}
