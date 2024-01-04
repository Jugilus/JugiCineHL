#include <limits>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmSprite.h"
#include "jmCompositeSprite.h"
#include "jmStreams.h"

#include "logic/jpLogicState.h"
#include "jpPlayedScene.h"
#include "jpPlayedApp.h"

#include "jpObjectParser.h"
#include "jpItem.h"
#include "jpDataUtilities.h"


namespace jugimap{




ItemDataPair::~ItemDataPair()
{
    if(originObjectObtainer){
        delete originObjectObtainer;
    }
}



bool ItemDataPair::updateItemAndData()
{

    if(originObjectObtainer){
        originObject = originObjectObtainer->findOriginObject();
    }

    if(originObject==nullptr){
        return false;
    }

    item = originObject->item()->findFirstChildItem(sourceItem);
    if(item==nullptr){
        return false;
    }

    if(dataIndex!=-1){
        data = item->dataStorage().at(dataIndex);
    }


    return true;
}



ItemDataAccessor::ItemDataAccessor(DataAccessorType _type) : mType(_type)
{

}



/*
bool DataAccessor::parseDataValue(const std::string & _signalValue, DataParsingHelper &_dpi, const std::string & _signalPath)
{




}
*/


bool ItemDataAccessor::parseLeftSide(PlayedScene *_scene, const std::string &_path, ItemDataParsingHelper &dpi)
{


    if(_path.substr(0,6)=="LOCAL/"){

        std::string leftSideDataName = _path.substr(6);
        mLeftSideItemDataPair.data = dpi.logicState->dataStorage().getObject(leftSideDataName);
        if(mLeftSideItemDataPair.data==nullptr){
            return false;
        }
        mLeftSideItemDataPair.dataType = mLeftSideItemDataPair.data->type();


    }else if(_path.substr(0,5)=="ITEM/"){

        std::string leftSideItemDataPath = _path.substr(5);
        dpi.accesorData = &mLeftSideItemDataPair;

        if(app->signalParserManager()->parseItemDataPairNEW(_scene, dpi, leftSideItemDataPath)==false){
            return false;
        }
        mAccessorNotUsed = dpi.accessorNotUsed;


    }else{
        dbgSystem.addMessage("Error parsing left side string '" + _path + "'!");
        return false;

    }

    return true;

}



bool ItemDataAccessor::parseRightSide(PlayedScene *_scene, const std::string &_path, ItemDataParsingHelper &dpi)
{


    if(_path.substr(0,6)=="LOCAL/"){
        mRightSideContent = RightSideContent::DATA;

        std::string dataName = _path.substr(6);
        mRightSideItemDataPair.data = dpi.logicState->dataStorage().getObject(dataName);
        if(mRightSideItemDataPair.data==nullptr){
            return false;
        }
         mRightSideItemDataPair.dataType = mRightSideItemDataPair.data->type();


    }else if(_path.substr(0,5)=="ITEM/"){
        mRightSideContent = RightSideContent::DATA;

        std::string itemDataPath = _path.substr(5);

        dpi.accesorData = &mRightSideItemDataPair;

        if(app->signalParserManager()->parseItemDataPairNEW(_scene, dpi, itemDataPath)==false){
            return false;
        }
        mAccessorNotUsed = dpi.accessorNotUsed;



    }else if(_path=="MIN_VALUE"){
        mRightSideContent = RightSideContent::MIN_VALUE_OF_LEFT_DATA;

        if(!(mLeftSideItemDataPair.dataType==ItemDataType::INT || mLeftSideItemDataPair.dataType==ItemDataType::FLOAT)){
            dbgSystem.addMessage("MIN_VALUE expression is avilable only for integer and float data!");
            return false;
        }

        //if(mLeftSideItemDataPair.data->type()==ItemDataType::INT){
        //    mDataQuery.mIntValue = static_cast<IntItemData*>(mDataQuery.mLeftSideItemDataPair.data)->minValue();

        //}else if(mDataQuery.mLeftSideItemDataPair.data->type()==ItemDataType::FLOAT){
        //    mDataQuery.mFloatValue = static_cast<FloatItemData*>(mDataQuery.mLeftSideItemDataPair.data)->minValue();
        //}

    }else if(_path=="MAX_VALUE"){
        mRightSideContent = RightSideContent::MAX_VALUE_OF_LEFT_DATA;

        if(!(mLeftSideItemDataPair.dataType==ItemDataType::INT || mLeftSideItemDataPair.dataType==ItemDataType::FLOAT)){
            dbgSystem.addMessage("MAX_VALUE expression is avilable only for integer and float data!");
            return false;
        }

        //if(mDataQuery.mLeftSideItemDataPair.data->type()==ItemDataType::INT){
        //    mDataQuery.mIntValue = static_cast<IntItemData*>(mDataQuery.mLeftSideItemDataPair.data)->maxValue();

        //}else if(mDataQuery.mLeftSideItemDataPair.data->type()==ItemDataType::FLOAT){
        //    mDataQuery.mFloatValue = static_cast<FloatItemData*>(mDataQuery.mLeftSideItemDataPair.data)->maxValue();
        //}


    }else{

        mRightSideContent = RightSideContent::DIRECT_VALUE;

        if(mLeftSideItemDataPair.dataType==ItemDataType::INT){
            if(StdString::integerNumber(_path, mDirectValues.mIntValue)==false){
                return false;
            }

        }else if(mLeftSideItemDataPair.dataType==ItemDataType::FLOAT){
            if(StdString::floatNumber(_path, mDirectValues.mFloatValue)==false){
                return false;
            }

        }else if(mLeftSideItemDataPair.dataType==ItemDataType::BOOL){
            if(StdString::boolValue(_path, mDirectValues.mBoolValue)==false){
                return false;
            }
        }

    }


    return true;
}




//=================================================================================

DataQuery::DataQuery() : ItemDataAccessor(DataAccessorType::DATA_QUERY)
{

}


bool DataQuery::parse(PlayedScene *_scene, const std::string & _fullPath, BaseObject *_rootParentObject, LogicState *_logicState, ParseItemMode _parseItemMode)
{

    dbgSystem.addMessage("Parsing 'DataQuery' from string '" + _fullPath +"' ..." );


    std::vector<std::string>parts = StdString::splitString(_fullPath, "=");
    if(parts.size()!=2){
        dbgSystem.addMessage("Error parsing string '" + _fullPath + "'!");
        return false;

    }
    const std::string & leftSide = parts[0];
    const std::string & rightSide = parts[1];


    ItemDataParsingHelper dpi;
    dpi.rootParentObject = _rootParentObject;
    dpi.logicState = _logicState;
    dpi.dataAccessorType = mType;
    dpi.parseItemMode = _parseItemMode;


    //---- LEFT SIDE
    if(parseLeftSide(_scene, leftSide, dpi)==false){
        return false;
    }
    if(mAccessorNotUsed){
        dbgSystem.removeLastMessage();
        return true;
    }


    //---- RIGHT SIDE
    if(parseRightSide(_scene, rightSide, dpi)==false){
        return false;
    }
    if(mAccessorNotUsed){
        dbgSystem.removeLastMessage();
        return true;
    }

    /*
    if(rightSide.substr(0,6)=="LOCAL/"){
        mRightSideContent = RightSideContent::DATA;

        std::string rightSidePath = rightSide.substr(6);
        mRightSideItemDataPair.data = _logicState->dataStorage().getObject(rightSidePath);
        if(mRightSideItemDataPair.data==nullptr){
            return false;
        }
         mRightSideItemDataPair.dataType = mRightSideItemDataPair.data->type();


    }else if(rightSide.substr(0,5)=="ITEM:"){
        mRightSideContent = RightSideContent::DATA;

        std::string rightSidePath = rightSide.substr(5);

        dpi.accesorData = &mRightSideItemDataPair;

        if(app->signalParserManager()->parseItemDataPairNEW(_scene, dpi, rightSidePath)==false){
            return false;
        }



    }else if(rightSide=="MIN_VALUE"){
        mRightSideContent = RightSideContent::MIN_VALUE_OF_LEFT_DATA;

        if(!(mLeftSideItemDataPair.dataType==ItemDataType::INT || mLeftSideItemDataPair.dataType==ItemDataType::FLOAT)){
            dbgSystem.addMessage("MIN_VALUE expression is avilable only for integer and float data!");
            return false;
        }

        //if(mLeftSideItemDataPair.data->type()==ItemDataType::INT){
        //    mDataQuery.mIntValue = static_cast<IntItemData*>(mDataQuery.mLeftSideItemDataPair.data)->minValue();

        //}else if(mDataQuery.mLeftSideItemDataPair.data->type()==ItemDataType::FLOAT){
        //    mDataQuery.mFloatValue = static_cast<FloatItemData*>(mDataQuery.mLeftSideItemDataPair.data)->minValue();
        //}

    }else if(rightSide=="MAX_VALUE"){
        mRightSideContent = RightSideContent::MAX_VALUE_OF_LEFT_DATA;

        if(!(mLeftSideItemDataPair.dataType==ItemDataType::INT || mLeftSideItemDataPair.dataType==ItemDataType::FLOAT)){
            dbgSystem.addMessage("MAX_VALUE expression is avilable only for integer and float data!");
            return false;
        }

        //if(mDataQuery.mLeftSideItemDataPair.data->type()==ItemDataType::INT){
        //    mDataQuery.mIntValue = static_cast<IntItemData*>(mDataQuery.mLeftSideItemDataPair.data)->maxValue();

        //}else if(mDataQuery.mLeftSideItemDataPair.data->type()==ItemDataType::FLOAT){
        //    mDataQuery.mFloatValue = static_cast<FloatItemData*>(mDataQuery.mLeftSideItemDataPair.data)->maxValue();
        //}


    }else{      // numeric

        mRightSideContent = RightSideContent::DIRECT_VALUE;

        if(mLeftSideItemDataPair.dataType==ItemDataType::INT){
            if(StdString::integerNumber(rightSide, mDirectValues.mIntValue)==false){
                return false;
            }

        }else if(mLeftSideItemDataPair.dataType==ItemDataType::FLOAT){
            if(StdString::floatNumber(rightSide, mDirectValues.mFloatValue)==false){
                return false;
            }

        }else if(mLeftSideItemDataPair.dataType==ItemDataType::BOOL){
            if(StdString::boolValue(rightSide, mDirectValues.mBoolValue)==false){
                return false;
            }
        }

    }
    */

    if(mRightSideContent==RightSideContent::DATA){
        if(mLeftSideItemDataPair.dataType != mRightSideItemDataPair.dataType){
            dbgSystem.addMessage("Data on the right side is of different type than the data on the left side!");
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}



bool DataQuery::isTrue()
{

    if(mLeftSideItemDataPair.originObject || mLeftSideItemDataPair.originObjectObtainer){
        if(mLeftSideItemDataPair.updateItemAndData()==false){
            return false;
        }
    }

    if(mRightSideContent==RightSideContent::DATA){
        if(mRightSideItemDataPair.originObject || mRightSideItemDataPair.originObjectObtainer){
            if(mRightSideItemDataPair.updateItemAndData()==false){
                return false;
            }
        }
    }


    if(mLeftSideItemDataPair.data->type()==ItemDataType::BOOL){

        BoolItemData *leftData = static_cast<BoolItemData*>(mLeftSideItemDataPair.data);

        bool rightValue = mDirectValues.mBoolValue;
        if(mRightSideItemDataPair.data){
            BoolItemData *rightData = static_cast<BoolItemData*>(mRightSideItemDataPair.data);
            rightValue = rightData->value();
        }

        return leftData->isValue(rightValue);


    }else if(mLeftSideItemDataPair.data->type()==ItemDataType::INT){

        IntItemData *leftData = static_cast<IntItemData*>(mLeftSideItemDataPair.data);


        int rightValue =  mDirectValues.mIntValue;

        if(mRightSideContent==RightSideContent::DATA ){
            IntItemData *rightData = static_cast<IntItemData*>(mRightSideItemDataPair.data);
            rightValue = rightData->value();

        }else if(mRightSideContent==RightSideContent::MIN_VALUE_OF_LEFT_DATA){
            rightValue = leftData->minValue();

        }else if(mRightSideContent==RightSideContent::MAX_VALUE_OF_LEFT_DATA){
            rightValue = leftData->maxValue();
        }

        return leftData->isValue(rightValue);


    }else if(mLeftSideItemDataPair.data->type()==ItemDataType::FLOAT){

        FloatItemData *leftData = static_cast<FloatItemData*>(mLeftSideItemDataPair.data);

        float rightValue = mDirectValues.mFloatValue;

        if(mRightSideContent==RightSideContent::DATA ){
            FloatItemData *rightData = static_cast<FloatItemData*>(mRightSideItemDataPair.data);
            rightValue = rightData->value();

        }else if(mRightSideContent==RightSideContent::MIN_VALUE_OF_LEFT_DATA){
            rightValue = leftData->minValue();

        }else if(mRightSideContent==RightSideContent::MAX_VALUE_OF_LEFT_DATA){
            rightValue = leftData->maxValue();
        }

        return leftData->isValue(rightValue);

    }


    return false;

}


//=================================================================================


DataSetter::DataSetter() : ItemDataAccessor(DataAccessorType::DATA_SETTER)
{

}


bool DataSetter::parse(PlayedScene *_scene, const std::string & _fullPath, BaseObject *_rootParentObject, LogicState *_logicState, ParseItemMode _parseItemMode)
{

    dbgSystem.addMessage("Parsing 'DataSetter' from string '" + _fullPath +"' ..." );


    if(_fullPath=="LOCAL/energyReduction = ADD/ITEM/ENTITY:THIS/skills:meleeCombat:attackDemage"){
        DummyFunction();
    }

    std::vector<std::string>parts = StdString::splitString(_fullPath, "=");
    if(parts.size()!=2){
        dbgSystem.addMessage("Error parsing string '" + _fullPath + "'!");
        return false;

    }
    const std::string & leftSide = parts[0];
    const std::string & fullRightSide = parts[1];


    ItemDataParsingHelper dpi;
    dpi.rootParentObject = _rootParentObject;
    dpi.logicState = _logicState;
    dpi.dataAccessorType = mType;
    dpi.parseItemMode = _parseItemMode;


    //---- LEFT SIDE
    if(parseLeftSide(_scene, leftSide, dpi)==false){
        return false;
    }
    if(mAccessorNotUsed){
        dbgSystem.removeLastMessage();
        return true;
    }


    //---- RIGHT SIDE
    std::string rightSide;
    if(fullRightSide.substr(0,4)=="ADD/"){
        mArithemticOperation = ArithemticOperator::ADD;
        rightSide = fullRightSide.substr(4);

    }else if(fullRightSide.substr(0,9)=="SUBTRACT/"){
        mArithemticOperation = ArithemticOperator::SUBTRACT;
        rightSide = fullRightSide.substr(9);

    }else if(fullRightSide.substr(0,9)=="MULTIPLY/"){
        mArithemticOperation = ArithemticOperator::MULTIPLY;
        rightSide = fullRightSide.substr(9);

    }else{
        mArithemticOperation = ArithemticOperator::NONE;
        rightSide = fullRightSide;
    }

    if(mArithemticOperation != ArithemticOperator::NONE){
        if( mLeftSideItemDataPair.dataType != ItemDataType::INT && mLeftSideItemDataPair.dataType != ItemDataType::FLOAT){
            dbgSystem.addMessage("Error! Arithemtic operations specified for a non-arithmetic data!");
            return false;
        }
    }


    if(parseRightSide(_scene, rightSide, dpi)==false){
        return false;
    }
    if(mAccessorNotUsed){
        dbgSystem.removeLastMessage();
        return true;
    }

    //----
    if(mRightSideContent==RightSideContent::DATA){
        if(mLeftSideItemDataPair.dataType != mRightSideItemDataPair.dataType){
            dbgSystem.addMessage("Data on the right side is of different type than the data on the left side!");
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}



void DataSetter::set()
{

    if(mLeftSideItemDataPair.originObject || mLeftSideItemDataPair.originObjectObtainer){
        if(mLeftSideItemDataPair.updateItemAndData()==false){
            return;
        }
    }

    if(mRightSideContent==RightSideContent::DATA){
        if(mRightSideItemDataPair.originObject || mRightSideItemDataPair.originObjectObtainer){
            if(mRightSideItemDataPair.updateItemAndData()==false){
                return;
            }
        }
    }


    if(mLeftSideItemDataPair.data->type()==ItemDataType::BOOL){

        BoolItemData *leftData = static_cast<BoolItemData*>(mLeftSideItemDataPair.data);

        bool rightValue = mDirectValues.mBoolValue;
        if(mRightSideItemDataPair.data){
            BoolItemData *rightData = static_cast<BoolItemData*>(mRightSideItemDataPair.data);
            rightValue = rightData->value();
        }

        leftData->setValue(rightValue);


    }else if(mLeftSideItemDataPair.data->type()==ItemDataType::INT){

        IntItemData *leftData = static_cast<IntItemData*>(mLeftSideItemDataPair.data);

        int rightValue = mDirectValues.mIntValue;

        if(mRightSideContent==RightSideContent::DATA ){
            IntItemData *rightData = static_cast<IntItemData*>(mRightSideItemDataPair.data);
            rightValue = rightData->value();

        }else if(mRightSideContent==RightSideContent::MIN_VALUE_OF_LEFT_DATA){
            rightValue = leftData->minValue();

        }else if(mRightSideContent==RightSideContent::MAX_VALUE_OF_LEFT_DATA){
            rightValue = leftData->maxValue();
        }

        //----
        if(mArithemticOperation==ArithemticOperator::NONE){
            leftData->setValue(rightValue);

        }else if(mArithemticOperation==ArithemticOperator::ADD){
            leftData->setValue(leftData->value() + rightValue);

        }else if(mArithemticOperation==ArithemticOperator::SUBTRACT){
            leftData->setValue(leftData->value() - rightValue);

        }else if(mArithemticOperation==ArithemticOperator::MULTIPLY){
            leftData->setValue(leftData->value() * rightValue);
        }


    }else if(mLeftSideItemDataPair.data->type()==ItemDataType::FLOAT){

        FloatItemData *leftData = static_cast<FloatItemData*>(mLeftSideItemDataPair.data);

        float rightValue = mDirectValues.mFloatValue;

        if(mRightSideContent==RightSideContent::DATA){
            FloatItemData *rightData = static_cast<FloatItemData*>(mRightSideItemDataPair.data);
            rightValue = rightData->value();

        }else if(mRightSideContent==RightSideContent::MIN_VALUE_OF_LEFT_DATA){
            rightValue = leftData->minValue();

        }else if(mRightSideContent==RightSideContent::MAX_VALUE_OF_LEFT_DATA){
            rightValue = leftData->maxValue();
        }

        //----
        if(mArithemticOperation==ArithemticOperator::NONE){
            leftData->setValue(rightValue);

        }else if(mArithemticOperation==ArithemticOperator::ADD){
            leftData->setValue(leftData->value() + rightValue);

        }else if(mArithemticOperation==ArithemticOperator::SUBTRACT){
            leftData->setValue(leftData->value() - rightValue);

        }else if(mArithemticOperation==ArithemticOperator::MULTIPLY){
            leftData->setValue(leftData->value() * rightValue);
        }

    }

}




//=================================================================================


ItemQuery::ItemQuery() : ItemDataAccessor(DataAccessorType::ITEM_QUERY)
{

}


bool ItemQuery::parse(PlayedScene *_scene, const std::string & _fullPath, BaseObject *_rootParentObject, LogicState *_logicState, ParseItemMode _parseItemMode)
{

    dbgSystem.addMessage("Parsing 'ItemQuery' from string '" + _fullPath +"' ..." );

    if(_fullPath=="ENTITY:THIS/itemsInventory SELECTED gamepad"){
        DummyFunction();
    }


    std::string leftSide;
    std::string rightSide;
    std::string delimiter;

    if(_fullPath.find(" STORED ") != std::string::npos){
        delimiter = " STORED ";
        mOperation = Operation::CONTAINS_STORED_CHILD_ITEM;

    }else if(_fullPath.find(" SELECTED ") != std::string::npos){
        delimiter = " SELECTED ";
        mOperation = Operation::CONTAINS_SELECTED_CHILD_ITEM;

    }else{

        dbgSystem.addMessage("Error parsing string '" + _fullPath + "'!");
        return false;
    }

        std::vector<std::string>parts = StdString::splitString(_fullPath, delimiter);
    if(parts.size()!=2){
        dbgSystem.addMessage("Error parsing string '" + _fullPath + "'!");
        return false;

    }
    leftSide = parts[0];
    rightSide = parts[1];


    ItemDataParsingHelper dpi;
    dpi.rootParentObject = _rootParentObject;
    dpi.logicState = _logicState;
    dpi.dataAccessorType = mType;
    dpi.parseItemMode = _parseItemMode;


    //---- LEFT SIDE
    dpi.accesorData = &mLeftSideItemDataPair;

    if(app->signalParserManager()->parseItemDataPairNEW(_scene, dpi, leftSide)==false){
        return false;
    }
    mAccessorNotUsed = dpi.accessorNotUsed;
    if(mAccessorNotUsed){
        dbgSystem.removeLastMessage();
        return true;
    }

    //---- RIGHT SIDE
    if(rightSide.substr(0, 9)=="ITEM_TAG:"){
        SourceItemsStorage *storage = _scene->sourceItemsStorage();

        std::string sTagNames = rightSide.substr(9);
        std::vector<std::string>tagNames = StdString::splitString(sTagNames, ",");
        for(const std::string &s : tagNames){
            ItemTag *itemTag = storage->itemsTagsStorage().getObject(s);
            if(itemTag==nullptr){
                return false;
            }
            mRightSideItemDataPair.itemTagFlags |= itemTag->tagFlag();
        }

        if(mRightSideItemDataPair.itemTagFlags==0){
            dbgSystem.addMessage("Error parsing item tags from string '" + sTagNames + "'!");
            return false;
        }


    }else{
        mRightSideItemDataPair.sourceItem = _scene->sourceItemsStorage()->sourceItems().getObject(rightSide);
        if(mRightSideItemDataPair.sourceItem==nullptr){
            return false;
        }
    }


    /*
    //---- RIGHT SIDE
    if(parseRightSide(_scene, rightSide, dpi)==false){
        return false;
    }
    if(mAccessorNotUsed){
        dbgSystem.removeLastMessage();
        return true;
    }

    if(mRightSideContent==RightSideContent::DATA){
        if(mLeftSideItemDataPair.dataType != mRightSideItemDataPair.dataType){
            dbgSystem.addMessage("Data on the right side is of different type than the data on the left side!");
            return false;
        }
    }
    */

    dbgSystem.removeLastMessage();
    return true;

}



bool ItemQuery::isTrue()
{

    if(mLeftSideItemDataPair.originObject || mLeftSideItemDataPair.originObjectObtainer){
        if(mLeftSideItemDataPair.updateItemAndData()==false){
            return false;
        }
    }


    if(mOperation==Operation::CONTAINS_STORED_CHILD_ITEM){

        if(mRightSideItemDataPair.sourceItem){
            Item *childItem = mLeftSideItemDataPair.item->findChildItem(mRightSideItemDataPair.sourceItem);
            if(childItem){
                return true;
            }

        }else if(mRightSideItemDataPair.itemTagFlags != 0){
            std::vector<Item*>items;
            mLeftSideItemDataPair.item->collectChildItemsWithTagFlag(items, mRightSideItemDataPair.itemTagFlags);
            if(items.empty()==false){
                return true;
            }

        }


    }else if(mOperation==Operation::CONTAINS_SELECTED_CHILD_ITEM){

        if(mRightSideItemDataPair.sourceItem){
            //Item *childItem = mLeftSideItemDataPair.item->findChildItem(mRightSideItemDataPair.sourceItem);

            //if(childItem && childItem->isSelected()){
            //    return true;
            //}
            Item *selectedChildItem = mLeftSideItemDataPair.item->findSelectedChildItem();
            if(selectedChildItem==nullptr){
                return false;
            }
            if(selectedChildItem->sourceItem() == mRightSideItemDataPair.sourceItem ||
               selectedChildItem->sourceItem() == mRightSideItemDataPair.sourceItem->proxy())
            {
                return true;
            }

        }else if(mRightSideItemDataPair.itemTagFlags != 0){
            Item *childItem = mLeftSideItemDataPair.item->findSelectedChildItem();
            if(childItem && childItem->sourceItem()->tagFlags() & mRightSideItemDataPair.itemTagFlags){
                return true;
            }
        }
    }


    return false;
}


//=================================================================================


ItemSetter::ItemSetter() : ItemDataAccessor(DataAccessorType::ITEM_MOVER)
{

}


bool ItemSetter::parse(PlayedScene *_scene, const std::string & _fullPath, BaseObject *_rootParentObject, LogicState *_logicState, ParseItemMode _parseItemMode)
{

    dbgSystem.addMessage("Parsing 'ItemMover' from string '" + _fullPath +"' ..." );


    if(_fullPath=="ENTITY:THIS/itemsInventory ADD ENTITY:CONTACTED:iGamepad"){
        DummyFunction();
    }


    std::string leftSide;
    std::string rightSide;
    std::string delimiter;

    if(_fullPath.find(" ADD ") != std::string::npos){
        delimiter = " ADD ";
        mOperation = Operation::ADD_CHILD_ITEM;

    }else if(_fullPath.find(" REMOVE ") != std::string::npos){
        delimiter = " REMOVE ";
        mOperation = Operation::REMOVE_CHILD_ITEM;

    }else{

        dbgSystem.addMessage("Error parsing string '" + _fullPath + "'!");
        return false;
    }

    std::vector<std::string>parts = StdString::splitString(_fullPath, delimiter);
    if(parts.size()!=2){
        dbgSystem.addMessage("Error parsing string '" + _fullPath + "'!");
        return false;

    }
    leftSide = parts[0];
    rightSide = parts[1];


    ItemDataParsingHelper dpi;
    dpi.rootParentObject = _rootParentObject;
    dpi.logicState = _logicState;
    dpi.dataAccessorType = mType;
    dpi.parseItemMode = _parseItemMode;


    //--- LEFT SIDE
    dpi.accesorData = &mLeftSideItemDataPair;
    if(app->signalParserManager()->parseItemDataPairNEW(_scene, dpi, leftSide)==false){
        return false;
    }
    mAccessorNotUsed = dpi.accessorNotUsed;
    if(mAccessorNotUsed){
        dbgSystem.removeLastMessage();
        return true;
    }


    //--- RIGHT SIDE
    if(mOperation==Operation::ADD_CHILD_ITEM){

        // parse EXISTING ITEM
        mRightSideContent = RightSideContent::ITEM;
        dpi.accesorData = &mRightSideItemDataPair;
        if(app->signalParserManager()->parseItemDataPairNEW(_scene, dpi, rightSide)==false){
            return false;
        }
        mAccessorNotUsed = dpi.accessorNotUsed;
        if(mAccessorNotUsed){
            dbgSystem.removeLastMessage();
            return true;
        }

    }else if(mOperation==Operation::REMOVE_CHILD_ITEM){

        // parse SOURCE ITEM -> a possible child item with that source item will be removed
        mRightSideItemDataPair.sourceItem = _scene->sourceItemsStorage()->sourceItems().getObject(rightSide);
        if(mRightSideItemDataPair.sourceItem==nullptr){
            return false;
        }
    }


    //--- check out if right side item is allowed in left side item
    if(StdVector::contains(mLeftSideItemDataPair.sourceItem->allowedChildSourceItems(), mRightSideItemDataPair.sourceItem)==false){
        dbgSystem.addMessage("Item '" + mRightSideItemDataPair.sourceItem->name() + "' is not allowed in '" + mLeftSideItemDataPair.sourceItem->name() + "' !");
        return false;
    }


    dbgSystem.removeLastMessage();
    return true;

}



void ItemSetter::set()
{

    if(mLeftSideItemDataPair.originObject || mLeftSideItemDataPair.originObjectObtainer){
        if(mLeftSideItemDataPair.updateItemAndData()==false){
            return;
        }
    }

    if(mRightSideContent==RightSideContent::ITEM){
        if(mRightSideItemDataPair.originObject || mRightSideItemDataPair.originObjectObtainer){
            if(mRightSideItemDataPair.updateItemAndData()==false){
                return;
            }
        }
    }

    if(mOperation==Operation::ADD_CHILD_ITEM){
        mLeftSideItemDataPair.item->addChildItem(mRightSideItemDataPair.item, AddChildItemMode::COPY);

    }else if(mOperation==Operation::REMOVE_CHILD_ITEM){
        mLeftSideItemDataPair.item->removeChildItem(mRightSideItemDataPair.sourceItem);
    }



}



//====================================================================================



DataToText::DataToText() : ItemDataAccessor(DataAccessorType::DATA_TO_TEXT)
{

}



bool DataToText::parse(PlayedScene *_scene, const std::string & _fullPath, BaseObject *_rootParentObject, LogicState *_logicState, ParseItemMode _parseItemMode)
{

    dbgSystem.addMessage("Parsing 'DataToText' from string '" + _fullPath +"' ..." );


    ItemDataParsingHelper dpi;
    dpi.rootParentObject = _rootParentObject;
    dpi.logicState = _logicState;
    dpi.dataAccessorType = mType;
    dpi.parseItemMode = _parseItemMode;


    if(parseLeftSide(_scene, _fullPath, dpi)==false){
        return false;
    }
    if(mAccessorNotUsed){
        dbgSystem.removeLastMessage();
        return true;
    }


    dbgSystem.removeLastMessage();
    return true;

}



std::string DataToText::text(bool includeDataName)
{

    if(mLeftSideItemDataPair.originObject || mLeftSideItemDataPair.originObjectObtainer){
        if(mLeftSideItemDataPair.updateItemAndData()==false){
            return "Data text could not be found";
        }
    }

    assert(mLeftSideItemDataPair.data);

    ItemData * data = mLeftSideItemDataPair.data;


    std::string text;
    if(includeDataName){
        text = data->name() + "=";
    }
    text += data->valueString();

    return text;

}



//====================================================================================


bool ItemDataParsingHelper::parseMainPaths(const std::string &_fullSignalPath)
{

    std::vector<std::string>pathParts = StdString::splitString(_fullSignalPath, "/");

    originObjectPath.clear();
    itemAndDataPath.clear();

    if(pathParts.size()>0){
        originObjectPath = pathParts[0];
    }
    if(pathParts.size()>1){
        itemAndDataPath = pathParts[1];
    }
    //if(pathParts.size()>2){
    //    signalStyle = pathParts[2];
    //}

    if(originObjectPath==""){
        dbgSystem.addMessage("Parse data from string '" + _fullSignalPath +"' error!");
        return false;
    }

    return true;

}


//===================================================================================

/*
ItemIdentifier *ItemIdentifiers::add(const std::string &_itemPath)
{

    mIdentifiers.emplace_back();
    mIdentifiers.back().itemPath = _itemPath;
    return &mIdentifiers.back();
}


bool ItemIdentifiers::getParsingHelper(ItemDataParsingHelper &_iph, const std::string &_itemPath, bool _setErrorMessage)
{

    for(unsigned int i=0; i<mIdentifiers.size(); i++){
        ItemIdentifier &si = mIdentifiers[i];

        if(si.itemPath == _itemPath ){
            _iph.itemIndex = i;

            _iph.parsingOK = true;
            return true;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("Item parsing helper for path '" + _itemPath + "' error!");
    }

    return false;
}

*/

//===================================================================================


ItemDataIdentifier *ItemDataIdentifiers::add(SourceItem *_sourceItem, const std::string &_itemPath, std::vector<ItemData *> &_data)
{

    mIdentifiers.emplace_back();
    mIdentifiers.back().sourceItem = _sourceItem;
    mIdentifiers.back().itemPath = _itemPath;

    for(ItemData* id : _data){
        mIdentifiers.back().dataNamesAndTypes.push_back({id->name(), id->type()});
    }

    return &mIdentifiers.back();

}


bool ItemDataIdentifiers::getParsingHelper(ItemDataParsingHelper &_iph, const std::string &_itemDataPath, bool _setErrorMessage)
{

    std::string itemPath;
    std::string dataName;

    if(_iph.parseItemMode==ParseItemMode::DATA){

        size_t index = _itemDataPath.find_last_of(":");
        if(index != std::string::npos){
            itemPath = _itemDataPath.substr(0, index);
            dataName = _itemDataPath.substr(index+1);

        }else{
            dataName = _itemDataPath;
        }

    }else if(_iph.parseItemMode==ParseItemMode::ITEM){

        itemPath = _itemDataPath;
    }


    for(unsigned int i=0; i<mIdentifiers.size(); i++){
        ItemDataIdentifier &si = mIdentifiers[i];

        if(si.itemPath == itemPath ){
            _iph.accesorData->sourceItem = si.sourceItem;
            //_iph.itemDataIdentifier = &si;


            if(_iph.parseItemMode==ParseItemMode::DATA){
                for(unsigned int d=0; d<si.dataNamesAndTypes.size(); d++){
                    if(si.dataNamesAndTypes[d].first == dataName){
                        _iph.accesorData->dataIndex = d;
                        _iph.accesorData->dataType = si.dataNamesAndTypes[d].second;
                        break;
                    }
                }

                if(_iph.accesorData->dataIndex==-1){
                    dbgSystem.addMessage("Data '" + dataName + "' not found in item '" + itemPath + "' !");
                    return false;
                }
            }

            _iph.parsingOK = true;
            return true;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("Item parsing helper for path '" + _itemDataPath + "' error!");
    }

    return false;
}


//===================================================================================

/*
Item * ItemDataLUT::find(ItemDataIdentifier *_identifier)
{

    for(Item* i : mItems){
        if(i->sourceItem()->itemDataIdentifier() == _identifier){
            return i;
        }
    }


    return nullptr;;

}
*/


}
