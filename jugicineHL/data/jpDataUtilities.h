#ifndef PLAYER__DATA__UTILITIES_H
#define PLAYER__DATA__UTILITIES_H

#include <assert.h>
#include <string>
#include <vector>

#include "jpData.h"
//#include "jpData.h"

namespace pugi {

class xml_node;

}



namespace jugimap{


class BaseObject;
class LogicState;
class PlayedScene;
class SourceItem;
class Item;
class Character;
struct SourceItem;
class ItemData;
struct ItemDataParsingHelper;
class OriginObjectObtainer;
struct ItemDataIdentifier;
//class DoSetData;
//class ACOnData;




struct ItemDataCollectingHelper
{

    unsigned int childSourceItemsDepthIndex = 0;
    std::vector<SourceItem*>childSourceItems;       // LINKS
    std::string itemDataName;
    //---

};



enum class ParseItemMode
{
    ITEM,
    DATA
};


struct ItemDataPair
{

    ~ItemDataPair();


    Item *item = nullptr;           // LINK
    ItemData* data = nullptr;       // LINK

    //int itemIndex = -1;
    SourceItem *sourceItem = nullptr;
    int dataIndex = -1;
    ItemDataType dataType = ItemDataType::UNKNOWN;
    OriginObjectObtainer *originObjectObtainer = nullptr;       // OWNED
    BaseObject * originObject = nullptr;                        // LINK

    unsigned int itemTagFlags = 0;


    bool updateItemAndData();


};



/*
class RootItemParameter
{
public:

    RootItemParameter(const std::string &_name) : mName(_name){}

    const std::string & name() const { return mName; }
    Item * rootItem(){ return mRootItem; }
    void setRootItem(Item *_item){ mRootItem = _item; }


private:
    std::string mName;
    Item * mRootItem = nullptr;     // LINK

};

*/

enum class ArithemticOperator
{
    NONE,
    ADD,
    SUBTRACT,
    MULTIPLY
};


enum class DataAccessorType
{
    DATA_QUERY,
    DATA_SETTER,
    ITEM_QUERY,
    ITEM_MOVER,
    DATA_TO_TEXT,
    UNKNOWN
};



class ItemDataAccessor
{
public:
    //friend class DoSetData;
    //friend class ACOnData;


    enum class RightSideContent
    {
        DIRECT_VALUE,
        DATA,
        MIN_VALUE_OF_LEFT_DATA,
        MAX_VALUE_OF_LEFT_DATA,
        ITEM,
        SOURCE_ITEM
    };


    struct DirectValues
    {
        int mIntValue = 0;
        float mFloatValue = 0.0f;
        bool mBoolValue = false;
    };


    ItemDataAccessor(DataAccessorType _type);

    ItemDataPair & leftSideItemDataPair(){ return mLeftSideItemDataPair;}
    ItemDataPair & rightSideItemDataPair(){ return mRightSideItemDataPair;}
    DirectValues & directValues(){ return mDirectValues; }

    bool accessorNotUsed(){ return mAccessorNotUsed;}

protected:

    DataAccessorType mType = DataAccessorType::UNKNOWN;
    ItemDataPair mLeftSideItemDataPair;
    ItemDataPair mRightSideItemDataPair;
    DirectValues mDirectValues;

    bool mAccessorNotUsed = false;


    RightSideContent mRightSideContent = RightSideContent::DIRECT_VALUE;


    bool parseLeftSide(PlayedScene *_scene, const std::string &_path, ItemDataParsingHelper &dpi);
    bool parseRightSide(PlayedScene *_scene, const std::string &_path, ItemDataParsingHelper &dpi);



};



class DataQuery : public ItemDataAccessor
{
public:
    //friend class ACOnData;


    DataQuery();

    bool parse(PlayedScene *_scene, const std::string & _fullPath, BaseObject *_rootParentObject, LogicState *_logicState, ParseItemMode _parseItemMode);


    bool isTrue();

private:


};


//==============================================================


class DataSetter : public ItemDataAccessor
{
public:
    //friend class DoSetData;

    DataSetter();

    bool parse(PlayedScene *_scene, const std::string & _fullPath, BaseObject *_rootParentObject, LogicState *_logicState, ParseItemMode _parseItemMode);


    void set();


private:

    ArithemticOperator mArithemticOperation = ArithemticOperator::NONE;

};


//==============================================================


class ItemQuery : public ItemDataAccessor
{
public:

    enum class Operation
    {
        CONTAINS_STORED_CHILD_ITEM,
        CONTAINS_SELECTED_CHILD_ITEM
    };


    ItemQuery();

    bool parse(PlayedScene *_scene, const std::string & _fullPath, BaseObject *_rootParentObject, LogicState *_logicState, ParseItemMode _parseItemMode);


    bool isTrue();


private:
    Operation mOperation = Operation::CONTAINS_STORED_CHILD_ITEM;

    //ArithemticOperator mArithemticOperation = ArithemticOperator::NONE;

};


//==============================================================



class ItemSetter : public ItemDataAccessor
{
public:
    //friend class DoSetData;
    enum class Operation
    {
        ADD_CHILD_ITEM,
        REMOVE_CHILD_ITEM
    };


    ItemSetter();

    bool parse(PlayedScene *_scene, const std::string & _fullPath, BaseObject *_rootParentObject, LogicState *_logicState, ParseItemMode _parseItemMode);


    void set();


private:

    Operation mOperation = Operation::ADD_CHILD_ITEM;

};



//==============================================================


class DataToText : public ItemDataAccessor
{
public:
    //friend class DoSetData;

    DataToText();

    bool parse(PlayedScene *_scene, const std::string & _fullPath, BaseObject *_rootParentObject, LogicState *_logicState, ParseItemMode _parseItemMode);


    std::string text(bool includeDataName=true);


private:

};



struct ItemDataParsingHelper
{

    BaseObject *rootParentObject = nullptr;
    LogicState *logicState = nullptr;

    std::string originObjectPath;
    std::string itemAndDataPath;
    //std::string signalStyle;

    bool parseMainPaths(const std::string &_fullSignalPath);


    //----
    bool accessorNotUsed = false;
    DataAccessorType dataAccessorType = DataAccessorType::UNKNOWN;
    ParseItemMode parseItemMode = ParseItemMode::DATA;
    //SourceItem *sourceItem = nullptr;
    bool parsingOK = false;


    ItemDataPair *accesorData = nullptr;       // LINK


    //int dataIndex = -1;



    //----
    //Item *item = nullptr;                           // LINK
    //ItemData *data = nullptr;
    //BaseObject * originObject = nullptr;         // LINK

    //---
    //OriginObjectObtainer *originObjectObtainer = nullptr;   // LINK ( ownership transfered to signal accessor )

};


//===================================================================================


/*
struct ItemIdentifier
{
    std::string itemPath;
};



class ItemIdentifiers
{
public:

    ItemIdentifier * add(const std::string &_itemPath);
    bool getParsingHelper(ItemDataParsingHelper &_iph, const std::string &_itemPath, bool _setErrorMessage=true);

    std::vector<ItemIdentifier> & identifiers(){ return mIdentifiers; }

private:
    std::vector<ItemIdentifier>mIdentifiers;

};
*/

//===================================================================================



struct ItemDataIdentifier
{
    SourceItem *sourceItem = nullptr;
    std::string itemPath;
    std::vector<std::pair<std::string, ItemDataType>>dataNamesAndTypes;
};



class ItemDataIdentifiers
{
public:

    ItemDataIdentifier * add(SourceItem *_sourceItem, const std::string &_itemPath, std::vector<ItemData*> &_data);
    bool getParsingHelper(ItemDataParsingHelper &_iph, const std::string &_itemDataPath, bool _setErrorMessage=true);

    std::vector<ItemDataIdentifier> & identifiers(){ return mIdentifiers; }

private:
    std::vector<ItemDataIdentifier>mIdentifiers;
    //std::vector<std::string>rootItemDataNames;


};


//===================================================================================

/*
class ItemDataLUT
{
public:

    Item * find(ItemDataIdentifier *_identifier);

private:
    std::vector<Item*>mItems;

};

*/


}


#endif
