#include <limits>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmSprite.h"
#include "jmCompositeSprite.h"
#include "jmStreams.h"

#include "logic/jpLogicState.h"
#include "jpPlayedScene.h"
#include "jpPlayedApp.h"

#include "jpCharacterSystem.h"
#include "jpItemUtilities.h"
#include "jpCharacterBase.h"


namespace jugimap{



/*

bool SourceCharacter::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading character '" + mName +"' ...");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="add"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="items"){
                    std::string sClasses = a.as_string();
                    items = StdString::splitString(sClasses, ",");

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

    dbgSystem.removeLastMessage();
    return true;

}


//====================================================================================


bool Character::build(CharacterSystem *_characterSystem)
{



    for(const std::string & s : mSourceCharacter->items){

        ItemCfg *sc = _characterSystem->sourceItems().getObject(s);
        if(sc==nullptr){
            return false;
        }

        Item *c = new Item(sc);

        if(c->build(_characterSystem)==false){
            return false;
        }

        mItems.addObject(c);
    }



    return true;

}

*/




bool SourceItemDataException::initCfg(const pugi::xml_node &_node)
{

    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());


        if(attributeName=="case"){
            mCaseCfg = a.as_string();


        }else if(attributeName=="factor"){
            std::string sValue = a.as_string();

            if(parseFloatData(sValue, mFactor)==false){
                return false;
            }


        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }
}


bool SourceItemDataException::build(CharacterSystem *_characterSystem)
{

    std::vector<std::string>parts = StdString::splitString(mCaseCfg, "/");

    if(parts.size()!=3){
        dbgSystem.addMessage("Error parsing '"+ mCaseCfg + "'!");
        return false;
    }

    const std::string & action = parts[0];
    const std::string & actionMember = parts[1];
    const std::string & actionMemberChildItem = parts[2];




    return true;


}



//====================================================================================

/*

bool AttributeInfluenceExceptionCfg::initCfg(const pugi::xml_node &_node)
{


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="case"){
            _case = a.as_string("");

        }else if(attributeName=="value"){
            std::string sValue = a.as_string();

            if(StdString::floatNumber(sValue, value)==false){
                return false;
            }

        }else if(attributeName=="factor"){
            std::string sValue = a.as_string();

            if(StdString::floatNumber(sValue, factor)==false){
                return false;
            }

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }

    return true;

}


bool AttributeInfluenceException::initConnections()
{



    return true;
}



//====================================================================================



bool ItemInfluenceCfg::initCfg(const pugi::xml_node &_node)
{

    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="item"){
            influencedItemData = a.as_string("name");

        }else if(attributeName=="value"){
            std::string sValue = a.as_string();

            if(sValue.substr(0,5)=="DATA:"){
                influencingItemData = sValue.substr(5);

            }else{
                if(StdString::floatNumber(sValue, value)==false){
                    return false;
                }
            }

        }else if(attributeName=="factor"){
            std::string sValue = a.as_string();

            if(StdString::floatNumber(sValue, factor)==false){
                return false;
            }

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }



    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="exception"){

            exceptions.emplace_back();
            AttributeInfluenceExceptionCfg & e = exceptions.back();

            if(e.initCfg(n)==false){
                return false;
            }

        }else{

            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;

        }
    }

    return true;

}


bool ItemInfluence::build(CharacterSystem *_characterSystem)
{

    dbgSystem.addMessage("Building attribute influence ...");


    for(AttributeInfluenceExceptionCfg & cfg : mCfg->exceptions){
        mInfluenceExceptions.emplace_back(& cfg);

    }

    dbgSystem.removeLastMessage();
    return true;

}


bool ItemInfluence::initConnections(CharacterSystem *_characterSystem)
{

    dbgSystem.addMessage("Initializing attribute influence ...");


    std::string influencedItem;
    std::string influencedItemData;
    std::vector<std::string>parts = StdString::splitString(mCfg->influencedItemData, ":");
    if(parts.size()>0){
        influencedItem = parts[0];
    }
    if(parts.size()>1){
        influencedItemData= parts[1];
    }
    if(influencedItem.empty()){
        dbgSystem.addMessage("Error parsing '" + mCfg->influencedItemData + "'!");
        return false;
    }


    mInfluencedSourceItem = _characterSystem->sourceItems().getObject(influencedItem);
    if(mInfluencedSourceItem==nullptr){
        return false;
    }

    mInfluencedItemSourceData = mInfluencedSourceItem->findSourceData(influencedItemData);
    if(mInfluencedItemSourceData==nullptr){
        return false;
    }


    //---
    if(mCfg->influencingItemData.empty()==false){
        mParentItemInfluencingData = mParentItem->findData(mCfg->influencingItemData);

        if(mParentItemInfluencingData==nullptr){
            return false;
        }
    }


    for(AttributeInfluenceException &aie : mInfluenceExceptions){
        if(aie.initConnections()==false){
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;
}


*/


bool ItemSourceData::initCfg_exceptions(const pugi::xml_node &_node)
{

    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="exception"){

            mExceptions.emplace_back();
            SourceItemDataException &e = mExceptions.back();

            if(e.initCfg(n)==false){
                return false;
            }


        }else{

            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;

        }
    }
}


//===================================================================================


IntSourceItemData::IntSourceItemData(const std::string &_name) : ItemSourceData(_name, ItemDataType::INT),
        mMinValue(0),
        mMaxValue(std::numeric_limits<int>::max()),
        mStartValue(0)
{
}


bool IntSourceItemData::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading item data '" + mName +"' ...");


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());


        if(attributeName=="name"){
            mName = a.as_string();


        }else if(attributeName=="minValue"){
            std::string sValue = a.as_string();

            if(parseIntData(sValue, mMinValue)==false){
                return false;
            }

        }else if(attributeName=="maxValue"){
            std::string sValue = a.as_string();

            if(parseIntData(sValue, mMaxValue)==false){
                return false;
            }

        }else if(attributeName=="startValue"){
            std::string sValue = a.as_string();

            if(parseIntData(sValue, mStartValue)==false){
                return false;
            }

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }

    initCfg_exceptions(_node);


    dbgSystem.removeLastMessage();
    return true;

}


//-------------------------------------------------------------------------------------


FloatSourceItemData::FloatSourceItemData(const std::string &_name): ItemSourceData(_name, ItemDataType::FLOAT),
        mMinValue(0.0f),
        mMaxValue(std::numeric_limits<float>::max()),
        mStartValue(0.0f)
{
}

bool FloatSourceItemData::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading item data '" + mName +"' ...");


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());


        if(attributeName=="name"){
            mName = a.as_string();


        }else if(attributeName=="minValue"){
            std::string sValue = a.as_string();

            if(parseFloatData(sValue, mMinValue)==false){
                return false;
            }

        }else if(attributeName=="maxValue"){
            std::string sValue = a.as_string();

            if(parseFloatData(sValue, mMaxValue)==false){
                return false;
            }


        }else if(attributeName=="startValue"){
            std::string sValue = a.as_string();

            if(parseFloatData(sValue, mStartValue)==false){
                return false;
            }


        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}


//-------------------------------------------------------------------------------------


BoolSourceItemData::BoolSourceItemData(const std::string &_name) : ItemSourceData(_name, ItemDataType::BOOL),
        mStartValue(false)
{
}


bool BoolSourceItemData::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading item data '" + mName +"' ...");


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
            mName = a.as_string();


        }else if(attributeName=="startValue"){
            std::string sValue = a.as_string();

            if(StdString::boolValue(sValue, mStartValue)==false){
                return false;
            }

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}



//====================================================================================


const std::string & ItemData::name()
{

    if(mType==ItemDataType::INT){
        IntItemData *d = static_cast<IntItemData*>(this);
        return d->srcData->mName;

    }else if(mType==ItemDataType::FLOAT){
        FloatItemData *d = static_cast<FloatItemData*>(this);
        return d->srcData->mName;

    }else if(mType==ItemDataType::BOOL){
        BoolItemData *d = static_cast<BoolItemData*>(this);
        return d->srcData->mName;

    }

    assert(false);
    return StdString::dummyString;

}



IntItemData::IntItemData(IntSourceItemData *_srcData, Item *_parentItem): ItemData(ItemDataType::INT, _parentItem),
        srcData(_srcData),
        mValue(_srcData->mStartValue)
{
}


//-------------------------------------------------------------------------------------


FloatItemData::FloatItemData(FloatSourceItemData* _srcData, Item* _parentItem) : ItemData(ItemDataType::FLOAT, _parentItem),
        srcData(_srcData),
        mValue(_srcData->mStartValue)
{
}


BoolItemData::BoolItemData(BoolSourceItemData *_srcData, Item *_parentItem) : ItemData(ItemDataType::BOOL, _parentItem),
        srcData(_srcData),
        mValue(_srcData->mStartValue)
{
}


//====================================================================================

SourceItem::SourceItem(const std::string &_name) :
    mName(_name)
{

    mSourceData.addObject(new IntSourceItemData("amount"));

}


bool SourceItem::initCfg(CharacterSystem *_characterSystem, const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading item '" + mName +"' ...");


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
            //mName = a.as_string("name");

        }else if(attributeName=="amount"){
            std::string sValue = a.as_string();

            IntSourceItemData* d = static_cast<IntSourceItemData*>(mSourceData.getObject("amount"));
            if(parseIntData(sValue, d->mStartValue)==false){
                return false;
            }

        }else if(attributeName=="maxAmount"){
            std::string sValue = a.as_string();

            IntSourceItemData* d = static_cast<IntSourceItemData*>(mSourceData.getObject("amount"));
            if(parseIntData(sValue, d->mMaxValue)==false){
                return false;
            }

        }else if(attributeName=="minAmount"){
            std::string sValue = a.as_string();

            IntSourceItemData* d = static_cast<IntSourceItemData*>(mSourceData.getObject("amount"));
            if(parseIntData(sValue, d->mMinValue)==false){
                return false;
            }


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


        }else if(attributeName=="childItemsDataFactor"){
            std::string sValue = a.as_string();

            if(StdString::floatNumber(sValue, mChildItemsDataFactor)==false){
                return false;
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


        /*
        }else if(nodeName=="influence"){

            mInfluenceCfgs.emplace_back();
            ItemInfluenceCfg & aiCfg = mInfluenceCfgs.back();
            if(aiCfg.initCfg(n)==false){
                return false;
            }
        */

        }else if(nodeName=="item"){

            SourceItem *childItem = _characterSystem->initCfg_addSourceItem(n);
            if(childItem==nullptr){
                return false;
            }

            mChildItemsCfgs.push_back(childItem);

            /*
            std::string itemName = n.attribute("name").as_string();
            if(itemName.empty()){
                dbgSystem.addMessage("Missing item name!");
                return false;
            }


            mChildItemsCfgs.emplace_back(itemName, this);
            ItemCfg & si = mChildItemsCfgs.back();

            if(si.initCfg(n)==false){
                return false;
            }
            */


        }else if(nodeName=="add"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="items"){
                    std::string sClasses = a.as_string();
                    mAddedChildred = StdString::splitString(sClasses, ",");

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


    dbgSystem.removeLastMessage();
    return true;

}


bool SourceItem::build(CharacterSystem *_characterSystem)
{

    dbgSystem.addMessage("Building source item '" + mName +"' ...");


    for(const std::string &s : mAddedChildred){

        SourceItem *sc = _characterSystem->sourceItems().getObject(s);
        if(sc==nullptr){
            return false;
        }

        mChildItemsCfgs.push_back(sc);
    }


    dbgSystem.removeLastMessage();
    return true;
}


bool SourceItem::findChildSourceItemViaPath(const std::string &_path, ParserPathway &_parserPathway, bool _setErrorMessage)
{

    std::vector<std::string> itemPathParts = StdString::splitString(_path, ":");

    if(itemPathParts.empty()){
        if(_setErrorMessage){
            dbgSystem.addMessage("Error parsing path '" + _path + "' !");
        }
        return false;
    }

    std::string dataName = itemPathParts.back();
    itemPathParts.pop_back();


    SourceItem *sourceItem = this;

    bool errorInPath = false;
    unsigned int i=0;
    while(i<itemPathParts.size()){

        const std::string &itemName = itemPathParts.at(i);
        errorInPath = false;
        bool itemFound = false;

        for(unsigned int k=0; k<sourceItem->childSourceItems().size(); k++){

            SourceItem* childItem = sourceItem->childSourceItems()[k];
            if(childItem->name() == itemName){
                sourceItem = childItem;
                _parserPathway.itemIndexes.push_back(k);
                _parserPathway.targetSourceItem = childItem;
                itemFound = true;
                break;
            }
        }

        if(itemFound==false){
            //if(itemName != itemPathParts.back()){        // last name can be also name of the data !
            errorInPath = true;
            //}
            break;
        }

        i++;
    }

    if(errorInPath){
        if(_setErrorMessage){
            dbgSystem.addMessage("Error parsing path '" + _path + "' !");
        }
        return false;
    }


    _parserPathway.targetSourceItem = sourceItem;
    _parserPathway.targetData = sourceItem->sourceData().getObject(dataName);
    if(_parserPathway.targetData==nullptr){
        return false;
    }


    return true;

}



//====================================================================================


Item::~Item()
{

    for(ItemData* id : mData){
        delete id;
    }

}


bool Item::build(CharacterSystem *_characterSystem)
{

    dbgSystem.addMessage("Building item '" + name() + "' ...");


    for(ItemSourceData *isd : mItemCfg->sourceData().objects()){
        if(isd->mType==ItemDataType::INT){
            mData.push_back(new IntItemData(static_cast<IntSourceItemData*>(isd), this));

        }else if(isd->mType==ItemDataType::FLOAT){
            mData.push_back(new FloatItemData(static_cast<FloatSourceItemData*>(isd), this));

        }else if(isd->mType==ItemDataType::BOOL){
            mData.push_back(new BoolItemData(static_cast<BoolSourceItemData*>(isd), this));

        }
    }

    /*
    for(ItemInfluenceCfg & aiCfg : mItemCfg->influenceCfgs()){
        mAttributeInfluences.emplace_back(&aiCfg, this);

        ItemInfluence &ai = mAttributeInfluences.back();
        if(ai.build(_characterSystem)==false){
            return false;
        }
    }
    */

    dbgSystem.removeLastMessage();
    return true;

}


bool Item::initConnections(CharacterSystem *_characterSystem)
{

    dbgSystem.addMessage("Initializing item '" + name() + "' ...");


    //for(ItemInfluence & ai : mAttributeInfluences){
    //    if(ai.initConnections(_characterSystem)==false){
    //        return false;
    //    }
   // }


    dbgSystem.removeLastMessage();
    return true;

}


float Item::obtainInfluence_float(FloatSourceItemData * _itemSourceData)
{


    return 0.0f;
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


//====================================================================================

/*

bool SourceClass::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading class '" + mName +"' ...");


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
            //name = a.as_string("name");

        }else if(attributeName=="exclusive"){
            std::string sValue = a.as_string();

            if(StdString::boolValue(sValue, exclusive)==false){
                return false;
            }


        }else if(attributeName=="influenceFactor"){
            std::string sValue = a.as_string();

            if(StdString::floatNumber(sValue, influenceFactor)==false){
                return false;
            }

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="item"){

            std::string itemName = n.attribute("name").as_string();
            if(itemName.empty()){
                dbgSystem.addMessage("Missing item name!");
                return false;
            }

            mSourceItems.emplace_back(itemName);
            SourceItem & sci = mSourceItems.back();

            if(sci.initCfg(n)==false){
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


//=========================================================================================================


bool Class::build(CharacterSystem *_characterSystem)
{

    dbgSystem.addMessage("Building class '" + mSourceClass->mName + "' ...");


    for(SourceItem & sci : mSourceClass->mSourceItems){

        Item *ci = new Item(&sci);
        if(ci->build(_characterSystem)==false){
            return false;
        }

        mItems.addObject(ci);
    }


    dbgSystem.removeLastMessage();
    return true;

}



bool Class::initConnections()
{

    dbgSystem.addMessage("Initializing class '" + mSourceClass->mName + "' ...");


    for(Item *ci : mItems.objects()){

        if(ci->initConnections()==false){
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}
*/


}
