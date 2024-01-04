#ifndef PLAYER__DATA__ITEM_BASE_H
#define PLAYER__DATA__ITEM_BASE_H

#include <assert.h>
#include <string>
#include <vector>

#include "jmSignal.h"
#include "jmStorage.h"

#include "jpDataUtilities.h"

//#include "jpLogicState.h"
//#include "jpLogicStateCfg.h"
#include "jpGlobal.h"
//#include "jpData.h"


namespace pugi {

class xml_node;

}



namespace jugimap{



//class Class;
class Action;
class Item;
class SourceItem;
class ItemData;
class PlayedScene;
struct ItemDataPair;
class SourceItemsStorage;
//class SourceItemsStorage;
//class ItemDataAction;
//struct ParserPathway;
//struct ItemDataCollectingHelper;


enum class ItemType
{
    NORMAL,
    COUNTABLE,
    SELECTABLE_GROUP
};



class SourceItem
{
public:
    friend class SourceItemsStorage;


    SourceItem();
    //SourceItem(const std::string &_name);

    ~SourceItem();

    bool initCfg(const pugi::xml_node &_node);
    bool build(SourceItemsStorage *_sourceItemsStorage);
    bool initConnections(PlayedScene *_scene);

    const std::string & name() const { return mName; }

    ItemType type() const { return mType; }
    //int storageIndex(){ return mStorageIndex; }

    SimpleNoNameStorage<ItemData*> & sourceData(){ return mSourceData; }
    std::vector<SourceItem*> & childSourceItems(){ return mChildSourceItems; }
    PrebuiltStorage<SourceItem> & createdChildSourceItems(){ return mCreatedChildSourceItems; }
    std::vector<SourceItem*> &allowedChildSourceItems(){ return mAllowedChildSourceItems;}


    //bool acceptingChildItem(SourceItem* _sourceItem);
    SourceItemsStorage *parentStorage(){ return mParentStorage; }
    //bool findChildSourceItemViaPath(const std::string &_path, ParserPathway &_parserPathway, bool _setErrorMessage=true);

    SourceItem *proxy(){ return mProxy;}
    ItemData *countableData(){ return mCountableData; }

    bool nativeChild(){ return mNativeChild;}
    //Item *newItem();


    //ItemIdentifiers & itemIdentifiers(){ return mItemIdentifiers; }
    //void buildLUT(ItemIdentifiers &_lut, std::string _prevIdentifier, int _depth);

    ItemDataIdentifiers & itemDataIdentifiers(){ return mItemDataIdentifiers; }
    void buildParsingIdentifiers(ItemDataIdentifiers &_idi, std::string _prevIdentifier, int _depth);

    //ItemDataIdentifier* itemDataIdentifier(){return mItemDataIdentifier; }

    //PoolStorage<Item*,10> & itemsPool(){ return mItemsPool; }
    //Item *acquireAndSetupNewItem(Item* _parentItem);
    //Item *acquireAndAssignItem(Item* _parentItem, Item* _srcItem);
    //void releaseItem(Item* _item);

    unsigned int tagFlags(){ return mTags; }


private:
    SourceItemsStorage *mParentStorage = nullptr;
    std::string mName;

    ItemType mType = ItemType::NORMAL;
    std::string mCountableDataName;
    ItemData *mCountableData = nullptr;
    float mChildItemsDataFactor = 1.0f;
    //int mStorageIndex = -1;

    std::string mProxyName;
    SourceItem *mProxy = nullptr;
    bool mNativeChild = false;

    SimpleNoNameStorage<ItemData*>mSourceData{"A data object"};                    // OWNED
    //SimpleNoNameStorage<SourceItem>mCreatedChildSourceItems;            // OWNED
    PrebuiltStorage<SourceItem>mCreatedChildSourceItems;

    std::vector<SourceItem*>mChildSourceItems;                       // LINKS ( created + added source items )
    std::vector<SourceItem*>mAllowedChildSourceItems;                // LINKS ( global source items )

    //---
    std::vector<std::string>mAddedChildrenNames;                 // temporary vector required for adding children in build phase
    std::vector<std::string>mAllowedChildrenNames;                 // temporary vector required for adding children in build phase

    //ItemIdentifiers mItemIdentifiers;
    ItemDataIdentifiers mItemDataIdentifiers;

    std::vector<std::string>mTagNames;
    unsigned int mTags = 0;


    //PoolStorage<Item*,10>mItemsPool;
    //ItemDataIdentifier *mItemDataIdentifier = nullptr;      // LINK

};



enum class AddChildItemMode
{
    COPY,
    MOVE
};


class Item : public BaseObject
{
public:
    friend class SourceItemsStorage;

    Item();
    Item(SourceItem *_cfg);
    Item & operator =(const Item &_src);
    ~Item();

    void clearContent();
    bool setup_newItem(SourceItem *_sourceItem, Item *_parentItem);
    //bool setup_newItem();
    bool setup_copyItem(SourceItem *_sourceItem, Item *_parentItem, Item *_src);
    //bool initConnections(PlayedScene *_scene);

    bool isSelectable(){ return (mParentItem && mParentItem->type()==ItemType::SELECTABLE_GROUP); }
    bool isSelected(){ return mSelected; }
    void setSelected(bool _selected){ mSelected = _selected; }


    const std::string & name(){ return mSourceItem->name(); }
    SourceItem * sourceItem(){ return mSourceItem; }
    Item* parentItem(){ return mParentItem; }
    ItemType type(){ return mSourceItem->type(); }
    ItemData* countableData(){ return mCountableData; }


    bool addChildItem(SourceItem *_sourceItem);
    bool addChildItem(Item *_item, AddChildItemMode _mode);
    bool removeChildItem(SourceItem *_sourceItem);
    bool removeChildItem(Item *_item);

    //std::vector<Item*> & childItems(){ return mChildItems; }
    std::vector<ItemData*> & dataStorage(){ return mData; }
    std::vector<Item*> & childItems(){ return mChildItems; }

    Item* findFirstChildItem(SourceItem* _sourceItem);
    Item* findChildItem(SourceItem* _sourceItem, bool _searchForProxy = true);
    void collectChildItems(std::vector<Item *> &_collectedItems, SourceItem* _sourceItem);
    void collectChildItemsWithTagFlag(std::vector<Item *> &_collectedItems, unsigned int _tagFlags);

    Item* findSelectedChildItem();

    Item* findChildItem(const std::string _name, bool _setErrorMessage=true);
    ItemData* findData(const std::string _name, bool _setErrorMessage=true);
    void findChildItemDataViaPath(const std::string &_path, ItemDataPair & idp, ParseItemMode _mode,  bool _setErrorMessage=true);
    void findChildItemDataViaPath(std::vector<std::string> &itemPathParts, ItemDataPair &idp, ParseItemMode _mode, bool _setErrorMessage=true);


    //bool collectData(ItemDataCollectingHelper &helper, float & collected);
    PoolObjectPos & poolPos(){ return mPoolPos; }

    SignalCallback *callbackOnChangedChildreen(){ return mCallbackOnChangedChildreen; }

    void setCallbackOnChangedChildreen(SignalCallback * _callback ){ mCallbackOnChangedChildreen = _callback; }

private:
    SourceItem *mSourceItem = nullptr;         // LINK
    Item *mParentItem = nullptr;         // LINK

    ItemData *mCountableData = nullptr;             // LINK (quick access)
    //bool mSelectable = true;
    bool mSelected = false;

    std::vector<ItemData*>mData;              // OWNED
    //SimpleNoNameStorage<ItemData*>mData;        // owned
    //std::vector<ItemInfluence>mAttributeInfluences;

    std::vector<Item*>mChildItems;                // OWNED
    //SimpleNoNameStorage<Item*>mChildItems;      // owned

    PoolObjectPos mPoolPos;

    //Action* mOnSelectedAction
    bool built = false;

    SignalCallback *mCallbackOnChangedChildreen = nullptr;        // LINK


    Item* checkAndAddCountableProxyItem(SourceItem *_proxySourceItem);



};



class ItemTag
{
public:
    ItemTag(const std::string &_name) : mName(_name){}

    const std::string &name(){ return mName; }

    unsigned int tagFlag(){ return mTagFlag; }
    void setTagFlag(unsigned int _tagFlag){ mTagFlag = _tagFlag; }


private:
    std::string mName;
    unsigned int mTagFlag = 0;

};



class SourceItemsStorage
{
public:

    bool preload_countObjects(const pugi::xml_node &_node);
    bool initCfg(const pugi::xml_node &_node);
    bool buildObjects(PlayedScene *_scene);
    bool initConnections(PlayedScene *_scene);


    SourceItem* findSourceItem(const std::string &_sourceItemName, bool _setErrorMessage=true);
    Item *acquireAndSetupItem(SourceItem* _sourceItem, Item* _parentItem);     // _sourceItem can be also stored inside other source item
    Item *acquireAndCopyItem(SourceItem* _sourceItem, Item* _parentItem, Item* _srcItem);     // _sourceItem can be also stored inside other source item
    void releaseItem(Item* _item);

    PrebuiltStorage<SourceItem>& sourceItems(){ return mGlobalSourceItems; }
    PoolStorage<Item,20>& itemsPool(){ return mItemsPool; }

    SimpleNoNameStorage<ItemTag> &itemsTagsStorage(){ return mItemTags; }

private:
    PrebuiltStorage<SourceItem>mGlobalSourceItems;
    PoolStorage<Item,20>mItemsPool;

    std::vector<std::string>sourceItemsNames;      // temporary vector for prebuilding source items

    SimpleNoNameStorage<ItemTag>mItemTags{"An item tag"};

};



}


#endif
