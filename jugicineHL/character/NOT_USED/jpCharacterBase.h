#ifndef PLAYER__CHARACTER_BASE_H
#define PLAYER__CHARACTER_BASE_H

#include <assert.h>
#include <string>
#include <vector>

#include "jmSignal.h"
#include "jmStorage.h"

#include "jpLogicState.h"
#include "jpLogicStateCfg.h"
#include "jpGlobal.h"


namespace pugi {

class xml_node;

}



namespace jugimap{



//class Class;
class Item;
class SourceItem;
class CharacterSystem;
class ItemDataAction;
struct ParserPathway;
struct ItemDataCollectingHelper;

class SourceItemDataException
{
public:

    bool initCfg(const pugi::xml_node &_node);
    bool build(CharacterSystem *_characterSystem);



private:

    ItemDataAction* mItemDataAction = nullptr;
    SourceItem* mRootSourceItem = nullptr;
    SourceItem* mChildSourceItem = nullptr;

    float mFactor = 1.0;

    //---
    std::string mCaseCfg;

};







//=============================================================================================

enum class ItemDataType
{
    INT,
    FLOAT,
    BOOL
};


struct ItemSourceData
{

    ItemSourceData(const std::string &_name, ItemDataType _type) : mName(_name), mType(_type){}

    bool initCfg_exceptions(const pugi::xml_node &_node);

    const std::string & name(){ return mName;}

    std::string mName;
    ItemDataType mType;

    std::vector<SourceItemDataException>mExceptions;

};


struct IntSourceItemData : public ItemSourceData
{

    IntSourceItemData(const std::string &_name);

    bool initCfg(const pugi::xml_node &_node);

    int mMinValue = 0;
    int mMaxValue = 0;
    int mStartValue = 0;
};


struct FloatSourceItemData : public ItemSourceData
{

    FloatSourceItemData(const std::string &_name);
    bool initCfg(const pugi::xml_node &_node);


    float mMinValue = 0.0f;
    float mMaxValue = 0.0f;
    float mStartValue = 0.0f;
};


struct BoolSourceItemData : public ItemSourceData
{

    BoolSourceItemData(const std::string &_name);
    bool initCfg(const pugi::xml_node &_node);


    bool mStartValue = false;
};


//----------------------------------------------------------------

struct ItemData
{
    ItemData(ItemDataType _type, Item* _parentItem) : mType(_type), mParentItem(_parentItem){}
    virtual ~ItemData(){}


    const std::string & name();
    ItemDataType type(){ return mType;}

    ItemDataType mType;
    Item* mParentItem = nullptr;
};


struct IntItemData : public ItemData
{
    IntItemData(IntSourceItemData *_srcData, Item* _parentItem);

    bool isValue(int _value){ return mValue == _value; }

    int value(){ return mValue; }
    void setValue(int _value){ mValue = ClampValue(_value, srcData->mMinValue, srcData->mMaxValue); }


    IntSourceItemData *srcData = nullptr;
    int mValue = 0.0;
};


struct FloatItemData : public ItemData
{

    FloatItemData(FloatSourceItemData* _srcData, Item* _parentItem);

    bool isValue(float _value){ return mValue == _value; }

    float value(){ return mValue; }
    void setValue(float _value){ mValue = ClampValue(_value, srcData->mMinValue, srcData->mMaxValue); }


    FloatSourceItemData* srcData = nullptr;
    float mValue = 0.0;
};


struct BoolItemData : public ItemData
{

    BoolItemData(BoolSourceItemData *_srcData, Item* _parentItem);


    bool isValue(bool _value){ return mValue == _value; }


    bool value(){ return mValue; }
    void setValue(bool _value){ mValue = _value; }


    BoolSourceItemData *srcData = nullptr;
    bool mValue = 0.0;

};





//=============================================================================================
/*

struct AttributeInfluenceExceptionCfg
{

    bool initCfg(const pugi::xml_node &_node);

    std::string _case;
    float value = 0.0f;
    float factor = 1.0f;

};



class AttributeInfluenceException
{
public:

    AttributeInfluenceException(AttributeInfluenceExceptionCfg* _cfg) : mAttributeInfluenceExceptionCfg(_cfg){}

    bool initConnections();

private:
    AttributeInfluenceExceptionCfg* mAttributeInfluenceExceptionCfg = nullptr;      //LINK


};



//=============================================================================================


struct ItemInfluenceCfg
{

    bool initCfg(const pugi::xml_node &_node);

    std::string influencedItemData;
    //bool valueFromItem = false;
    std::string influencingItemData;

    float value = 0.0f;
    float factor = 1.0f;

    std::vector<AttributeInfluenceExceptionCfg>exceptions;

};


class ItemInfluence
{
public:

    ItemInfluence(ItemInfluenceCfg* _cfg, Item* _parentItem) : mCfg(_cfg), mParentItem(_parentItem){}

    bool build(CharacterSystem *_characterSystem);
    bool initConnections(CharacterSystem *_characterSystem);




private:
    ItemInfluenceCfg * mCfg = nullptr;       // LINK
    Item * mParentItem = nullptr;

    ItemData* mParentItemInfluencingData = nullptr;                             // LINK

    SourceItem *mInfluencedSourceItem = nullptr;                        // LINK
    ItemSourceData *mInfluencedItemSourceData = nullptr;                        // LINK

    std::vector<AttributeInfluenceException>mInfluenceExceptions;

};

*/

//=============================================================================================






class SourceItem
{
public:

    SourceItem(const std::string &_name);

    bool initCfg(CharacterSystem *_characterSystem, const pugi::xml_node &_node);
    bool build(CharacterSystem *_characterSystem);


    const std::string & name(){ return mName; }

    bool isUniqe(){ return mUnique; }
    bool isExclusiveGroup(){ return mExclusiveGroup; }

    SimpleNoNameStorage<ItemSourceData*> & sourceData(){ return mSourceData; }
    //std::vector<ItemInfluenceCfg> & influenceCfgs(){ return mInfluenceCfgs; }
    std::vector<SourceItem*> & childSourceItems(){ return mChildItemsCfgs; }

    bool findChildSourceItemViaPath(const std::string &_path, ParserPathway &_parserPathway, bool _setErrorMessage=true);




private:
    std::string mName;

    bool mUnique = false;
    bool mExclusiveGroup = false;
    float mChildItemsDataFactor = 1.0f;
    //IntSourceItemData mAmount{"amount"};

    SimpleNoNameStorage<ItemSourceData*>mSourceData;
    //std::vector<ItemInfluenceCfg>mInfluenceCfgs;
    std::vector<SourceItem*>mChildItemsCfgs;               // LINKS

    //---
    std::vector<std::string>mAddedChildred;                 // temporary vector required for adding children in build phase

};



class Item
{
public:

    Item(SourceItem *_cfg) : mItemCfg(_cfg){}
    ~Item();

    bool build(CharacterSystem *_characterSystem);
    bool initConnections(CharacterSystem *_characterSystem);

    float obtainInfluence_float(FloatSourceItemData * _itemSourceData);

    bool isSelected(){ return mSelected; }

    const std::string & name(){ return mItemCfg->name(); }
    SourceItem * sourceItem(){ return mItemCfg; }
    Item* parentItem(){ return mParentItem; }


    std::vector<Item*> & childItems(){ return mChildItems; }
    ItemData* findData(const std::string _name, bool _setErrorMessage=true);

    bool collectData(ItemDataCollectingHelper &helper, float & collected);


private:
    SourceItem *mItemCfg = nullptr;         // LINK
    Item *mParentItem = nullptr;         // LINK

    bool mSelected = 0;

    std::vector<ItemData*>mData;                        // OWNED
    //std::vector<ItemInfluence>mAttributeInfluences;

    std::vector<Item*>mChildItems;

};





}


#endif
