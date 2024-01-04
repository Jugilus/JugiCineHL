#include <limits>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmSprite.h"
#include "jmCompositeSprite.h"
#include "jmStreams.h"

#include "logic/jpLogicState.h"
#include "jpPlayedScene.h"
#include "jpPlayedApp.h"

//#include "jpCharacterSystem.h"
//#include "jpItemUtilities.h"
#include "jpData.h"
#include "jpItem.h"


namespace jugimap{





//====================================================================================


SourceItem::SourceItem()
{
    //mSourceData.addObject(new IntItemData("amount"));
}


/*
SourceItem::SourceItem(const std::string &_name) :
    mName(_name)
{

    mSourceData.addObject(new IntItemData("amount"));

}
*/




SourceItem::~SourceItem()
{


}


bool SourceItem::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading item '" + mName +"' ...");

    if(name()=="hero"){
        DummyFunction();
    }

    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
            //mName = a.as_string("name");

        /*
        }else if(attributeName=="amount"){
            std::string sValue = a.as_string();

            IntItemData* d = static_cast<IntItemData*>(mSourceData.getObject("amount"));
            int value = 0;
            if(parseIntData(sValue, value)==false){
                return false;
            }
            d->setValue(value);

        }else if(attributeName=="maxAmount"){
            std::string sValue = a.as_string();

            IntItemData* d = static_cast<IntItemData*>(mSourceData.getObject("amount"));
            int value = 0;
            if(parseIntData(sValue, value)==false){
                return false;
            }
            d->_setMaxValue(value);

        }else if(attributeName=="minAmount"){
            std::string sValue = a.as_string();

            IntItemData* d = static_cast<IntItemData*>(mSourceData.getObject("amount"));
            int value = 0;
            if(parseIntData(sValue, value)==false){
                return false;
            }
            d->_setMinValue(value);

        }else if(attributeName=="unique"){
            std::string sValue = a.as_string();

            if(StdString::boolValue(sValue, mUnique)==false){
                return false;
            }


        }else if(attributeName=="exclusive"){
            std::string sValue = a.as_string();

            if(StdString::boolValue(sValue, mExclusiveGroup)==false){
                return false;
            }

        */

        }else if(attributeName=="type"){
            std::string sValue = a.as_string();
            if(sValue=="COUNTABLE"){
                mType = ItemType::COUNTABLE;

            }else if(sValue=="SELECTABLE_GROUP"){
                mType = ItemType::SELECTABLE_GROUP;

            }else{
                dbgSystem.addMessage("Wrong type '" + sValue + "' !");
                return false;

            }

        }else if(attributeName=="countableData"){
            mCountableDataName = a.as_string();

        }else if(attributeName=="countableProxyItem"){
            mProxyName = a.as_string();

        }else if(attributeName=="childItemsDataFactor"){
            std::string sValue = a.as_string();

            if(StdString::floatNumber(sValue, mChildItemsDataFactor)==false){
                return false;
            }

        }else if(attributeName=="tags"){
            std::string sValue = a.as_string();
            mTagNames = StdString::splitString(sValue, ",");


        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }


    //----- prebuild created child source items
    std::vector<std::string>createdChildNames;

    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="item"){
            std::string itemName = n.attribute("name").as_string();
            if(itemName.empty()){
                dbgSystem.addMessage("Missing item name!");
                return false;
            }
            if(StdVector::contains(createdChildNames, itemName)){
                dbgSystem.addMessage("Item name '" + itemName +"' already used!");
                return false;
            }
            createdChildNames.push_back(itemName);
        }
    }

    mCreatedChildSourceItems.preBuild(createdChildNames.size());
    for(unsigned int i=0; i<createdChildNames.size(); i++){
         mCreatedChildSourceItems.objects()[i].mName = createdChildNames[i];
    }


    //-----
    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="int"){

            IntItemData *sid = new IntItemData(n.attribute("name").as_string());
            if(sid->initCfg(n)==false){
                return false;
            }
            if(mSourceData.addObject(sid)==nullptr){
                return false;
            }


        }else if(nodeName=="float"){

            FloatItemData *sid = new FloatItemData(n.attribute("name").as_string());
            if(sid->initCfg(n)==false){
                return false;
            }
            if(mSourceData.addObject(sid)==nullptr){
                return false;
            }


        }else if(nodeName=="bool"){

            BoolItemData *sid = new BoolItemData(n.attribute("name").as_string());
            if(sid->initCfg(n)==false){
                return false;
            }
            if(mSourceData.addObject(sid)==nullptr){
                return false;
            }

        }else if(nodeName=="sprite"){

            SpriteData *sd = new SpriteData(n.attribute("name").as_string());
            if(sd->initCfg(n)==false){
                return false;
            }
            if(mSourceData.addObject(sd)==nullptr){
                return false;
            }

        }else if(nodeName=="string"){

            TextData *sd = new TextData(n.attribute("name").as_string());
            if(sd->initCfg(n)==false){
                return false;
            }
            if(mSourceData.addObject(sd)==nullptr){
                return false;
            }

        }else if(nodeName=="textSegment"){

            TextSegmentData *sd = new TextSegmentData(n.attribute("name").as_string());
            if(sd->initCfg(n)==false){
                return false;
            }
            if(mSourceData.addObject(sd)==nullptr){
                return false;
            }

        }else if(nodeName=="aniSpriteObject"){

            AniSpriteObjectData *sd = new AniSpriteObjectData(n.attribute("name").as_string());
            if(sd->initCfg(n)==false){
                return false;
            }
            if(mSourceData.addObject(sd)==nullptr){
                return false;
            }

        }else if(nodeName=="tween"){

            TweenData *sd = new TweenData(n.attribute("name").as_string());
            if(sd->initCfg(n)==false){
                return false;
            }
            if(mSourceData.addObject(sd)==nullptr){
                return false;
            }


        /*
        }else if(nodeName=="influence"){

            mInfluenceCfgs.emplace_back();
            ItemInfluenceCfg & aiCfg = mInfluenceCfgs.back();
            if(aiCfg.initCfg(n)==false){
                return false;
            }
        */

        }else if(nodeName=="item"){

            //SourceItem *childItem = _characterSystem->initCfg_addSourceItem(n);
            //if(childItem==nullptr){
            //    return false;
            //}

            //mChildItemsCfgs.push_back(childItem);



            /*
            std::string itemName = n.attribute("name").as_string();
            if(itemName.empty()){
                dbgSystem.addMessage("Missing item name!");
                return false;
            }

            SourceItem *sourceItem =  mCreatedChildSourceItems.addObject(SourceItem(itemName));
            if(sourceItem==nullptr){
                return false;
            }
            */

            SourceItem *sourceItem =  mCreatedChildSourceItems.getObject(n.attribute("name").as_string());
            assert(sourceItem);

            if(sourceItem->initCfg(n)==false){
                return false;
            }

            sourceItem->mNativeChild = true;
            mChildSourceItems.push_back(sourceItem);


        }else if(nodeName=="addedChildren"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="items"){
                    std::string sItems = a.as_string();
                    std::vector<std::string>itemNames = StdString::splitString(sItems, ",");
                    for(const std::string & s : itemNames){
                        if(StdVector::contains(mAddedChildrenNames, s)==false){
                            mAddedChildrenNames.push_back(s);
                        }
                    }
                    //mAddedChildrenNames = StdString::splitString(sItems, ",");

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else if(nodeName=="allowedChildren"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="items"){
                    std::string sItems = a.as_string();
                    std::vector<std::string>itemNames = StdString::splitString(sItems, ",");
                    for(const std::string & s : itemNames){
                        if(StdVector::contains(mAllowedChildrenNames, s)==false){
                            mAllowedChildrenNames.push_back(s);
                        }
                    }
                    //mAllowedChildrenNames = StdString::splitString(sItems, ",");

                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else{

            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;

        }
    }

    if(mType==ItemType::COUNTABLE){
        if(mCountableDataName.empty()){
            dbgSystem.addMessage("Item is defined as countable but 'countableData' attribute is missing!");
            return false;
        }

        mCountableData = mSourceData.getObject(mCountableDataName, false);
        if(mCountableData==nullptr){
            dbgSystem.addMessage("Countable data '" + mCountableDataName + "not found!");
            return false;
        }

        if(!(mCountableData->type()==ItemDataType::INT || mCountableData->type()==ItemDataType::FLOAT)){
            dbgSystem.addMessage("The defined countable data '" + mCountableDataName + " is not of numeric type (integer or float)!");
            return false;

        }

    }



    dbgSystem.removeLastMessage();
    return true;

}


bool SourceItem::build(SourceItemsStorage *_sourceItemsStorage)
{

    dbgSystem.addMessage("Building item '" + mName +"' ...");

    if(name()=="hero"){
        DummyFunction();
    }

    mParentStorage = _sourceItemsStorage;


    for(SourceItem &si : mCreatedChildSourceItems.objects()){
        if(si.build(_sourceItemsStorage)==false){
            return false;
        }
    }


    for(const std::string &s : mAllowedChildrenNames){

        SourceItem *sc = _sourceItemsStorage->sourceItems().getObject(s);
        if(sc==nullptr){
            return false;
        }

        /*
        if(sc->type()==ItemType::COUNTABLE){
            ItemData *scCountableData = sc->countableData();
            ItemData *linkedData = mSourceData.getObject(scCountableData->name(),false);
            if(linkedData==nullptr){
                dbgSystem.addMessage("Allowed child item '" + sc->name() + " is countable with countable data '" + scCountableData->name() +"'!");
                dbgSystem.addMessage("There is no item data with that name inside item '" + mName + "' !");
                return false;
            }
            if(linkedData->type()!=scCountableData->type()){
                dbgSystem.addMessage("Countable data '" + scCountableData->name() +"' of allowed item '" + sc->name() +"' is of different type than the connected data of the item '" + mName +"'!");
                return false;
            }
        }
        */

        mAllowedChildSourceItems.push_back(sc);
    }


    for(const std::string &s : mAddedChildrenNames){

        SourceItem *sc = _sourceItemsStorage->sourceItems().getObject(s);
        if(sc==nullptr){
            return false;
        }

        if(StdVector::contains(mAllowedChildSourceItems, sc)==false){
             dbgSystem.addMessage("Item '" + sc->name() + " can not be added to the child items becouse it is not among allowed items!");
             return false;
        }

        mChildSourceItems.push_back(sc);
    }


    if(mProxyName.empty()==false){

        if(mType==ItemType::COUNTABLE){
            mProxy = _sourceItemsStorage->sourceItems().getObject(mProxyName,false);
            if(mProxy==nullptr){
                dbgSystem.addMessage("Proxy item '" + mProxyName + " not found!");
                return false;
            }

        }else{
            mProxyName.clear();
        }

    }


    //----
    //for(unsigned int i=0; i<mChildSourceItems.size(); i++){
    //    mChildSourceItems[i]->mStorageIndex = i;
    //}

    for(const std::string &tagName : mTagNames){
        ItemTag *it = _sourceItemsStorage->itemsTagsStorage().getObject(tagName);
        if(it==nullptr){
            return false;
        }
        mTags |= it->tagFlag();
    }


    dbgSystem.removeLastMessage();
    return true;
}


bool SourceItem::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing item '" + name() + "' ...");

    if(name()=="hero"){
        DummyFunction();
    }


    for(ItemData *id : mSourceData.objects()){
        if(id->initConnections(_scene)==false){
            return false;
        }
    }

    for(SourceItem& i : mCreatedChildSourceItems.objects()){
        if(i.initConnections(_scene)==false){
            return false;
        }
    }


    //for(ItemInfluence & ai : mAttributeInfluences){
    //    if(ai.initConnections(_characterSystem)==false){
    //        return false;
    //    }
   // }


    dbgSystem.removeLastMessage();
    return true;

}


void SourceItem::buildParsingIdentifiers(ItemDataIdentifiers &_idi, std::string _prevIdentifier, int _depth)
{

    std::string identifier = _prevIdentifier;


     // root (_depth==0) item is not included in the identifier path

    if(&mItemDataIdentifiers == &_idi){     //ROOT

        _idi.add(this, "", mSourceData.objects());      // root item has empty string as item identifier (you don't write it in cfg path)

    }else if(_depth>0){

        if(identifier.empty()==false){
            identifier += ":";
        }
        identifier += mName;

        _idi.add(this, identifier, mSourceData.objects());
    }

    //---
    for(SourceItem &si : mCreatedChildSourceItems.objects()){
        si.buildParsingIdentifiers(_idi, identifier, _depth+1);
    }

    //---
    for(SourceItem *si : mAllowedChildSourceItems){
        si->buildParsingIdentifiers(_idi, identifier, _depth+1);
    }

}

/*

Item * SourceItem::acquireAndSetupNewItem(Item* _parentItem)
{

    Item *item = mItemsPool.acquireObject();

    if(item->setup_newItem(this, _parentItem, _srcItem)==false){
        return nullptr;
    }

    return item;

}

Item * SourceItem::acquireAndAssignItem(Item* _parentItem, Item* _srcItem)
{

    Item *item = mItemsPool.acquireObject();

}

*/




/*

bool SourceItem::acceptingChildItem(SourceItem* _sourceItem)
{

    for(SourceItem *si : mAllowedChildSourceItems){
        if(si == _sourceItem){
            return true;
        }
    }

    return true;

}
*/

/*
Item* SourceItem::newItem()
{



    //Item *i = new Item(this);

    Item *i = mParentStorage->itemsPool().acquireObject();

    if(i->setup(this, false)==false){
        return nullptr;
    }

    return i;

}
*/

//====================================================================================


Item::Item()
{
    mBaseType = BaseObjectType::ITEM;
}


Item::Item(SourceItem *_cfg) : mSourceItem(_cfg)
{
    mBaseType = BaseObjectType::ITEM;
}


Item & Item::operator=(const Item &_src)
{
    if(&_src==this){
        return *this;
    }

    assert(_src.mSourceItem == mSourceItem);





    return *this;
}



Item::~Item()
{

    for(ItemData* id : mData){
        delete id;
    }

    // All items and child items are stored in the storage pool

    //for(Item* id : mChildItems){
    //    delete id;
    //}

}


void Item::clearContent()
{

    for(ItemData* id : mData){
        delete id;
    }
    mData.clear();


    //----
    for(Item *i : mChildItems){
        i->sourceItem()->parentStorage()->releaseItem(i);
    }
    mChildItems.clear();


    //----
    mSourceItem = nullptr;
    mParentItem = nullptr;
    mCountableData = nullptr;
    mSelected = false;
    built = false;

}


bool Item::setup_newItem(SourceItem *_sourceItem, Item *_parentItem)
{

    assert(built==false);

    mSourceItem = _sourceItem;
    mParentItem = _parentItem;

    //----
    for(ItemData *id : mSourceItem->sourceData().objects()){
        ItemData * idCopy = ItemData::copy(id);
        mData.push_back(idCopy);
        if(mSourceItem->countableData() && mSourceItem->countableData()->name()==idCopy->name()){
            mCountableData = idCopy;
        }

        idCopy->setupInternalDataConnections(this);
    }

    //----
    for(SourceItem *si : mSourceItem->childSourceItems()){

        if(si->nativeChild()){
            Item *item = si->parentStorage()->acquireAndSetupItem(si, this);
            mChildItems.push_back(item);

        }else{
            addChildItem(si);
        }

    }

    built = true;

    return true;

}


bool Item::setup_copyItem(SourceItem *_sourceItem, Item *_parentItem, Item *_src)
{

    assert(built==false);

    mSourceItem = _sourceItem;
    mParentItem = _parentItem;


    //----
    for(ItemData *id : _src->dataStorage()){
        ItemData * idCopy = ItemData::copy(id);
        mData.push_back(idCopy);
        if(mSourceItem->countableData() && mSourceItem->countableData()->name()==idCopy->name()){
            mCountableData = idCopy;
        }

        idCopy->setupInternalDataConnections(this);
    }


    //----
    for(Item *i : _src->childItems()){
        Item *item = i->sourceItem()->parentStorage()->acquireAndCopyItem(i->sourceItem(), this, i);
        mChildItems.push_back(item);
    }

    built = true;

    return true;

}



/*
bool Item::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing item '" + name() + "' ...");

    if(name()=="hero"){
        DummyFunction();
    }


    for(ItemData *id : mData){
        if(id->initConnections(_scene)==false){
            return false;
        }
    }

    for(Item* i : mChildItems){
        if(i->initConnections(_scene)==false){
            return false;
        }
    }


    //for(ItemInfluence & ai : mAttributeInfluences){
    //    if(ai.initConnections(_characterSystem)==false){
    //        return false;
    //    }
   // }


    dbgSystem.removeLastMessage();
    return true;

}
*/


bool Item::addChildItem(SourceItem *_sourceItem)
{

    if(StdVector::contains(mSourceItem->allowedChildSourceItems(), _sourceItem)==false){
        return false;
    }


    //if(_sourceItem->type()==ItemType::COUNTABLE && _sourceItem->proxy()){
    //    checkAndAddCountableProxyItem(_sourceItem->proxy());
    //}


    Item *item = _sourceItem->parentStorage()->acquireAndSetupItem(_sourceItem, this);
    addChildItem(item, AddChildItemMode::MOVE);
    //mChildItems.push_back(item);

    return true;
}



Item* Item::checkAndAddCountableProxyItem(SourceItem *_proxySourceItem)
{

    Item* proxyItem = findChildItem(_proxySourceItem->name(), false);

    if(proxyItem==nullptr){
        proxyItem = _proxySourceItem->parentStorage()->acquireAndSetupItem(_proxySourceItem, this);
        mChildItems.push_back(proxyItem);
    }

    return proxyItem;

}


bool Item::addChildItem(Item *_item, AddChildItemMode _mode)
{

    //assert(_item->parentItem()==nullptr);       // Item must be taken from other item (or new)


    if(_item->type()==ItemType::COUNTABLE){


        Item *currentChildItem = nullptr;

        if(mSourceItem->proxy()){

            if(_item->sourceItem()->proxy()==mSourceItem->proxy()){
                currentChildItem = this;
            }

        }else{

            if(_item->sourceItem()->proxy()){
                currentChildItem = checkAndAddCountableProxyItem(_item->sourceItem()->proxy());
            }else{
                currentChildItem = findChildItem(_item->name(), false);
            }
        }

        if(currentChildItem){
            ItemData * addedCountableData = _item->countableData();                 assert(addedCountableData);
            //ItemData * currentData = findData(addedCountableData->name());          assert(currentData);
            ItemData * currentData = currentChildItem->findData(addedCountableData->name());          assert(currentData);
            assert(addedCountableData->type()==currentData->type());

            if(addedCountableData->type()==ItemDataType::INT){
                IntItemData *dAdded = static_cast<IntItemData*>(addedCountableData);
                IntItemData *dCurrent = static_cast<IntItemData*>(currentData);
                int currentValue = dCurrent->value();
                dCurrent->setValue(currentValue + dAdded->value());

            }else if(addedCountableData->type()==ItemDataType::FLOAT){
                FloatItemData *dAdded = static_cast<FloatItemData*>(addedCountableData);
                FloatItemData *dCurrent = static_cast<FloatItemData*>(currentData);
                float currentValue = dCurrent->value();
                dCurrent->setValue(currentValue + dAdded->value());
            }

            //---
            if(_mode==AddChildItemMode::MOVE){
                //_item->sourceItem()->parentStorage()->itemsPool().releaseObject(_item);
                _item->sourceItem()->parentStorage()->releaseItem(_item);
            }

            if(mCallbackOnChangedChildreen){
                if(mSourceItem->type()==ItemType::SELECTABLE_GROUP && mChildItems.size()==1 && mChildItems[0]->isSelected()==false){
                    mCallbackOnChangedChildreen->onChildItemChanged(this, currentChildItem, nullptr, currentChildItem);
                }else{
                    mCallbackOnChangedChildreen->onChildItemChanged(this, currentChildItem, nullptr, nullptr);
                }
            }

        }else{

            Item *addedItem = _item;

            if(_mode==AddChildItemMode::COPY){
                addedItem = _item->sourceItem()->parentStorage()->acquireAndCopyItem(_item->sourceItem(), this, _item);
            }

            /*
            if(_mode==AddChildItemMode::MOVE){
                addedItem = _item;
                mChildItems.push_back(_item);

                if(mCallbackOnChangedChildreen){
                    mCallbackOnChangedChildreen->onChildItemChanged(this, _item, nullptr, nullptr);
                }

            }else{
                Item *itemCopy = _item->sourceItem()->parentStorage()->acquireAndCopyItem(_item->sourceItem(), this, _item);
                //*itemCopy = *_item;
                mChildItems.push_back(itemCopy);

                if(mCallbackOnChangedChildreen){

                    mCallbackOnChangedChildreen->onChildItemChanged(this, itemCopy, nullptr, nullptr);
                }
            }
            */

            mChildItems.push_back(addedItem);

            if(mCallbackOnChangedChildreen){
                if(mSourceItem->type()==ItemType::SELECTABLE_GROUP && mChildItems.size()==1 && mChildItems[0]->isSelected()==false){
                    mCallbackOnChangedChildreen->onChildItemChanged(this, addedItem, nullptr, addedItem);
                }else{
                    mCallbackOnChangedChildreen->onChildItemChanged(this, addedItem, nullptr, nullptr);
                }
            }
        }


    }else{

        Item *addedItem = _item;

        if(_mode==AddChildItemMode::COPY){
            addedItem = _item->sourceItem()->parentStorage()->acquireAndCopyItem(_item->sourceItem(), this, _item);
        }

        /*
        if(_mode==AddChildItemMode::MOVE){
            mChildItems.push_back(_item);

            if(mCallbackOnChangedChildreen){
                mCallbackOnChangedChildreen->onChildItemChanged(this, _item, nullptr, nullptr);
            }

        }else{

            Item *itemCopy = _item->sourceItem()->parentStorage()->acquireAndCopyItem(_item->sourceItem(), this, _item);
            //*itemCopy = *_item;
            mChildItems.push_back(itemCopy);

            if(mCallbackOnChangedChildreen){
                mCallbackOnChangedChildreen->onChildItemChanged(this, itemCopy, nullptr, nullptr);
            }
        }
        */

        mChildItems.push_back(addedItem);

        if(mCallbackOnChangedChildreen){
            if(mSourceItem->type()==ItemType::SELECTABLE_GROUP && mChildItems.size()==1 && mChildItems[0]->isSelected()==false){
                mCallbackOnChangedChildreen->onChildItemChanged(this, addedItem, nullptr, addedItem);
            }else{
                mCallbackOnChangedChildreen->onChildItemChanged(this, addedItem, nullptr, nullptr);
            }
        }

    }


    return true;

}


bool Item::removeChildItem(SourceItem *_sourceItem)
{


    Item *item = nullptr;

    if(_sourceItem->type()==ItemType::COUNTABLE){

        if(_sourceItem->proxy()){
            item = findChildItem(_sourceItem->proxy());
        }else{
            item = findChildItem(_sourceItem);
        }

        if(item==nullptr){
            return false;
        }

        ItemData * removedCountableData = _sourceItem->countableData();                 assert(removedCountableData);
        //ItemData * currentData = findData(addedCountableData->name());                assert(currentData);
        ItemData * currentData = item->findData(removedCountableData->name());          assert(currentData);
        assert(removedCountableData->type()==currentData->type());
        bool removeItem = false;

        if(removedCountableData->type()==ItemDataType::INT){
            IntItemData *dRemoved = static_cast<IntItemData*>(removedCountableData);
            IntItemData *dCurrent = static_cast<IntItemData*>(currentData);
            int currentValue = dCurrent->value();
            dCurrent->setValue(currentValue - dRemoved->value());

            if(dCurrent->value()<=dCurrent->minValue()){
                removeItem = true;
            }

        }else if(removedCountableData->type()==ItemDataType::FLOAT){
            FloatItemData *dRemoved = static_cast<FloatItemData*>(removedCountableData);
            FloatItemData *dCurrent = static_cast<FloatItemData*>(currentData);
            float currentValue = dCurrent->value();
            dCurrent->setValue(currentValue - dRemoved->value());

            if(dCurrent->value()<=dCurrent->minValue()){
                removeItem = true;
            }
        }

        if(removeItem){
            /*
            StdVector::removeElement(mChildItems, item);
            if(mCallbackOnChangedChildreen){
                mCallbackOnChangedChildreen->onChildItemChanged(this, nullptr, item);
            }
            item->sourceItem()->parentStorage()->releaseItem(item);
            */
            removeChildItem(item);
        }

    }else{
        std::vector<Item*>items;
        collectChildItems(items, _sourceItem);

        for(Item *i : items){
            if(i->isSelected()){
                item = i;
                break;
            }
        }
        if(item==nullptr && items.size()>0){
            item = items[0];
        }
        if(item){
            removeChildItem(item);
        }
    }

    return true;

}


bool Item::removeChildItem(Item *_item)
{

    Item *newSelectedItem = nullptr;

    int index = StdVector::indexOfElement(mChildItems, _item);
    if(index==-1){
        return false;
    }

    StdVector::removeElement(mChildItems, _item);

    if(index>=mChildItems.size()){
        index--;
    }

    if(_item->isSelected() && index>=0 && index<mChildItems.size()){
        newSelectedItem = mChildItems[index];
    }

    if(mCallbackOnChangedChildreen){
        mCallbackOnChangedChildreen->onChildItemChanged(this, nullptr, _item, newSelectedItem);
    }
    _item->sourceItem()->parentStorage()->releaseItem(_item);

    return true;

}



Item * Item::findFirstChildItem(SourceItem *_sourceItem)
{

    if(sourceItem()==_sourceItem){
        return this;
    }

    for(Item *i : mChildItems){
        Item *item = i->findFirstChildItem(_sourceItem);
        if(item){
            return item;
        }
    }

    return nullptr;

}


Item* Item::findChildItem(SourceItem *_sourceItem, bool _searchForProxy)
{

    for(Item *i : mChildItems){
        if(i->sourceItem()==_sourceItem){
            return i;
        }
    }

    if(_searchForProxy && _sourceItem->proxy()){
        for(Item *i : mChildItems){
            if(i->sourceItem()==_sourceItem->proxy()){
                return i;
            }
        }
    }

    return nullptr;
}


void Item::collectChildItems(std::vector<Item *> & _collectedItems, SourceItem *_sourceItem)
{

    for(Item *i : mChildItems){
        if(i->sourceItem()==_sourceItem){
            _collectedItems.push_back(i);
        }
    }

}


void Item::collectChildItemsWithTagFlag(std::vector<Item *> &_collectedItems, unsigned int _tagFlags)
{

    for(Item *i : mChildItems){
        if(i->sourceItem()->tagFlags() & _tagFlags){
            _collectedItems.push_back(i);
        }
    }
}


Item* Item::findSelectedChildItem()
{

    for(Item *i : mChildItems){
        if(i->isSelected()){
            return i;
        }
    }

    return nullptr;
}


Item* Item::findChildItem(const std::string _name, bool _setErrorMessage)
{

    for(Item *i : mChildItems){
        if(i->name()==_name){
            return i;
        }
    }


    if(_setErrorMessage){
        dbgSystem.addMessage("Item '" + _name +"' not found!");
    }
    return nullptr;
}



ItemData* Item::findData(const std::string _name, bool _setErrorMessage)
{

    for(ItemData *id : mData){
        if(id->name()==_name){
            return id;
        }
    }


    if(_setErrorMessage){
        dbgSystem.addMessage("Item data '" + _name +"' not found!");
    }
    return nullptr;
}


void Item::findChildItemDataViaPath(const std::string &_path, ItemDataPair &idp, ParseItemMode _mode, bool _setErrorMessage)
{


    std::vector<std::string> itemPathParts = StdString::splitString(_path, ":");

    findChildItemDataViaPath(itemPathParts, idp, _mode, _setErrorMessage);

    if(idp.item==nullptr){
        if(_setErrorMessage){
            dbgSystem.addMessage("Item with path '" + _path + "' not found!");
        }
    }
    if(_mode==ParseItemMode::DATA && idp.data==nullptr){
        if(_setErrorMessage){
            dbgSystem.addMessage("Data with path '" + _path + "' not found!");
        }
    }

}


void Item::findChildItemDataViaPath(std::vector<std::string>& itemPathParts, ItemDataPair &idp, ParseItemMode _mode, bool _setErrorMessage)
{

    if(itemPathParts.empty()){
        if(_mode==ParseItemMode::ITEM){
            idp.item = this;
        }
        return;
    }

    std::string dataName;
    if(_mode==ParseItemMode::DATA){
        dataName = itemPathParts.back();
        itemPathParts.pop_back();
    }


    Item *item = this;

    bool errorInPath = false;
    unsigned int i=0;


    while(i<itemPathParts.size()){

        const std::string &itemName = itemPathParts.at(i);
        errorInPath = false;
        bool stateFound = false;

        for(Item* i : item->childItems()){
            if(i->name() == itemName){
                item = i;
                stateFound = true;
                break;
            }
        }

        if(stateFound==false){
            errorInPath = true;
            break;
        }

        i++;
    }

    if(errorInPath){
        return;
    }

    idp.item = item;

    //---
    if(dataName.empty()==false){
        idp.data = item->findData(dataName);
    }

}


/*
bool Item::collectData(ItemDataCollectingHelper &helper, float &collected)
{





    if(helper.childSourceItemsDepthIndex < helper.childSourceItems.size()){

        SourceItem * si = helper.childSourceItems[helper.childSourceItemsDepthIndex];
        helper.childSourceItemsDepthIndex ++;

        for(Item * i : mChildItems){
            if(i->sourceItem()==si){

                if(mItemCfg->isExclusiveGroup() && i->isSelected()==false){
                    break;
                }

                bool ok = i->collectData(helper, collected);
                if(ok==false){
                    return false;
                }
            }
        }

    }else{

        assert(mData.empty()==false && mData.front()->name()=="amount");

        IntItemData *amount = static_cast<IntItemData*>(mData.front());
        if(amount->mValue>0){
            for(ItemData *id : mData){
                if(id->name()==helper.itemDataName){
                    //collected +=
                }

            }
        }




    }



    return true;
}
*/

//=========================================================================================


bool SourceItemsStorage::preload_countObjects(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Preloading items ... !");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){

        std::string nodeName = std::string(n.name());

        if(nodeName=="item"){

            std::string itemName = n.attribute("name").as_string();
            if(itemName.empty()){
                dbgSystem.addMessage("Missing item name!");
                return false;
            }
            if(StdVector::contains(sourceItemsNames, itemName)){
                dbgSystem.addMessage("Item name '" + itemName +"' already used!");
                return false;
            }
            sourceItemsNames.push_back(itemName);
        }
    }


    dbgSystem.removeLastMessage();
    return true;
}



bool SourceItemsStorage::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading items ... !");


    //-----
    mGlobalSourceItems.preBuild(sourceItemsNames.size());
    for(unsigned int i=0; i<sourceItemsNames.size(); i++){
         mGlobalSourceItems.objects()[i].mName = sourceItemsNames[i];
    }


    //-----
    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){

        std::string nodeName = std::string(n.name());

        if(nodeName=="itemTags"){

            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                std::string childNodeName = std::string(nChild.name());

                if(childNodeName=="tag"){

                    for(pugi::xml_attribute a = nChild.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="name"){
                            std::string sValue = a.as_string();
                            if(mItemTags.addObject(sValue)==nullptr){
                                return false;
                            }

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



        }else if(nodeName=="item"){

            /*
            std::string itemName = n.attribute("name").as_string();

            if(mSourceItems.getObject(itemName,false))
            if(itemName.empty()){
                dbgSystem.addMessage("Missing item name!");
                return false;
            }

            SourceItem *sourceItem =  mSourceItems.addObject(SourceItem(itemName));
            */

            SourceItem *sourceItem = mGlobalSourceItems.getObject(n.attribute("name").as_string());
            assert(sourceItem);

            //if(sourceItem==nullptr){
            //    return false;
            //}

            if(sourceItem->initCfg(n)==false){
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


bool SourceItemsStorage::buildObjects(PlayedScene *_scene)
{

    dbgSystem.addMessage("Building items ... !");


    for(unsigned int i=0; i<mItemTags.objects().size(); i++){
        ItemTag &it = mItemTags.objects()[i];
        unsigned int tagFlag = 1 << i;
        it.setTagFlag(tagFlag);
    }

    //---
    for(SourceItem &si : mGlobalSourceItems.objects()){
        if(si.build(this)==false){
            return false;
        }
        //si.buildLUT(si.itemDataIdentifiers(), "", 0);
    }


    dbgSystem.removeLastMessage();

    return true;

}


bool SourceItemsStorage::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing items ... !");

    for(SourceItem &si : mGlobalSourceItems.objects()){
        if(si.initConnections(_scene)==false){
            return false;
        }
    }


    dbgSystem.removeLastMessage();

    return true;

}


SourceItem* SourceItemsStorage::findSourceItem(const std::string &_sourceItemName, bool _setErrorMessage)
{
    return mGlobalSourceItems.getObject(_sourceItemName, _setErrorMessage);
}



Item *SourceItemsStorage::acquireAndSetupItem(SourceItem* _sourceItem, Item *_parentItem)
{

    Item *item = mItemsPool.acquireObject();

    if(item->setup_newItem(_sourceItem, _parentItem)==false){
        return nullptr;
    }

    return item;
}


Item *SourceItemsStorage::acquireAndCopyItem(SourceItem* _sourceItem, Item *_parentItem, Item* _srcItem)
{

    Item *item = mItemsPool.acquireObject();

    if(item->setup_copyItem(_sourceItem, _parentItem, _srcItem)==false){
        return nullptr;
    }

    return item;
}


void SourceItemsStorage::releaseItem(Item *_item)
{

    mItemsPool.releaseObject(_item);
    _item->clearContent();

}



}
